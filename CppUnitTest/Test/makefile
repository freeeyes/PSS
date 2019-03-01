include makefile.define

default:all

PATS1 = test_linux.o testcase.o

LIB_BASE_OBJS1 = test_linux.o testcase.o

LIB_BASE1 = Test_Pss

all:$(LIB_BASE1) makefile

$(LIB_BASE1):$(PATS1)
	$(CC) -rdynamic -o $(LIB_BASE1) $(LIB_BASE_OBJS1) $(LIBS)

clean:
	rm -rf *.o  $(LIB_BASE1) $(LIB_BASE_OBJS1)
cl:
	rm -rf *.o 

