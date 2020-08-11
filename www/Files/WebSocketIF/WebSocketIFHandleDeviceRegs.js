/*****************************************************************************!
 * Function : WebSocketIFHandleDeviceRegs
 *****************************************************************************/
function
WebSocketIFHandleDeviceRegs
(InPacket)
{
  var					device;

  device = DeviceDefFindByName(InPacket.devicename);
  if ( device ) {
    device.registers = InPacket.registers;
  }
  WebSocketIFSendDeviceDefRegRequestNext();
}
 
