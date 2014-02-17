using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Security;
using System.Security.Cryptography;
using System.Xml;

namespace iConnect
{
    struct PacketKey
    {
        public UInt16 serverid;
        public UInt16 appid;
        public UInt32 dataid;
    }
    class icSocket
    {
        private AppClass app;
        private bool connected = false;
        private Socket socket;
        private Thread sendouttimeThread;
        private Thread sendThread;
        private Thread recvThread;
        private Thread heartThread;
        private bool Reging = false; //是否已注册
        private bool Reged = false; //是否已注册
        private byte[] buff = new byte[8*1024]; //接受缓存
        private UInt32 bufflength = 0;
        private UInt32 buffindex = 0;
        private byte[] packetbuff; //拼包缓存
        private UInt32 packetbuffindex = 0;
        private bool brokenpacket = false;
        private bool brokenhead = false;
        private UInt16 serverid = 0;
        private string ip = "";
        private int port = 0;
        //private bool havedata = false;
        private readonly object SynObject = new object();
        private MD5 md5Pro = new MD5CryptoServiceProvider();
        private Dictionary<PacketKey, byte[]> bigsegmentbuff = new Dictionary<PacketKey, byte[]>();
        private Dictionary<UInt32, sendingpacket> sendinglist = new Dictionary<UInt32, sendingpacket>();
        private readonly object Synsendinglist = new object();

        private BinaryFormatter sformatter = new BinaryFormatter();
        private BinaryFormatter rformatter = new BinaryFormatter();
        private EventWaitHandle sendblockEvent = new EventWaitHandle(false, EventResetMode.ManualReset);

        private int MinTimeOut = 60;
        private int MinNetSpeed = 10240;

        /// <summary>
        /// 连接状态
        /// </summary>
        public bool Connected
        {
            get { return connected; }
        }

        public event SendStatusDelegate SendStatusEvent;
        public delegate void SendStatusDelegate(SendStatusEventArgs e);
        public event ReceiveDelegate ReceivedEvent;
        public delegate void ReceiveDelegate(ReceivedEventArgs e);
        public event GetDataDelegate GetDataEvent;
        public delegate icPacket GetDataDelegate(EventArgs e);
        public event ExceptionDelegate ExceptionEvent;
        public delegate void ExceptionDelegate(Exception e);
        public event updateClientDelegate updateClientEvent;
        public delegate void updateClientDelegate(ClienUpdateEventArgs e);

        internal icSocket(AppClass App, string ConfigPath)
        {
            this.app = App;
            XmlDocument doc = new XmlDocument();
            doc.Load(ConfigPath + "Config.xml");
            XmlNodeList nodes = doc.GetElementsByTagName("Server");
            ip = nodes[0].Attributes.GetNamedItem("ip").InnerText;
            port = Convert.ToInt32(nodes[0].Attributes.GetNamedItem("port").InnerText);
            serverid = Convert.ToUInt16(nodes[0].Attributes.GetNamedItem("id").InnerText);
            XmlNodeList TimeOutnodes = doc.GetElementsByTagName("TimeOut");
            if (TimeOutnodes.Count > 0)
            {
                MinTimeOut = Convert.ToInt32(TimeOutnodes[0].Attributes.GetNamedItem("min").InnerText);
                MinNetSpeed = Convert.ToInt32(TimeOutnodes[0].Attributes.GetNamedItem("minnetspeed").InnerText);
            }

            sendouttimeThread = new Thread(sendouttimeThreadStart);
            sendThread = new Thread(sendThreadStart);
            recvThread = new Thread(recvThreadStart);
            heartThread = new Thread(heartThreadStart);
            sendThread.Priority = ThreadPriority.BelowNormal;
            sendThread.IsBackground = true;
            heartThread.Priority = ThreadPriority.Lowest;
            heartThread.IsBackground = true;
            recvThread.IsBackground = true;
            sendouttimeThread.IsBackground = true;
            sendouttimeThread.Priority = ThreadPriority.Lowest;
            heartThread.Start();
            sendThread.Start();
            recvThread.Start();
            sendouttimeThread.Start();
        }

        /**********************************************************
         * 
         *  连接模块
         * 
         *********************************************************/

        //掉线重连，5秒间隔
        private void checkconnect()
        {
            lock (SynObject)
            {
                while (socket == null || !socket.Connected)
                {
                    connect();
                    if (!socket.Connected)
                        Thread.Sleep(5000);
                }
            }
        }
        //连接
        private void connect()
        {
            try
            {
                connected = false;
                Reging = false;
                Reged = false;
                sendblockEvent.Reset();
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPAddress ip = IPAddress.Parse(this.ip);
                IPEndPoint ipe = new IPEndPoint(ip, port);
                //socket.ReceiveTimeout = 60000;
                //socket.SendTimeout = 60000;
                socket.NoDelay = true;
                socket.Connect(ipe);
                connected = true;
            }
            catch (Exception e)
            {
                connected = false;
                Reging = false;
                Reged = false;
                sendblockEvent.Reset();
                if (ExceptionEvent != null) ExceptionEvent(e);
            }
        }
        //心跳线程
        private void heartThreadStart()
        {
            while (true)
            {
                try
                {
                    if (!Reged && !Reging)
                    {
                        checkconnect();
                        //登记
                        socket.Send(icParse.makeQueryServerPacket());
                        socket.Send(icParse.makeRegPacket(app));
                        Reging = true;
                    }
                    Thread.Sleep(30000);
                    checkconnect();
                    //心跳
                    socket.Send(icParse.makeHeartPacket());
                }
                catch (Exception e)
                {
                    if (ExceptionEvent != null) ExceptionEvent(e);
                }
            }
        }

        /**********************************************************
         * 
         * 发送模块
         * 
         *********************************************************/

        //发送线程
        private void sendThreadStart()
        {
            MD5 md5Pro = new MD5CryptoServiceProvider();
            while (true)
            {
                try
                {
                    checkconnect();
                    sendblockEvent.WaitOne();
                    if (GetDataEvent != null)
                    {
                        icPacket packet = GetDataEvent(new EventArgs());

                        if (packet != null)
                        {
                            UInt16 Serverid = packet.Serverid;
                            UInt16 Appid = (UInt16)packet.Appid;
                            UInt32 Connectid = packet.Connectid;
                            UInt32 dataid = packet.dataid;
                            object data = packet.data;
                            MemoryStream stream = new MemoryStream();
                            sformatter.Serialize(stream, data);
                            byte[] d = stream.GetBuffer();
                            byte[] md5 = md5Pro.ComputeHash(d);
                            byte[] p = icParse.makeDataPacket(md5, d, serverid, (UInt16)app.id, Serverid, Appid, dataid);
                            UInt32 packetsize = (UInt32)p.Length;
                            lock (Synsendinglist)
                            {
                                sendingpacket sp = new sendingpacket();
                                sp.data = packet.data;
                                sp.packetsize = packetsize;
                                sp.sendtime = DateTime.Now;
                                double t = (sp.packetsize / MinNetSpeed);
                                if (t < MinTimeOut) t = MinTimeOut;
                                sp.outtime = sp.sendtime.AddSeconds(t);
                                sendinglist.Add(dataid, sp);
                            }
                            SendPakcet(p);
                            sendinglist[dataid].sendfinishtime = DateTime.Now;
                        }
                    }
                }
                catch (Exception e)
                {
                    if (ExceptionEvent != null) ExceptionEvent(e);
                }
            }
        }
        //提取包并发送
        private void SendPakcet(byte[] p)
        {
            bool decodeing = true;
            int sendcount = 0;
            int packetlength = BitConverter.ToInt32(p, 0);
            while (decodeing)
            {
                socket.Send(p, sendcount, packetlength + 4, SocketFlags.None);
                sendcount = sendcount + packetlength + 4;
                if (sendcount >= p.Length - 14)
                {
                    decodeing = false;
                }
                else
                {
                    packetlength = BitConverter.ToInt32(p, sendcount);
                    if (packetlength == 0) decodeing = false;
                }
            }
        }
        /**********************************************************
         * 
         *  接收模块
         * 
         *********************************************************/
        //接收线程
        private void recvThreadStart()
        {
            while (true)
            {
                try
                {
                    checkconnect();
                    bufflength = (UInt32) socket.Receive(buff);
                    buffindex = 0;
                    //havedata = true;
                    ParseBuff();
                }
                catch (Exception e)
                {
                    if (ExceptionEvent != null) ExceptionEvent(e);
                }
            }
        }

        //解包收到的数据
        private void ParseBuff()
        {
            if (bufflength > 0)
            {
                UInt32 buffavlength = bufflength - buffindex;
                while (buffavlength > 0)
                {
                    if (!brokenpacket) // 完整包开始
                    {
                        if (buffavlength < 4)
                        {
                            //剩余数据不到一个包头部的长度，复制到临时buff
                            packetbuff = new byte[4];
                            Array.Copy(buff, buffindex, packetbuff, 0, buffavlength);
                            packetbuffindex = buffavlength;
                            buffindex = buffindex + buffavlength;
                            brokenpacket = true;
                            brokenhead = true;
                        }
                        else
                        {
                            UInt32 PacketLength = BitConverter.ToUInt32(buff, (int)buffindex);
                            buffindex = buffindex + 4;
                            buffavlength = bufflength - buffindex;
                            if (buffavlength < PacketLength)
                            {
                                //剩余数据不到一个包的长度，复制到临时buff
                                packetbuff = new byte[PacketLength];
                                Array.Copy(buff, buffindex, packetbuff, 0, buffavlength);
                                packetbuffindex = buffavlength;
                                buffindex = buffindex + buffavlength;
                                brokenpacket = true;
                                brokenhead = false;
                            }
                            else
                            {
                                MergeSegment(buff, buffindex, PacketLength);
                                buffindex = buffindex + PacketLength;
                                brokenpacket = false;
                                brokenhead = false;
                            }
                        }
                    }
                    else
                    {
                        if (brokenhead)
                        {
                            if (buffavlength < (4 - packetbuffindex))
                            {
                                Array.Copy(buff, buffindex, packetbuff, packetbuffindex, buffavlength);
                                packetbuffindex = packetbuffindex + buffavlength;
                                buffindex = buffindex + buffavlength;
                                brokenpacket = true;
                                brokenhead = true;
                            }
                            else
                            {
                                Array.Copy(buff, buffindex, packetbuff, packetbuffindex, 4 - packetbuffindex);
                                buffindex = buffindex + 4 - packetbuffindex;
                                packetbuffindex = packetbuffindex + 4 - packetbuffindex;
                                buffavlength = bufflength - buffindex;
                                UInt32 PacketLength = BitConverter.ToUInt32(packetbuff, 0);
                                if (buffavlength < PacketLength)
                                {
                                    //剩余数据不到一个包的长度，复制到临时buff
                                    packetbuff = new byte[PacketLength];
                                    Array.Copy(buff, buffindex, packetbuff, 0, buffavlength);
                                    packetbuffindex = buffavlength;
                                    buffindex = buffindex + buffavlength;
                                    brokenpacket = true;
                                    brokenhead = false;
                                }
                                else
                                {
                                    MergeSegment(buff, buffindex, PacketLength);
                                    buffindex = buffindex + PacketLength;
                                    brokenpacket = false;
                                    brokenhead = false;
                                }

                            }
                        }
                        else
                        {
                            UInt32 PacketLength = (UInt32)packetbuff.Length;
                            if (buffavlength < (PacketLength - packetbuffindex))
                            {
                                //剩余数据不到一个包的长度，复制到临时buff
                                Array.Copy(buff, buffindex, packetbuff, packetbuffindex, buffavlength);
                                packetbuffindex = packetbuffindex + buffavlength;
                                buffindex = buffindex + buffavlength;
                                brokenpacket = true;
                                brokenhead = false;
                            }
                            else
                            {
                                Array.Copy(buff, buffindex, packetbuff, packetbuffindex, PacketLength - packetbuffindex);
                                buffindex = buffindex + PacketLength - packetbuffindex;
                                MergeSegment(packetbuff, 0, PacketLength);
                                brokenpacket = false;
                                brokenhead = false;
                            }
                        }
                    }
                    buffavlength = bufflength - buffindex;
                }
            }
        }
        //合并分段
        private void MergeSegment(byte[] buff, UInt32 buffindex, UInt32 PacketLength)
        {
            const UInt16 CommnadSize = 2;     //命令大小
            const UInt16 ConnectSize = 12;     //连接大小
            const UInt16 DataidSize = 4;      //数据块编号大小
            const UInt16 DataLengthSize = 4;  //数据块长度大小
            const UInt16 DataOffsetSize = 4;  //数据块偏移大小
            const UInt16 PacketHead = CommnadSize + ConnectSize + DataidSize + DataLengthSize + DataOffsetSize;
            UInt32 segmentlength;
            int command = buff[buffindex] + (buff[buffindex + 1] << 8);
            if (command != (int)COMMAND_ID.COMMAND_PACKET)
            {
                ParseData(buff, buffindex, PacketLength);
            }
            else
            {
                UInt32 datalength = BitConverter.ToUInt32(buff, (int) buffindex + CommnadSize + ConnectSize + DataidSize); //总数据块大小
                if (PacketLength == datalength + PacketHead)   //未分段
                    ParseData(buff, buffindex, PacketLength);
                else
                {
                    UInt16 sourserverid = BitConverter.ToUInt16(buff, (int)buffindex + CommnadSize);
                    UInt16 sourappid = BitConverter.ToUInt16(buff, (int)buffindex + CommnadSize + 4);
                    UInt32 dataid = BitConverter.ToUInt32(buff, (int)buffindex + CommnadSize + 12);
                    PacketKey key = new PacketKey() { serverid = sourserverid, appid = sourappid, dataid = dataid };

                    UInt32 offset = BitConverter.ToUInt32(buff, (int)buffindex + CommnadSize + ConnectSize + DataidSize + DataLengthSize);
                    if(!bigsegmentbuff.ContainsKey(key))
                    {
                        byte[] b = new byte[datalength + PacketHead + 4]; // 前4字节为已接受多少数据
                        bigsegmentbuff.Add(key, b);
                        Array.Copy(buff, buffindex, bigsegmentbuff[key], 4, PacketLength);
                        bigsegmentbuff[key][0] = (byte)(PacketLength);
                        bigsegmentbuff[key][1] = (byte)(PacketLength >> 8);
                        bigsegmentbuff[key][2] = (byte)(PacketLength >> 16);
                        bigsegmentbuff[key][3] = (byte)(PacketLength >> 24);
                    }
                    else
                    {
                        segmentlength = BitConverter.ToUInt32(bigsegmentbuff[key], 0);
                        Array.Copy(buff, buffindex + PacketHead, bigsegmentbuff[key], offset + PacketHead + 4, PacketLength - PacketHead);
                        segmentlength = segmentlength + PacketLength - PacketHead;
                        bigsegmentbuff[key][0] = (byte)(segmentlength);
                        bigsegmentbuff[key][1] = (byte)(segmentlength >> 8);
                        bigsegmentbuff[key][2] = (byte)(segmentlength >> 16);
                        bigsegmentbuff[key][3] = (byte)(segmentlength >> 24);
                    }
                    segmentlength = BitConverter.ToUInt32(bigsegmentbuff[key], 0);
                    if (segmentlength == bigsegmentbuff[key].Length - 4) //最后一个段
                    {
                        ParseData(bigsegmentbuff[key], 4, segmentlength);
                        bigsegmentbuff.Remove(key);
                    }
                }
            }
        }

        //解析
        private void ParseData(byte[] buff, UInt32 buffindex, UInt32 PacketLength)
        {
            int command = (buff[buffindex] + (buff[buffindex + 1] << 8));
            if (command == (int)COMMAND_ID.COMMAND_KEEP_ALIVE_RESPONSE)
            {
            }
            else if (command == (int)COMMAND_ID.COMMAND_REG_RESPONSE)
            {
                byte Regresult = buff[buffindex + 2];
                if (Regresult == 1)
                {
                    Reged = true;
                    Reging = false;
                    sendblockEvent.Set();
                }
                else
                {
                    Reged = false;
                    Reging = false;
                    sendblockEvent.Reset();
                    Exception e = new Exception("应用注册失败，同类应用可能已经在运行。");
                    if (ExceptionEvent != null) ExceptionEvent(e);
                }
            }
            else if (command == (int)COMMAND_ID.COMMAND_PACKET_SERVERDOWN)
            {
                if (SendStatusEvent != null)
                {
                    UInt16 serverid = BitConverter.ToUInt16(buff, (int)buffindex + 2);
                    UInt32 dataid = BitConverter.ToUInt32(buff, (int)buffindex + 4);
                    lock (Synsendinglist)
                    {
                        if (sendinglist.ContainsKey(dataid))
                        {
                            SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_SERVERDOWN, sendinglist[dataid]);
                            sendinglist.Remove(dataid);
                            SendStatusEvent(e);
                        }
                    }
                }
            }
            else if (command == (int)COMMAND_ID.COMMAND_PACKET_APPDOWN)
            {
                if (SendStatusEvent != null)
                {
                    UInt16 serverid = BitConverter.ToUInt16(buff, (int)buffindex + 2);
                    UInt16 sourceappid = BitConverter.ToUInt16(buff, (int)buffindex + 4);
                    UInt16 destappid = BitConverter.ToUInt16(buff, (int)buffindex + 6);
                    UInt32 dataid = BitConverter.ToUInt32(buff, (int)buffindex + 8);
                    lock (Synsendinglist)
                    {
                        if (sendinglist.ContainsKey(dataid))
                        {
                            SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_APPDOWN, sendinglist[dataid]);
                            sendinglist.Remove(dataid);
                            SendStatusEvent(e);
                        }
                    }
                }
            }
            else if (command == (int)COMMAND_ID.COMMAND_PACKET_ERR)
            {
                if (SendStatusEvent != null)
                {
                    UInt32 dataid = BitConverter.ToUInt32(buff, (int)buffindex + 14);
                    lock (Synsendinglist)
                    {
                        if (sendinglist.ContainsKey(dataid))
                        {
                            SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_ERR, sendinglist[dataid]);
                            sendinglist.Remove(dataid);
                            SendStatusEvent(e);
                        }
                    }
                }
            }
            else if (command == (int)COMMAND_ID.COMMAND_PACKET_RECEIVED)
            {
                if(SendStatusEvent != null)
                {
                    UInt32 dataid = BitConverter.ToUInt32(buff, (int)buffindex + 14);
                    lock (Synsendinglist)
                    {
                        if (sendinglist.ContainsKey(dataid))
                        {
                            sendinglist[dataid].outtime = DateTime.Now;
                            SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_RECEIVED, sendinglist[dataid]);
                            sendinglist.Remove(dataid);
                            SendStatusEvent(e);
                        }
                    }
                }
            }
            else if (command == (int)COMMAND_ID.COMMAND_QUERY_SERVER_RESPONSE)
            {
                if(updateClientEvent != null)
                {
                    char[] trimChars = {(char)0 };
                    List<ServerClass> servers = new List<ServerClass>();
                    int servercount = BitConverter.ToUInt16(buff, (int)buffindex + 2);
                    UInt32 pos = buffindex + 4;
                    for(int i = 0; i<servercount; ++i)
                    {
                        ushort serverid = BitConverter.ToUInt16(buff, (int)pos); pos = pos + 2;
                        string stationid = Encoding.Default.GetString(buff, (int)pos, 33).Trim(trimChars); pos = pos + 33;
                        byte online = buff[pos]; ++pos;
                        ServerClass s = new ServerClass();
                        s.m_Serverid = serverid;
                        s.m_szStationID = stationid;
                        s.isonline = (online == 1);
                        servers.Add(s);
                    }
                    ClienUpdateEventArgs e = new ClienUpdateEventArgs(CLIENTUPDATE.updateServer);
                    e.data = servers;
                    updateClientEvent(e);
                }
            }
            else if ((command == (int)COMMAND_ID.COMMAND_QUERY_APP_RESPONSE) || (command == (int)COMMAND_ID.COMMAND_SYN_APP))
            {
                if (updateClientEvent != null)
                {
                    List<AppObj> applist = new List<AppObj>();
                    UInt32 pos = buffindex + 2;
                    ushort serverid = BitConverter.ToUInt16(buff, (int)pos); pos = pos + 2;
                    ushort appcount = BitConverter.ToUInt16(buff, (int)pos); pos = pos + 2;
                    for (int i = 0; i < appcount; ++i)
                    {
                        ushort appid = BitConverter.ToUInt16(buff, (int)pos); pos = pos + 2;
                        byte isadd = buff[pos]; ++pos;
                        AppObj a = new AppObj();
                        a.m_Serverid = serverid;
                        a.app = (APPID)appid;
                        a.isonline = (isadd == 1);
                        applist.Add(a);
                    }
                    ClienUpdateEventArgs e = new ClienUpdateEventArgs(CLIENTUPDATE.updateApp);
                    e.data = applist;
                    updateClientEvent(e);
                }
            }
            //接受到数据包
            else if (command == (int)COMMAND_ID.COMMAND_PACKET)
            {
                if (ReceivedEvent != null)
                {
                    const int CommnadSize = 2;     //命令大小
                    const int ConnectSize = 12;     //连接大小
                    const int DataidSize = 4;      //数据块编号大小
                    const int DataLengthSize = 4;  //数据块长度大小
                    const int DataOffsetSize = 4;  //数据块偏移大小
                    const int MD5Length = 16;
                    const int PacketHead = CommnadSize + ConnectSize + DataidSize + DataLengthSize + DataOffsetSize;
                    bool datacorrect = true;
                    UInt16 SourServerid = BitConverter.ToUInt16(buff, (int)buffindex + 2);
                    UInt16 DestServerid = BitConverter.ToUInt16(buff, (int)buffindex + 4);
                    UInt16 SourAppid = BitConverter.ToUInt16(buff, (int)buffindex + 6);
                    UInt16 DestAppid = BitConverter.ToUInt16(buff, (int)buffindex + 8);
                    UInt32 connid = BitConverter.ToUInt32(buff, (int)buffindex + 10);
                    UInt32 dataid = BitConverter.ToUInt32(buff, (int)buffindex + 14);

                    byte[] smd5 = new byte[MD5Length];
                    Array.Copy(buff, buffindex + PacketHead, smd5, 0, MD5Length);
                    byte[] dmd5 = md5Pro.ComputeHash(buff, (int)buffindex + PacketHead + MD5Length, (int)PacketLength - PacketHead - MD5Length);
                    for (int i = 0; i < MD5Length; i++)
                    {
                        if (smd5[i] != dmd5[i]) datacorrect = false;
                    }
                    //验证数据正确性
                    if (datacorrect)
                    {
                        icPacket packet = new icPacket();
                        object data = null;
                        MemoryStream stream = new MemoryStream(buff, (int)buffindex + PacketHead + MD5Length, (int)PacketLength - PacketHead - MD5Length);
                        data = rformatter.Deserialize(stream);
                        packet.data = data;
                        packet.Serverid = SourServerid;
                        packet.Appid = (APPID)SourAppid;
                        packet.dataid = dataid;
                        ReceivedEventArgs e = new ReceivedEventArgs(packet);
                        ReceivedEvent(e);
                        socket.Send(icParse.makePacketReceived(DestServerid, DestAppid, SourServerid, SourAppid, 0, dataid));
                    }
                    else
                    {
                        socket.Send(icParse.makePacketErr(DestServerid, DestAppid, SourServerid, SourAppid, 0, dataid));
                    }
                }
            }
        }

        /**********************************************************
         * 
         *  超时验证
         * 
         *********************************************************/

        //超时验证线程
        private void sendouttimeThreadStart()
        {
            while (true)
            {
                lock (Synsendinglist)
                {
                    var outlist = from p in sendinglist
                                  where p.Value.outtime < DateTime.Now
                                  select p;
                    foreach ( var v in outlist)
                    {
                        sendinglist.Remove(v.Key);
                        SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_TIMEOUT, v.Value);
                        SendStatusEvent(e);
                    }
                }
                Thread.Sleep(30000);
            }
        }
    }
}
