CC			= gcc
LINK			= gcc

CC_FLAGS		= -g -c -Wall -Ilib/include  -DDEVELOPMENT

LINK_FLAGS		= -g -Llib -ldl -lncurses

TARGET1			= cansimws

TARGET2			= cansimwsclient

ALLTARGETS		= $(TARGET1) $(TARGET2)

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
			    ServerUserInput.o			\
			    WebConnection.o			\
			    jsoncanif.o				\
			    main.o				\
			    linenoise.o				\
			   )

OBJS2			= $(sort				\
			    cansimwsclient.o			\
			    String.o				\
			    MemoryManager.o			\
			    FileUtils.o				\
			    ClientUserInput.o			\
			    ThreadSafePrint.o			\
			    linenoise.o				\
			   )

LIBS1			= -ljson -lmongoose -lm -lpthread -lrt
LIBS2			= -lpthread -lmongoose

.PHONY			: all clean veryclean

all			: $(ALLTARGETS)

$(TARGET1)		: $(OBJS1)
			  cd lib && make
			  @echo [LD] $(TARGET1)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET1) $(OBJS1) $(LIBS1)

$(TARGET2)		: $(OBJS2)
			  @echo [LD] $(TARGET2)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET2) $(OBJS2) $(LIBS2)

%.o			: %.c
			  @echo [CC] $@
			  @$(CC) $(CC_FLAGS) $<

include			  depends.mk

junkclean		: 
			  -rm -rf $(wildcard *-bak)

clean			: 
			  -rm -rf $(wildcard *.o $(ALLTARGETS))

