CC			= gcc
LINK			= gcc

CC_FLAGS		= -g -c -Wall -Ilib/include  -DDEVELOPMENT

LINK_FLAGS		= -g -Llib -ldl -lncurses -lsqlite3 

TARGET			= cansimws

TARGET2			= CANDefsToJSON

TARGET3			= cansimwsclient

ALLTARGETS		= $(TARGET) $(TARGET2) $(TARGET3)

OBJS			= $(sort 				\
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
			    SQLStatements.o			\
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
			    CANDefsToJSON.o			\
			    String.o				\
			    MemoryManager.o			\
			    FileUtils.o				\
			    ThreadSafePrint.o			\
			    AllCanDefinitions.o			\
			   )

OBJS3			= $(sort				\
			    cansimwsclient.o			\
			    String.o				\
			    MemoryManager.o			\
			    FileUtils.o				\
			    ClientUserInput.o			\
			    ThreadSafePrint.o			\
			    linenoise.o				\
			   )

LIBS			= -ljson -lmongoose -lm -lpthread -lsqlite3 -lrt
LIBS3			= -lpthread -lmongoose

.PHONY			: all clean veryclean

all			: $(ALLTARGETS)

$(TARGET)		: $(OBJS)
			  cd lib && make
			  @echo [LD] $(TARGET)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(TARGET2)		: $(OBJS2)
			  @echo [LD] $(TARGET2)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET2) $(OBJS2) $(LIBS)

$(TARGET3)		: $(OBJS3)
			  @echo [LD] $(TARGET3)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET3) $(OBJS3) $(LIBS3)

%.o			: %.c
			  @echo [CC] $@
			  @$(CC) $(CC_FLAGS) $<

include			  depends.mk

junkclean		: 
			  -rm -rf $(wildcard *-bak)

clean			: 
			  -rm -rf $(wildcard *.o $(ALLTARGETS))

