/*****************************************************************************!
 * Function : WebSocketIFSendRemovePanelRequest
 *****************************************************************************/
function
WebSocketIFSendRemovePanelRequest
(InBayIndex, InPanelIndex)
{
  var					request;
  request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.type = "removepanel";
  request.body = {};
  request.body.panelindex = InPanelIndex;
  request.body.bayindex = InBayIndex;

  WebSocketIFSendGeneralRequest(request); 

}

