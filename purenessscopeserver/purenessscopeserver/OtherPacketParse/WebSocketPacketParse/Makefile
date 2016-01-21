include Makefile.define

# 默认超作
default:all

# 主应用文件列表
LIB_BASE_OBJS = PacketParse.o BuffPacket.o ACEMemory.o PacketParseBase.o
LIB_BASE = libPacketParse.so

all: libPacketParse.so clear
			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE): $(LIB_BASE_OBJS)
	$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
clear:	
	mv libPacketParse.so ../../PurenessScopeServer/Linux_Bin/
cl:	
	rm -rf *.o 
