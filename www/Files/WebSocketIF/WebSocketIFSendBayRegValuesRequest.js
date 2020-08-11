/*****************************************************************************!
 * Function : WebSocketIFSendBayRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendBayRegValuesRequest(InBayIndex)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBayIndex;
  request.type = "getbayregvalues";
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}


