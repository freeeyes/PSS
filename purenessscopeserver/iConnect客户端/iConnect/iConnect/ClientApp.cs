using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace iConnect
{
    ///<summary>通讯客户端，每一个APP只存在一个实例</summary>
    public class ClientApp : Client
    {
        internal ClientApp(AppClass App, string ConfigPath)
            : base(App,ConfigPath)
        {
        }
        private BinaryFormatter sformatter = new BinaryFormatter();

        ///<summary>异步发送数据，提交后立即返回，数据包在缓存中等待发送</summary>
        ///<param name="data">数据包</param>
        public void Send(PacketManaged data)
        {
            UInt16 serverid = localserverid;
            UInt16 Appid = (UInt16)APPID.Authorization;
            if ((serverid > 0) && (Appid > 0))
            {
                 if (isAppOnline(serverid, Appid))
                 {
                    lock (Synsend)
                    {
                        //封装寄出地址
                        data.Sserverid = localserverid;
                        data.SAppid = app.id;
                        data.checktime.Add(DateTime.Now);

                        if (data.UnSerializedData != null) //对数据序列化
                        {
                            DateTime date1 = DateTime.Now;
                            MemoryStream stream = new MemoryStream();
                            sformatter.Serialize(stream, data.UnSerializedData);
                            data.SerializedData = stream.GetBuffer();
                            data.UnSerializedData = null;
                            data.datasize = data.SerializedData.Length;
                        }
                        //
                        icPacket p = new icPacket();
                        p.Serverid = serverid;
                        p.Appid = (APPID)Appid;
                        p.dataid = Dataid;
                        p.data = data;
                        sendbuff.Enqueue(p);
                        if (sendbuffblocked)
                        {
                            sendbuffblocked = false;
                            sendbuffblockEvent.Set();
                        }
                    }
                 }
                 else
                 {
                     sendingpacket status = new sendingpacket();
                     status.data = data;
                     status.packetsize = 0;
                     status.sendtime = DateTime.Now;
                     status.outtime = DateTime.Now;
                     SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_APPDOWN, status);
                     ics_SendStatusEvent(e);
                 }
            }
            else
            {
                sendingpacket status = new sendingpacket();
                status.data = data;
                status.packetsize = 0;
                status.sendtime = DateTime.Now;
                status.outtime = DateTime.Now;
                SendStatusEventArgs e = new SendStatusEventArgs(SENDSTATUS.PACKET_SERVERUNKOWN, status);
                ics_SendStatusEvent(e);
            }
        }
    }
}
