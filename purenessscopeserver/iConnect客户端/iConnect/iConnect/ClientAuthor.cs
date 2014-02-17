using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace iConnect
{
    public class ClientAuthor : Client
    {
        internal ClientAuthor(AppClass App, string ConfigPath)
            : base(App,ConfigPath)
        {
        }
        ///<summary>异步发送数据，提交后立即返回，数据包在缓存中等待发送</summary>
        ///<param name="data">数据包</param>
        public void Send(string stationid, APPID Appid, PacketManaged data)
        {
            UInt16 serverid = StationIdtoServerid(stationid);
            if (serverid > 0)
            {
                 if (isAppOnline(serverid, (ushort)Appid))
                 {
                    lock (Synsend)
                    {
                        data.checktime.Add(DateTime.Now);
                        icPacket p = new icPacket();
                        p.Serverid = serverid;
                        p.Appid = Appid;
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
