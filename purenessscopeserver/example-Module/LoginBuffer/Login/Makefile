include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = BaseCommand.o Login.o ../Common/SMOption.o ../Common/UserValidManager.o ../Common/UserInfoManager.o

LIB_BASE_OBJS = BaseCommand.o Login.o SMOption.o UserValidManager.o UserInfoManager.o
LIB_BASE = libLogin.so

all: libLogin.so clear
			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE):$(PATS)
	$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS) $(LIBS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
clear:	
	mv libLogin.so ../../../PurenessScopeServer/Linux_Bin/	
cl:	
	rm -rf *.o 
