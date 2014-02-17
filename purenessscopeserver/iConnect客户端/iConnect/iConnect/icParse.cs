using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace iConnect
{

    class icParse
    {
        internal static byte[] makeRegPacket(AppClass App)
        {
            const int PacketHeadSize = 4;
            const int CommnadSize = 2;
            const int AppIDSize = 2;
            const int AppTypeSize = 2;
            const int StringSize = 4;
            UInt16 AppID = (UInt16)App.id;
            UInt16 AppType = (UInt16)App.type;
            byte[] appname = Encoding.ASCII.GetBytes(App.appname);
            int appnamel = appname.Length;
            int length = CommnadSize + AppIDSize + AppTypeSize + StringSize + appname.Length;
            byte[] p = new byte[PacketHeadSize + length];
            p[0] = (byte)(length);
            p[1] = (byte)(length >> 8);
            p[2] = (byte)(length >> 16);
            p[3] = (byte)(length >> 24);
            p[4] = unchecked((byte)(COMMAND_ID.COMMAND_REG));
            p[5] = (byte)((UInt16)COMMAND_ID.COMMAND_REG >> 8);
            p[6] = (byte)(AppID);
            p[7] = (byte)(AppID >> 8);
            p[8] = (byte)(AppType);
            p[9] = (byte)(AppType >> 8);
            p[10] = (byte)(appnamel );
            p[11] = (byte)(appnamel >> 8);
            p[12] = (byte)(appnamel >> 16);
            p[13] = (byte)(appnamel >> 24);
            appname.CopyTo(p, PacketHeadSize + CommnadSize + AppIDSize + AppTypeSize + StringSize);
            return p;
        }
        internal static byte[] makeHeartPacket()
        {
            byte[] p = new byte[6];
            p[0] = 2;
            p[4] = unchecked((byte)(COMMAND_ID.COMMAND_KEEP_ALIVE));
            p[5] = (byte)((ushort)COMMAND_ID.COMMAND_KEEP_ALIVE >> 8);
            return p;
        }
        internal static byte[] makeQueryServerPacket()
        {
            byte[] p = new byte[6];
            p[0] = 2;
            p[4] = unchecked((byte)(COMMAND_ID.COMMAND_QUERY_SERVER));
            p[5] = (byte)((UInt16)COMMAND_ID.COMMAND_QUERY_SERVER >> 8);
            return p;
        }
        internal static byte[] makePacketReceived(UInt16 SourServerid, UInt16 SourAppid, UInt16 DestServerid, UInt16 DestAppid, UInt16 Connid, UInt32 dataid)
        {
            UInt32 length = 18;
            byte[] p = new byte[length + 4];
            UInt32 pos = 0;
            p[pos] = (byte)(length); ++pos;
            p[pos] = (byte)(length >> 8); ++pos;
            p[pos] = (byte)(length >> 16); ++pos;
            p[pos] = (byte)(length >> 24); ++pos;
            p[pos] = unchecked((byte)(COMMAND_ID.COMMAND_PACKET_RECEIVED)); ++pos;
            p[pos] = (byte)((UInt16)COMMAND_ID.COMMAND_PACKET_RECEIVED >> 8); ++pos;
            p[pos] = (byte)(SourServerid); ++pos;
            p[pos] = (byte)(SourServerid >> 8); ++pos;
            p[pos] = (byte)(DestServerid); ++pos;
            p[pos] = (byte)(DestServerid >> 8); ++pos;
            p[pos] = (byte)(SourAppid); ++pos;
            p[pos] = (byte)(SourAppid >> 8); ++pos;
            p[pos] = (byte)(DestAppid); ++pos;
            p[pos] = (byte)(DestAppid >> 8); ++pos;
            p[pos] = (byte)(Connid); ++pos;
            p[pos] = (byte)(Connid >> 8); ++pos;
            p[pos] = (byte)(Connid >> 16); ++pos;
            p[pos] = (byte)(Connid >> 24); ++pos;
            p[pos] = (byte)(dataid); ++pos;
            p[pos] = (byte)(dataid >> 8); ++pos;
            p[pos] = (byte)(dataid >> 16); ++pos;
            p[pos] = (byte)(dataid >> 24); ++pos;
            return p;
        }
        internal static byte[] makePacketErr(UInt16 SourServerid, UInt16 SourAppid, UInt16 DestServerid, UInt16 DestAppid, UInt16 Connid, UInt32 dataid)
        {
            UInt32 length = 18;
            byte[] p = new byte[length + 4];
            UInt32 pos = 0;
            p[pos] = (byte)(length); ++pos;
            p[pos] = (byte)(length >> 8); ++pos;
            p[pos] = (byte)(length >> 16); ++pos;
            p[pos] = (byte)(length >> 24); ++pos;
            p[pos] = unchecked((byte)(COMMAND_ID.COMMAND_PACKET_ERR)); ++pos;
            p[pos] = (byte)((UInt16)COMMAND_ID.COMMAND_PACKET_ERR >> 8); ++pos;
            p[pos] = (byte)(SourServerid); ++pos;
            p[pos] = (byte)(SourServerid >> 8); ++pos;
            p[pos] = (byte)(DestServerid); ++pos;
            p[pos] = (byte)(DestServerid >> 8); ++pos;
            p[pos] = (byte)(SourAppid); ++pos;
            p[pos] = (byte)(SourAppid >> 8); ++pos;
            p[pos] = (byte)(DestAppid); ++pos;
            p[pos] = (byte)(DestAppid >> 8); ++pos;
            p[pos] = (byte)(Connid); ++pos;
            p[pos] = (byte)(Connid >> 8); ++pos;
            p[pos] = (byte)(Connid >> 16); ++pos;
            p[pos] = (byte)(Connid >> 24); ++pos;
            p[pos] = (byte)(dataid); ++pos;
            p[pos] = (byte)(dataid >> 8); ++pos;
            p[pos] = (byte)(dataid >> 16); ++pos;
            p[pos] = (byte)(dataid >> 24); ++pos;
            return p;
        }

        internal static byte[] makeDataPacket(byte[] md5, byte[] data, UInt16 SourServerid, UInt16 SourAppid, UInt16 DestServerid, UInt16 DestAppid, UInt32 dataid)
        {
            UInt16 cid = (ushort)COMMAND_ID.COMMAND_PACKET;
            return makePacket(md5, data, cid, SourServerid, SourAppid, DestServerid, DestAppid, 0, dataid);
        }

        private static byte[] makePacket(byte[] md5, byte[] data, UInt16 cid, UInt16 SourServerid, UInt16 SourAppid, UInt16 DestServerid, UInt16 DestAppid, UInt16 Connid, UInt32 dataid)
        {
            const int PacketHeadSize = 4;  //包头大小
            const int CommnadSize = 2;     //命令大小
            const int ConnectSize = 12;     //连接大小
            const int DataidSize = 4;      //数据块编号大小
            const int DataLengthSize = 4;  //数据块长度大小
            const int DataOffsetSize = 4;  //数据块偏移大小
            const int MD5Size = 16;        //MD5 大小
            const int segmentHead = PacketHeadSize + CommnadSize + ConnectSize + DataidSize + DataLengthSize + DataOffsetSize;
            const int segmentSize = 80000; //分段长度  
            int pos = 0;
            int PageSize = 0;              //单个数据包大小
            int datalength = data.Length + MD5Size; //总数据块长度
            int count =(int) Math.Ceiling((double) datalength / segmentSize);  //计算分段数
            int packetsize = datalength + segmentHead * count;            //总缓冲区大小
            int sdataoffset = 0;      // 源数据偏移量
            int ddataoffset = 0;      // 目标数据偏移量
            int segmentlength = 0;    //每段内数据长度
            int packetlength = 0;           //包头，为数据包长度
            byte[] p = new byte[packetsize];
            int dataavlength = datalength;        //待分段的数据长度
            while (dataavlength > 0)
            {
                if (dataavlength > segmentSize)                    //计算每个包长度
                {
                    PageSize = segmentHead + segmentSize;
                }
                else
                {
                    PageSize = segmentHead + dataavlength;
                }
                packetlength = PageSize - PacketHeadSize; //包长度
                segmentlength = PageSize - segmentHead;
                p[pos] = (byte)(packetlength); ++pos;
                p[pos] = (byte)(packetlength >> 8); ++pos;
                p[pos] = (byte)(packetlength >> 16); ++pos;
                p[pos] = (byte)(packetlength >> 24); ++pos;
                p[pos] = (byte)(cid); ++pos;
                p[pos] = (byte)(cid >> 8); ++pos;
                p[pos] = (byte)(SourServerid); ++pos;
                p[pos] = (byte)(SourServerid >> 8); ++pos;
                p[pos] = (byte)(DestServerid); ++pos;
                p[pos] = (byte)(DestServerid >> 8); ++pos;
                p[pos] = (byte)(SourAppid); ++pos;
                p[pos] = (byte)(SourAppid >> 8); ++pos;
                p[pos] = (byte)(DestAppid); ++pos;
                p[pos] = (byte)(DestAppid >> 8); ++pos;
                p[pos] = (byte)(Connid); ++pos;
                p[pos] = (byte)(Connid >> 8); ++pos;
                p[pos] = (byte)(Connid >> 16); ++pos;
                p[pos] = (byte)(Connid >> 24); ++pos;
                p[pos] = (byte)(dataid); ++pos;
                p[pos] = (byte)(dataid >> 8); ++pos;
                p[pos] = (byte)(dataid >> 16); ++pos;
                p[pos] = (byte)(dataid >> 24); ++pos;
                p[pos] = (byte)(datalength); ++pos;
                p[pos] = (byte)(datalength >> 8); ++pos;
                p[pos] = (byte)(datalength >> 16); ++pos;
                p[pos] = (byte)(datalength >> 24); ++pos;
                p[pos] = (byte)(ddataoffset); ++pos;
                p[pos] = (byte)(ddataoffset >> 8); ++pos;
                p[pos] = (byte)(ddataoffset >> 16); ++pos;
                p[pos] = (byte)(ddataoffset >> 24); ++pos;
                if (ddataoffset == 0)
                {
                    Array.Copy(md5, 0, p, pos, MD5Size);
                    pos = pos + MD5Size;
                    ddataoffset = ddataoffset + MD5Size;
                    segmentlength = segmentlength - MD5Size;
                    dataavlength = dataavlength - MD5Size;
                }
                Array.Copy(data, sdataoffset, p, pos, segmentlength);
                pos = pos + segmentlength;
                sdataoffset = sdataoffset + segmentlength; 
                ddataoffset = ddataoffset + segmentlength;
                dataavlength = dataavlength - segmentlength;
                segmentlength = 0;
            }
            return p;
        }
    }
}
