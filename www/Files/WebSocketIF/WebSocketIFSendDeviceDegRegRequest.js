/*****************************************************************************!
 * Function : WebSocketIFSendDeviceDefRegRequest 
 *****************************************************************************/
function
WebSocketIFSendDeviceDefRegRequest
()
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.devicename = MainDeviceDefs[WebSocketIFDeviceIndex].name;
  request.type = "getdeviceregs";
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}


