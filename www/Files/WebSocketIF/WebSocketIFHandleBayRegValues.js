/*****************************************************************************!
 * Function : WebSocketIFHandleBayRegValues
 *****************************************************************************/
function 
WebSocketIFHandleBayRegValues
(InPacket)
{
  var					bay;
  var					i;

  bay = BayFindByIndex(InPacket.bayindex);
  if ( null == bay ) {
    return;
  }
  bay.registers = InPacket.registers;
  for (i = 0; i < bay.registers.length; i++) {
    input = document.getElementById("RegInput " + bay.registers[i].valuetype);
    if ( input ) {
      input.value = bay.registers[i].value;
    }
  }
}


