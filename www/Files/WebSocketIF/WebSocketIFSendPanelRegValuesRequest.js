/*****************************************************************************!
 * Function : WebSocketIFSendPanelRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendPanelRegValuesRequest(InBayIndex, InPanelIndex)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.panelindex = InPanelIndex;
  request.bayindex = InBayIndex;
  request.type = "getpanelregvalues";
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}


