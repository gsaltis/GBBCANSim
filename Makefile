CC			= gcc
LINK			= gcc

CC_FLAGS		= -g -c -Wall -Ilib/include  -DDEVELOPMENT

LINK_FLAGS		= -g -Llib -ldl -lncurses

TARGET1			= cansimws

ALLTARGETS		= $(TARGET1) 

OBJS1			= $(sort 				\
			    AllCanDefinitions.o			\
			    Bay.o				\
			    BayType.o				\
			    BytesManage.o			\
			    CANInterface.o			\
			    CANRegisterDef.o			\
			    CanMsg.o				\
			    DefFileToken.o			\
			    DeviceDef.o                         \
			    DeviceMessageDef.o			\
			    DeviceMessageDef.o			\
			    DeviceRegDef.o			\
			    DeviceRegDef.o			\
			    Devices.o				\
			    DirManagement.o			\
			    FileUtils.o				\
			    FuseBreaker.o			\
			    FuseBreakerType.o			\
			    HTTPServerThread.o			\
			    JSONIF.o				\
			    MemoryManager.o			\
			    NumericTypes.o			\
			    Panel.o				\
			    PanelConnection.o			\
			    PanelType.o				\
			    Rectifier.o				\
			    String.o				\
			    StringUtils.o			\
			    ThreadSafePrint.o			\
			    UserInputHandling.o			\
			    WebSocketIF.o			\
			    WebConnection.o			\
			    jsoncanif.o				\
			    main.o				\
			    linenoise.o				\
			   )

LIBS1			= -ljson -lmongoose -lm -lpthread -lrt

.PHONY			: all clean veryclean libs

all			: libs $(ALLTARGETS)

libs			: 
			  cd lib && make

$(TARGET1)		: $(OBJS1)
			  cd lib && make
			  @echo [LD] $(TARGET1)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET1) $(OBJS1) $(LIBS1)

%.o			: %.c
			  @echo [CC] $@
			  @$(CC) $(CC_FLAGS) $<

include			  depends.mk

junkclean		: 
			  -rm -rf $(wildcard *-bak)

clean			: 
			  -rm -rf $(wildcard *.o $(ALLTARGETS))

