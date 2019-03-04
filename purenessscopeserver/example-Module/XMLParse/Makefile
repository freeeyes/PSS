include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = ./TinyXML/tinystr.o ./TinyXML/tinyxml.o ./TinyXML/tinyxmlerror.o ./TinyXML/tinyxmlparser.o \
	   BaseCommand.o XmlOpeation.o XMLParse.o

LIB_BASE_OBJS = tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o \
				BaseCommand.o XmlOpeation.o XMLParse.o
LIB_BASE = libXMLParse.so

all: libXMLParse.so clear
			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE):$(PATS)
	$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS) $(LIBS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
clear:	
	mv libXMLParse.so ../../PurenessScopeServer/Linux_Bin/	
cl:	
	rm -rf *.o 
