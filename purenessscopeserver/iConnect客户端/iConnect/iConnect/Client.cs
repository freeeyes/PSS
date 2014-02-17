using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Xml;

namespace iConnect
{
    ///<summary>通讯客户端，每一个APP只存在一个实例</summary>
    public class Client
    {
        private string binPath = ""; //配置文件地址
        private int connectcount = 1; // 连接数，目前只用了单线程
        protected AppClass app;
        protected Queue<icPacket> sendbuff = new Queue<icPacket>();
        private Queue<icPacket> recvbuff = new Queue<icPacket>();
        private Queue<SendStatusEventArgs> reportbuff = new Queue<SendStatusEventArgs>();
        protected readonly object Synsend = new object();
        private readonly object Synrecv = new object();
        private readonly object Synreport = new object();
        private List<icSocket> sockets = new List<icSocket>();
        private Thread ReceivedThread;
        private Thread SendstatusThread;
        private Dictionary<ushort, ActServer> Serverlist = new Dictionary<ushort, ActServer>(); //服务器列表
        private UInt32 dataid = 0;
        private readonly object Syndataid = new object();
        protected UInt16 localserverid = 0;
        /// <summary>
        /// 自增包ID
        /// </summary>
        protected UInt32 Dataid
        {
            get
            {
                lock (Syndataid)
                {
                    ++dataid;
                    return dataid;
                }
            }
        }
        /// <summary>
        /// 返回本地服务器ID
        /// </summary>
        public UInt16 LocalServerid
        {
            get { return localserverid; }
        }
        /// <summary>
        /// 返回本地StationID
        /// </summary>
        public string LocalStationid
        {
            get { return ServeridtoStationId(localserverid); }
        }

        //private uint recv = 0;

        protected bool sendbuffblocked = true;
        protected EventWaitHandle sendbuffblockEvent = new EventWaitHandle(false, EventResetMode.ManualReset);
        private EventWaitHandle recvbuffblockEvent = new EventWaitHandle(false, EventResetMode.ManualReset);
        private EventWaitHandle getrecvbuffblockEvent = new EventWaitHandle(false, EventResetMode.ManualReset);
        private EventWaitHandle reportbuffblockEvent = new EventWaitHandle(false, EventResetMode.ManualReset);

        ///<summary>发送状态的事件</summary>
        public event SendStatusDelegate SendStatusEvent;
        public delegate void SendStatusDelegate(SendStatusEventArgs e);
        ///<summary>异步接收数据的事件，设置后将使GetData方法失效</summary>
        public event ReceiveDelegate ReceivedEvent;
        public delegate void ReceiveDelegate(ReceivedobjEventArgs e);
        ///<summary>接收异常的事件</summary>
        public event ExceptionDelegate ExceptionEvent;
        public delegate void ExceptionDelegate(Exception e);
        ///<summary>状态更新事件</summary>
        public event updateClientDelegate updateClientEvent;
        public delegate void updateClientDelegate(ClienUpdateEventArgs e);



        internal Client(AppClass App, string ConfigPath)
        {
            this.binPath = ConfigPath;
            this.app = App;
            XmlDocument doc = new XmlDocument();
            doc.Load(binPath + "Config.xml");
            XmlNodeList nodes = doc.GetElementsByTagName("Connect");
            connectcount = Convert.ToInt32(nodes[0].Attributes.GetNamedItem("count").InnerText);
            XmlNodeList nodes2 = doc.GetElementsByTagName("Server");
            localserverid = Convert.ToUInt16(nodes2[0].Attributes.GetNamedItem("id").InnerText);
            ini();
        }

        private void ini()
        {
            /*if (connectcount < 1)*/ connectcount = 1;
            for (int i = 0; i < connectcount; i++)
            {
                icSocket ics = new icSocket(app, binPath);
                ics.ReceivedEvent += new icSocket.ReceiveDelegate(ics_ReceivedEvent);
                ics.GetDataEvent += new icSocket.GetDataDelegate(ics_GetDataEvent);
                ics.ExceptionEvent += new icSocket.ExceptionDelegate(ics_ExceptionEvent);
                ics.updateClientEvent += new icSocket.updateClientDelegate(ics_updateClientEvent);
                ics.SendStatusEvent += new icSocket.SendStatusDelegate(ics_SendStatusEvent);
                sockets.Add(ics);
            }
            ReceivedThread = new Thread(ReceivedThreadStart);
            ReceivedThread.IsBackground = true;
            ReceivedThread.Start();
            SendstatusThread = new Thread(SendstatusThreadStart);
            SendstatusThread.IsBackground = true;
            SendstatusThread.Start();
        }


        void ics_updateClientEvent(ClienUpdateEventArgs e)
        {
            switch (e.clientupdate)
            {
                case CLIENTUPDATE.updateServer:
                    lock (Serverlist)
                    {
                        foreach(var v in e.data as List<ServerClass>)
                        {
                            if(Serverlist.ContainsKey(v.m_Serverid))
                            {
                                Serverlist[v.m_Serverid].server = v;
                                if(!v.isonline)
                                {
                                    foreach(var z in Serverlist[v.m_Serverid].applist)
                                    {
                                        z.Value.isonline = false;
                                    }
                                }
                            }
                            else
                            {
                                ActServer s = new ActServer();
                                s.server = v;
                                Serverlist.Add(v.m_Serverid, s);
                            }
                        }
                    }
                    if (updateClientEvent != null)
                    {
                        updateClientEvent(e);
                    }
                    break;
                case CLIENTUPDATE.updateApp:
                    lock (Serverlist)
                    {
                        foreach (var v in e.data as List<AppObj>)
                        {
                            if (Serverlist.ContainsKey(v.m_Serverid))
                            {
                               if(Serverlist[v.m_Serverid].applist.ContainsKey(v.app))
                               {
                                   Serverlist[v.m_Serverid].applist[v.app]=v;
                               }
                               else
                               {
                                   Serverlist[v.m_Serverid].applist.Add(v.app, v);
                               }
                               if (v.isonline) Serverlist[v.m_Serverid].server.isonline = true;
                            }
                        }
                    }
                    if (updateClientEvent != null)
                    {
                        List<AppObj> apps = new List<AppObj>();
                        List<APPID> appids = Communicator.getVisualApp(app.id);

                        foreach (var v in e.data as List<AppObj>)
                        {
                            if (appids.Contains(v.app)) apps.Add(v);
                        }
                        e.data = apps;
                        updateClientEvent(e);
                    }
                    break;
                default:
                    break;
            }
        }

        /**********************************************************
         * 
         *  公开方法
         * 
         *********************************************************/
        ///<summary>停止线程和连接，在退出前必须调用</summary>
//         public void Stop()
//         {
//             foreach (var v in sockets)
//             {
//                 v.ExceptionEvent -= ics_ExceptionEvent;
//                 v.Stop();
//             }
//             ReceivedThread.Abort();
//             SendstatusThread.Abort();
//         }
        ///<summary>是否已连接上通讯服务器，返回当前和通讯服务器之间的连接数</summary>
        ///<returns>返回当前和通讯服务器之间的连接数</returns>
        public int isConnected()
        {
            int result = 0;
            foreach (var v in sockets)
            {
                if (v.Connected) result++;
            }
            return result;
        }
        public UInt16 StationIdtoServerid(string stationid)
        {
            lock (Serverlist)
            {
                foreach (var v in Serverlist)
                {
                    if (v.Value.server.m_szStationID == stationid) return v.Value.server.m_Serverid;
                }
                return 0;
            }
        }
        /// <summary>
        /// 从服务器ID转换到StationID
        /// </summary>
        /// <param name="serverid">服务器ID</param>
        /// <returns>返回StationID，没找到则返回空字符串</returns>
        public string ServeridtoStationId(UInt16 serverid)
        {
            lock (Serverlist)
            {
                if (Serverlist.ContainsKey(serverid))
                    return Serverlist[serverid].server.m_szStationID;
                else
                    return "";
            }
        }
        /// <summary>
        /// 主动从接受缓存读取数据。对多线程安全。当ReceivedEvent未设置时启用，如果没数据，或设置了ReceivedEvent，将自动阻塞线程
        /// </summary>
        /// <returns>接受到的数据包</returns>
        public ReceivedobjEventArgs GetData()
        {
            getrecvbuffblockEvent.WaitOne();
            if (ReceivedEvent == null)
            {
                icPacket packet = null;
                lock (Synrecv)
                {
                    if (recvbuff.Count > 0)
                        packet = recvbuff.Dequeue();
                    else
                        getrecvbuffblockEvent.Reset();
                }
                if ((packet != null) && (packet.data != null))
                {
                    ReceivedobjEventArgs e = new ReceivedobjEventArgs(packet.data);
                    e.Appid = packet.Appid;
                    e.Serverid = packet.Serverid;
                    ReOriginData(e);
                    return e;
                }
                else
                {
                    return null;
                }
            }
            else
            {
                getrecvbuffblockEvent.Reset();
                return null;
            }
        }


        /**********************************************************
         * 
         *  内部方法
         * 
         *********************************************************/
        protected bool isAppOnline(UInt16 serverid, UInt16 appid)
        {
            bool result = false;
            lock (Serverlist)
            {
                if (Serverlist.ContainsKey(serverid))
                {
                    if (Serverlist[serverid].applist.ContainsKey((APPID)appid))
                    {
                        result = Serverlist[serverid].applist[(APPID)appid].isonline;
                    }
                }
            }
            return result;
        }

        /**********************************************************
         * 
         *  异常通知模块
         * 
         *********************************************************/
        void ics_ExceptionEvent(Exception e)
        {
            if (ExceptionEvent != null)
            {
                ExceptionEvent(e);
            }
        }
        /**********************************************************
         * 
         *  接收队列处理模块
         * 
         *********************************************************/
        //推送数据
        private void ReceivedThreadStart()
        {
            while (true)
            {
                recvbuffblockEvent.WaitOne();
                if (ReceivedEvent != null)
                {
                    icPacket packet = null;
                    lock (Synrecv)
                    {
                        if (recvbuff.Count > 0)
                            packet = recvbuff.Dequeue();
                        else
                            recvbuffblockEvent.Reset();
                    }
                    if ((packet != null) && (packet.data != null))
                    {
                        ReceivedobjEventArgs e = new ReceivedobjEventArgs(packet.data);
                        e.Appid = packet.Appid;
                        e.Serverid = packet.Serverid;
                        ReOriginData(e);
                        ReceivedEvent(e);
                    }
                }
                else
                   recvbuffblockEvent.Reset();
            }
        }
        protected virtual void ReOriginData(ReceivedobjEventArgs e)
        {

        }
        //Socket接受到数据
        void ics_ReceivedEvent(ReceivedEventArgs e)
        {
            lock (Synrecv)
            {
                recvbuff.Enqueue(e.Packet);
                //如果设置了接受事件，推送线程放行
                if (ReceivedEvent != null)
                    recvbuffblockEvent.Set();
                else
                    getrecvbuffblockEvent.Set();
            }
        }
        /**********************************************************
         * 
         *  推送发送报告处理模块
         * 
         *********************************************************/

        private void SendstatusThreadStart()
        {
            while (true)
            {
                reportbuffblockEvent.WaitOne();
                if (SendStatusEvent != null)
                {
                    SendStatusEventArgs e = null;
                    lock (Synreport)
                    {
                        if (reportbuff.Count > 0)
                            e = reportbuff.Dequeue();
                        else
                            reportbuffblockEvent.Reset();
                    }
                    if (e != null)
                    {
                        SendStatusEvent(e);
                    }
                }
                else
                {
                    reportbuffblockEvent.Reset();
                }
            }
        }
        protected void ics_SendStatusEvent(SendStatusEventArgs e)
        {
            lock (Synreport)
            {
                if (SendStatusEvent != null)
                {
                    reportbuff.Enqueue(e);
                    reportbuffblockEvent.Set();
                }
            }
        }



        /**********************************************************
         * 
         *  发送队列处理模块
         * 
         *********************************************************/
        //Socket取数据
        icPacket ics_GetDataEvent(EventArgs e)
        {
            sendbuffblockEvent.WaitOne();
            icPacket result = null;
            lock (Synsend)
            {
                if (sendbuff.Count > 0) result = sendbuff.Dequeue();
                if (sendbuff.Count == 0)
                {
                    if (!sendbuffblocked)
                    {
                        sendbuffblocked = true;
                        sendbuffblockEvent.Reset();
                    }
                }
                else
                {
                    if (sendbuffblocked)
                    {
                        sendbuffblocked = false;
                        sendbuffblockEvent.Set();
                    }
                }
            }
            return result;
        }
        class ActServer
        {
           internal ServerClass server;
           internal Dictionary<APPID, AppObj> applist = new Dictionary<APPID, AppObj>();
        }
    }
}
