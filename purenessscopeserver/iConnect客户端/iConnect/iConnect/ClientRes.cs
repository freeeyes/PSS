using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace iConnect
{
    ///<summary>通讯客户端，每一个APP只存在一个实例</summary>
    public class ClientRes : Client
    {
        private BinaryFormatter rformatter = new BinaryFormatter();
        internal ClientRes(AppClass App, string ConfigPath)
            : base(App,ConfigPath)
        {
        }
        ///<summary>异步发送数据，提交后立即返回，数据包在缓存中等待发送</summary>
        ///<param name="data">数据包</param>
        public void Send(PacketManaged data)
        {
            UInt16 serverid = data.Sserverid;
            UInt16 Appid = (UInt16)data.SAppid;
            if ((serverid > 0) && (Appid > 0))
            {
                 if (isAppOnline(serverid, (ushort)Appid))
                 {
                    lock (Synsend)
                    {
                        data.checktime.Add(DateTime.Now);
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
        protected override void ReOriginData(ReceivedobjEventArgs e)
        {
            base.ReOriginData(e);
            PacketManaged data = (PacketManaged)e.Data;
            if (data.SerializedData != null)
            {
                MemoryStream stream = new MemoryStream(data.SerializedData);
                data.UnSerializedData = rformatter.Deserialize(stream);
                data.SerializedData = null;
            }
        }
    }
}
