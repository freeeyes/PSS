include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = BaseCommand.o PlugA.o 

LIB_BASE_OBJS = BaseCommand.o PlugA.o 
LIB_BASE = libPlugA.so

all: libPlugA.so clear
			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE):$(PATS)
	$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS) $(LIBS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
clear:	
	mv libPlugA.so ../../../PurenessScopeServer/Linux_Bin/	
cl:	
	rm -rf *.o 
