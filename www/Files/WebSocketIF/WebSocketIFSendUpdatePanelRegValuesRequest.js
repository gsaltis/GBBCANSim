/*****************************************************************************!
 * Function : WebSocketIFSendUpdatePanelRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendUpdatePanelRegValuesRequest
(InBayIndex, InPanelIndex, InBody)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBayIndex;
  request.panelindex = InPanelIndex
  request.type = "setpanelregvalues";
  request.body = InBody;

  WebSocketIFSendGeneralRequest(request);
}
