/*****************************************************************************!
 * Function : WebSocketIFSendAddPanelRequest
 *****************************************************************************/
function
WebSocketIFSendAddPanelRequest
(InBayIndex, InPanelIndex, InListNumber)
{
  var						request;

  request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.panelindex = InPanelIndex;
  request.bayindex = InBayIndex;
  request.type = "addpanel";
  request.body = {};
  request.body.listnumber = InListNumber
  request.body.panelindex = InPanelIndex;
  request.body.bayindex = InBayIndex;

  WebSocketIFSendGeneralRequest(request);
}
