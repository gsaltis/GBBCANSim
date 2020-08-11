AllCanDefinitions.o: AllCanDefinitions.c CanMsg.h DeviceDef.h \
 DeviceDefines.h DeviceRegDef.h NumericTypes.h JSONIF.h \
 lib/include/json.h String.h DeviceMessageDef.h AllCanDefinitions.h
Bay.o: Bay.c Bay.h String.h Panel.h lib/include/sqlite3.h FuseBreaker.h \
 lib/include/json.h CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h DeviceMessageDef.h PanelType.h BayType.h main.h \
 lib/include/mongoose.h FuseBreakerType.h WebConnection.h \
 PanelConnection.h Rectifier.h MemoryManager.h FileUtils.h Devices.h \
 CANInterface.h
BayType.o: BayType.c BayType.h lib/include/json.h String.h \
 MemoryManager.h JSONIF.h ascii.h
BytesManage.o: BytesManage.c
CANDefsToJSON.o: CANDefsToJSON.c String.h FileUtils.h MemoryManager.h \
 AllCanDefinitions.h CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h lib/include/json.h DeviceMessageDef.h
CANInterface.o: CANInterface.c CANInterface.h String.h MemoryManager.h \
 CLIThread.h ThreadSafePrint.h BytesManage.h Devices.h CanMsg.h \
 DeviceDef.h DeviceDefines.h DeviceRegDef.h NumericTypes.h JSONIF.h \
 lib/include/json.h DeviceMessageDef.h main.h lib/include/mongoose.h \
 lib/include/sqlite3.h FuseBreakerType.h PanelType.h BayType.h \
 WebConnection.h
CanMsg.o: CanMsg.c CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h lib/include/json.h String.h DeviceMessageDef.h \
 ThreadSafePrint.h
CANRegisterDef.o: CANRegisterDef.c CANRegisterDef.h String.h \
 NumericTypes.h
CANSimMain.o: CANSimMain.c CanMsg.h DeviceDef.h DeviceDefines.h \
 DeviceRegDef.h NumericTypes.h JSONIF.h lib/include/json.h String.h \
 DeviceMessageDef.h Devices.h CANInterface.h ThreadSafePrint.h \
 UserInputHandling.h DirManagement.h
DefFileToken.o: DefFileToken.c DefFileToken.h
DeviceDef.o: DeviceDef.c DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h lib/include/json.h String.h DeviceMessageDef.h \
 DirManagement.h MemoryManager.h DefFileToken.h ThreadSafePrint.h
DeviceMessageDef.o: DeviceMessageDef.c DeviceMessageDef.h DeviceDefines.h \
 DeviceRegDef.h NumericTypes.h JSONIF.h lib/include/json.h String.h \
 MemoryManager.h DefFileToken.h ThreadSafePrint.h DeviceDef.h
DeviceRegDef.o: DeviceRegDef.c DeviceRegDef.h DeviceDefines.h \
 NumericTypes.h JSONIF.h lib/include/json.h String.h DefFileToken.h \
 MemoryManager.h ThreadSafePrint.h
Devices.o: Devices.c CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h lib/include/json.h String.h DeviceMessageDef.h \
 AllCanDefinitions.h CANInterface.h ThreadSafePrint.h UserInputHandling.h \
 Devices.h MemoryManager.h main.h lib/include/mongoose.h \
 lib/include/sqlite3.h FuseBreakerType.h PanelType.h BayType.h \
 WebConnection.h BytesManage.h
DirManagement.o: DirManagement.c DirManagement.h
FileUtils.o: FileUtils.c String.h MemoryManager.h
FuseBreaker.o: FuseBreaker.c FuseBreaker.h String.h lib/include/json.h \
 CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h NumericTypes.h \
 JSONIF.h DeviceMessageDef.h MemoryManager.h Panel.h \
 lib/include/sqlite3.h PanelType.h Bay.h BayType.h main.h \
 lib/include/mongoose.h FuseBreakerType.h WebConnection.h \
 PanelConnection.h Rectifier.h Devices.h CANInterface.h
FuseBreakerType.o: FuseBreakerType.c MemoryManager.h String.h ascii.h \
 FuseBreakerType.h lib/include/json.h JSONIF.h
HTTPServerThread.o: HTTPServerThread.c lib/include/mongoose.h \
 HTTPServerThread.h String.h main.h lib/include/sqlite3.h DeviceDef.h \
 DeviceDefines.h DeviceRegDef.h NumericTypes.h JSONIF.h \
 lib/include/json.h DeviceMessageDef.h FuseBreakerType.h PanelType.h \
 BayType.h WebConnection.h MemoryManager.h
jsoncanif.o: jsoncanif.c jsoncanif.h lib/include/json.h CanMsg.h \
 DeviceDef.h DeviceDefines.h DeviceRegDef.h NumericTypes.h JSONIF.h \
 String.h DeviceMessageDef.h AllCanDefinitions.h ThreadSafePrint.h \
 Devices.h CANInterface.h MemoryManager.h main.h lib/include/mongoose.h \
 lib/include/sqlite3.h FuseBreakerType.h PanelType.h BayType.h \
 WebConnection.h
JSONIF.o: JSONIF.c lib/include/json.h String.h MemoryManager.h JSONIF.h
linenoise.o: linenoise.c linenoise.h
main.o: main.c lib/include/mongoose.h lib/include/sqlite3.h jsoncanif.h \
 lib/include/json.h CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h String.h DeviceMessageDef.h AllCanDefinitions.h \
 Devices.h CANInterface.h ThreadSafePrint.h UserInputHandling.h \
 DirManagement.h HTTPServerThread.h WebSocketIF.h FuseBreaker.h Bay.h \
 Panel.h PanelType.h BayType.h main.h FuseBreakerType.h WebConnection.h \
 PanelConnection.h Rectifier.h FileUtils.h ascii.h ANSIColors.h \
 MemoryManager.h
MemoryManager.o: MemoryManager.c MemoryManager.h ThreadSafePrint.h
NumericTypes.o: NumericTypes.c
Panel.o: Panel.c Panel.h lib/include/sqlite3.h FuseBreaker.h String.h \
 lib/include/json.h CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h JSONIF.h DeviceMessageDef.h PanelType.h MemoryManager.h \
 Bay.h BayType.h main.h lib/include/mongoose.h FuseBreakerType.h \
 WebConnection.h PanelConnection.h Rectifier.h Devices.h CANInterface.h \
 SQLStatements.h
PanelConnection.o: PanelConnection.c PanelConnection.h lib/include/json.h \
 String.h MemoryManager.h JSONIF.h Bay.h Panel.h lib/include/sqlite3.h \
 FuseBreaker.h CanMsg.h DeviceDef.h DeviceDefines.h DeviceRegDef.h \
 NumericTypes.h DeviceMessageDef.h PanelType.h BayType.h main.h \
 lib/include/mongoose.h FuseBreakerType.h WebConnection.h Rectifier.h
PanelType.o: PanelType.c MemoryManager.h ascii.h PanelType.h \
 lib/include/json.h String.h JSONIF.h
Rectifier.o: Rectifier.c Rectifier.h CanMsg.h DeviceDef.h DeviceDefines.h \
 DeviceRegDef.h NumericTypes.h JSONIF.h lib/include/json.h String.h \
 DeviceMessageDef.h Devices.h CANInterface.h Bay.h Panel.h \
 lib/include/sqlite3.h FuseBreaker.h PanelType.h BayType.h main.h \
 lib/include/mongoose.h FuseBreakerType.h WebConnection.h \
 PanelConnection.h MemoryManager.h
smdup.o: smdup.c
SQLStatements.o: SQLStatements.c String.h SQLStatements.h
String.o: String.c String.h MemoryManager.h ascii.h
StringUtils.o: StringUtils.c StringUtils.h
ThreadSafePrint.o: ThreadSafePrint.c
UserInputHandling.o: UserInputHandling.c CanMsg.h DeviceDef.h \
 DeviceDefines.h DeviceRegDef.h NumericTypes.h JSONIF.h \
 lib/include/json.h String.h DeviceMessageDef.h CANInterface.h \
 ThreadSafePrint.h Devices.h MemoryManager.h UserInputHandling.h main.h \
 lib/include/mongoose.h lib/include/sqlite3.h FuseBreakerType.h \
 PanelType.h BayType.h WebConnection.h Bay.h Panel.h FuseBreaker.h \
 PanelConnection.h Rectifier.h WebSocketIF.h linenoise.h ANSIColors.h
WebConnection.o: WebConnection.c lib/include/mongoose.h WebConnection.h \
 MemoryManager.h ThreadSafePrint.h String.h
WebSocketIF.o: WebSocketIF.c WebSocketIF.h lib/include/mongoose.h \
 FuseBreaker.h String.h lib/include/json.h CanMsg.h DeviceDef.h \
 DeviceDefines.h DeviceRegDef.h NumericTypes.h JSONIF.h \
 DeviceMessageDef.h Bay.h Panel.h lib/include/sqlite3.h PanelType.h \
 BayType.h main.h FuseBreakerType.h WebConnection.h PanelConnection.h \
 Rectifier.h MemoryManager.h FileUtils.h Devices.h CANInterface.h \
 ThreadSafePrint.h WebSocketIFCreateInfoScript.c
WebSocketIFCreateInfoScript.o: WebSocketIFCreateInfoScript.c
