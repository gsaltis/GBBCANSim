/*****************************************************************************!
 * Function : WebSocketHandlerInputPacket
 *****************************************************************************/
function
WebSocketIFHandleInputPacket(InData)
{
  var					requestpacket;

  console.log(InData);
  requestpacket = JSON.parse(InData);

  if ( requestpacket.packettype == "response" ) {
    console.log(requestpacket);
    WebSocketIFHandleResponsePacket(requestpacket);
  }
}

