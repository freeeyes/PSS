using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace iConnect
{
    public class ServerClass
    {
        public ushort m_Serverid;
        public string m_szServerIP;
        public string m_szServerName;        //服务器名称
        public string m_szServerVersion;     //服务器版本
        public string m_szStationID;
        public ushort m_nPort;
        public bool isonline = false;
    }

    public class AppObj
    {
        public ushort m_Serverid;
        public APPID app;
        public bool isonline = false;
    }
}
