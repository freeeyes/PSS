using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace iConnect
{
    /// <summary>
    /// 需要走认证的数据包。资源端用SetData填充数据原包发回
    /// </summary>
    [Serializable]
    public class PacketManaged
    {
        internal UInt16 Sserverid;
        internal APPID SAppid;
        public string LoginName;
        public long ServiceArea;
        public ConnectServiceResource ServiceResource;
        internal int datasize = 0;
        internal byte[] SerializedData = null;
        internal object UnSerializedData = null;
        internal List<DateTime> checktime = new List<DateTime>();
        public int DataSize
        {
            get { return datasize; }
        }
        /// <summary>
        /// 发信人服务器ID，只读
        /// </summary>
        public UInt16 SourceServerid
        {
            get { return Sserverid; }
        }
        /// <summary>
        /// 发信人APPID，只读
        /// </summary>
        public APPID SourceAppid
        {
            get { return SAppid; }
        }
        /// <summary>
        /// 数据包沿途每一次发送记录的时间
        /// </summary>
        public List<DateTime> CheckTime
        {
            get { return checktime; }
        }
        /// <summary>
        /// 数据包生成第一次发送的时间
        /// </summary>
        public DateTime SendTime
        {
            get { if (checktime.Count > 0) return checktime[0]; else return DateTime.MinValue; }
        }
        /// <summary>
        /// 设置数据包的自定义数据
        /// </summary>
        /// <param name="data"></param>
        public void SetData (object data)
        {
            UnSerializedData = data;
            SerializedData = null;
        }
        /// <summary>
        /// 读取数据包的自定义数据
        /// </summary>
        /// <returns></returns>
        public object GetData()
        {
            return UnSerializedData;
        }
    }

    public enum ConnectServiceResource
    {
        Undefined = -1,
        Resource1 = 0,
        Resource2 = 1,
        Resource3 = 2
    }
    public enum ConnectServiceArea : long
    {
        Undefined = -1,       //未定义
        Area1 = 0,
        Area2 = 11,
        Area3 = 12
    }

    public enum APPID : ushort
    {
        Unkown = 0,
        Authorization = 1, //认证
        TestApp,
        TestAppResource,
        TestApp2App,
        iMoniter           //监控
    }

    ///<summary>Client的类型。App2App为应用对应用，App2Res为应用对资源</summary>
    public enum APPTYPE : ushort
    {
        App2Res = 1,
        App2App,
        Res2App,
        Authorization,
        Moniter
    }

    public enum CLIENTUPDATE
    {
        updateServer,
        updateApp
    }

    public enum COMMAND_ID : ushort
    {
        COMMAND_REG = 0x1000,
        COMMAND_REG_RESPONSE = 0x1001,

        COMMAND_KEEP_ALIVE = 0x2000,
        COMMAND_KEEP_ALIVE_RESPONSE = 0x2001,
        COMMAND_QUERY_APP = 0x2010,
        COMMAND_QUERY_APP_RESPONSE = 0x2011,
        COMMAND_SYN_APP = 0x2012,
        COMMAND_QUERY_SERVER = 0x2020,
        COMMAND_QUERY_SERVER_RESPONSE = 0x2021,

        COMMAND_PACKET = 0x3000,
        COMMAND_PACKET_RECEIVED = 0x3001,
        COMMAND_PACKET_SERVERDOWN = 0x3002,
        COMMAND_PACKET_APPDOWN = 0x3003,
        COMMAND_PACKET_REJECT = 0x3004,
        COMMAND_PACKET_ERR = 0x3005,
        COMMAND_PACKET_OUTTIME = 0x3006,
    }

    public enum SENDSTATUS
    {
        PACKET_RECEIVED,
        PACKET_SERVERDOWN,
        PACKET_APPDOWN,
        PACKET_REJECT,
        PACKET_ERR,
        PACKET_TIMEOUT,
        PACKET_SERVERUNKOWN,
        PACKET_CANNOTCROSSAPP,
    }

    //public struct Appindex
    //{
    //    public ushort Serverid;
    //    public ushort Appid;
    //}

    public class testEventArgs : EventArgs
    {
        private object packet;

        public testEventArgs(object packet)
        {
            this.packet = packet;
        }
        public object Packet
        {
            get { return packet; }
            set { packet = value; }
        }
    }
    ///<summary>异步发送反馈</summary>
    public class SendStatusEventArgs : EventArgs
    {
        private sendingpacket m_packet;
        private SENDSTATUS m_status;
        private string m_msg;

        public SendStatusEventArgs(SENDSTATUS status, sendingpacket packet)
        {
            this.m_packet = packet;
            this.m_status = status;
            switch (status)
            {
                case SENDSTATUS.PACKET_RECEIVED: 
                    m_msg = "数据已接收";
                    break;
                case SENDSTATUS.PACKET_SERVERDOWN:
                    m_msg = "发送到远程服务器失败，可能远程服务器未在线";
                    break;
                case SENDSTATUS.PACKET_APPDOWN:
                    m_msg = "发送到远程应用失败，可能远程应用未在线";
                    break;
                case SENDSTATUS.PACKET_REJECT:
                    m_msg = "拒绝接收数据包";
                    break;
                case SENDSTATUS.PACKET_ERR:
                    m_msg = "数据包传输错误";
                    break;
                case SENDSTATUS.PACKET_TIMEOUT:
                    m_msg = "数据包传输超时";
                    break;
                case SENDSTATUS.PACKET_CANNOTCROSSAPP:
                    m_msg = "没有给其他应用发送数据的权限";
                    break;
                case SENDSTATUS.PACKET_SERVERUNKOWN:
                    m_msg = "未知的stationid,或Appid错误";
                    break;
                default:
                    break;
            }
        }
        public sendingpacket Packet
        {
            get { return m_packet; }
        }
        public string Msg
        {
            get { return m_msg; }
        }
        public SENDSTATUS Status
        {
            get { return m_status; }
        }
    }
    public class sendingpacket
    {
        /// <summary>
        /// 实际发送的时间
        /// </summary>
        public DateTime sendtime;
        /// <summary>
        /// 实际发送完成的时间
        /// </summary>
        public DateTime sendfinishtime;
        /// <summary>
        /// 正常接收完成包里，这个时间是确认包已收到的时间。在超时包里，这是设定的超时时间。
        /// </summary>
        public DateTime outtime;
        public object data;
        public UInt32 packetsize;
    }
    ///<summary>接受事件的数据，Packet为接受到的数据包</summary>
    public class ReceivedEventArgs : EventArgs
    {
        private icPacket packet;

        public ReceivedEventArgs(icPacket packet)
        {
            this.packet = packet;
        }
        public icPacket Packet
        {
            get { return packet; }
        }
    }
    ///<summary>接受事件的数据，Packet为接受到的数据包</summary>
    public class ReceivedobjEventArgs : EventArgs
    {
        public UInt16 Serverid;
        public APPID Appid;
        private object data;

        public ReceivedobjEventArgs(object data)
        {
            this.data = data;
        }
        public object Data
        {
            get { return data; }
        }
    }
    public class ClienUpdateEventArgs : EventArgs
    {
        private CLIENTUPDATE m_clientupdate;
        private object mdata;

        public ClienUpdateEventArgs(CLIENTUPDATE update)
        {
            m_clientupdate = update;
        }
        /// <summary>
        /// CLIENTUPDATE = updateServer 时，data类型：List(AppObj)，
        /// CLIENTUPDATE = updateApp 时，data类型：List(ServerClass)，
        /// </summary>
        public object data
        {
            get { return mdata; }
            set { mdata = value; }
        }
        public CLIENTUPDATE clientupdate
        {
            get { return m_clientupdate; }
        }
    }
}
