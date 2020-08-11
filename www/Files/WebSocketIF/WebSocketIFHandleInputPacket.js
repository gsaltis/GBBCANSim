/*****************************************************************************!
 * Function : WebSocketHandlerInputPacket
 *****************************************************************************/
function
WebSocketIFHandleInputPacket(InData)
{
  var					requestpacket;

  requestpacket = JSON.parse(InData);

  if ( requestpacket.packettype == "response" ) {
    console.log(requestpacket);
    WebSocketIFHandleResponsePacket(requestpacket);
  }
}

