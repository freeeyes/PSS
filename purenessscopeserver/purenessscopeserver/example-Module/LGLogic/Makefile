include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = BaseCommand.o LGLogic.o LSServerManager.o ListManager.o ./IniRead/dictionary.o ./IniRead/iniparser.o

LIB_BASE_OBJS = BaseCommand.o LGLogic.o LSServerManager.o dictionary.o iniparser.o ListManager.o
LIB_BASE = libLGLogic.so

all: libLGLogic.so clear
			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE):$(PATS)
	$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS) $(LIBS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
clear:	
	mv libLGLogic.so ../../PurenessScopeServer/Linux_Bin/	
cl:	
	rm -rf *.o 
