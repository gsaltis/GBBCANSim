/*****************************************************************************!
 * Function : WebSocketIFHandlePanelRegValues
 *****************************************************************************/
function 
WebSocketIFHandlePanelRegValues
(InPacket)
{
  var					panel;
  var					i;

  bay = BayFindByIndex(InPacket.bayindex);

  if ( bay == null ) {
    return;
  }
  panel = BayFindPanelByIndex(bay, InPacket.panelindex);
  if ( null == panel ) {
    return;
  }
  panel.registers = InPacket.registers;
  for (i = 0; i < panel.registers.length; i++) {
    input = document.getElementById("RegInput " + panel.registers[i].valuetype);
    if ( input ) {
      input.value = panel.registers[i].value;
      input.dataOriginalValue = panel.registers[i].value;
    }
  }
}


