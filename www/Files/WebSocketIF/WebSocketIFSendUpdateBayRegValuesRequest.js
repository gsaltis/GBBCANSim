/*****************************************************************************!
 * Function : WebSocketIFSendUpdateBayRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendUpdateBayRegValuesRequest
(InBayIndex, InBody)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBayIndex;
  request.type = "setbayregvalues";
  request.body = InBody;

  WebSocketIFSendGeneralRequest(request);
}
