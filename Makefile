CC			= gcc
LINK			= gcc

CC_FLAGS		= -g -c -Wall -Ilib/include  -DDEVELOPMENT

LINK_FLAGS		= -g -Llib -ldl -lncurses -lsqlite3

TARGET			= cansimws

TARGET2			= CANDefsToJSON

ALLTARGETS		= $(TARGET) $(TARGET2)

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

LIBS			= -ljson -lmongoose -lm -lpthread -lsqlite3 -lrt

.PHONY			: all clean veryclean

all			: $(TARGET) $(TARGET2)

$(TARGET)		: $(OBJS)
			  @echo [LD] $(TARGET)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(TARGET2)		: $(OBJS2)
			  @echo [LD] $(TARGET2)
			  @$(LINK) $(LINK_FLAGS) -o $(TARGET2) $(OBJS2) $(LIBS)

%.o			: %.c
			  @echo [CC] $@
			  @$(CC) $(CC_FLAGS) $<

include			  depends.mk

junkclean		: 
			  -rm -rf $(wildcard)

clean			: 
			  -rm -rf $(wildcard *.o $(ALLTARGETS))

