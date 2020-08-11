/*****************************************************************************!
 * Function : WebSocketIFSendDeviceDefRegRequestNext
 *****************************************************************************/
function
WebSocketIFSendDeviceDefRegRequestNext
()
{
  WebSocketIFDeviceIndex++;
  if ( WebSocketIFDeviceIndex >= MainDeviceDefs.length ) {
    WebSocketIFSendSimpleRequest("getbays");
  } else {
    WebSocketIFSendDeviceDefRegRequest();
  }
}


