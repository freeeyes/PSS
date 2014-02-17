using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace iConnect
{
    public class icPacket
    {
        public UInt16 Serverid;
        public APPID Appid;
        public UInt32 Connectid;
        public UInt32 dataid;
        public bool needSerialize = false;
        public object data;
    }
}
