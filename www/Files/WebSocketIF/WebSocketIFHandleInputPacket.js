/*****************************************************************************!
 * Function : WebSocketHandlerInputPacket
 *****************************************************************************/
function
WebSocketIFHandleInputPacket(InData)
{
  var					requestpacket;

  requestpacket = JSON.parse(InData);
  console.log(requestpacket);
  if ( requestpacket.packettype == "response" ) {
    WebSocketIFHandleResponsePacket(requestpacket);
  }
}

