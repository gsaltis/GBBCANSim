/*****************************************************************************!
 * Function : PanelEditClose 
 *****************************************************************************/
function PanelEditClose(InBay)
{
  var					registerArea;
  var					panel, i, registerInput;
  var					request = {};

  registerArea = document.getElementById("RegisterArea");
  registerArea.dataPanel.style.border = "solid 1px black";
  panel = registerArea.dataPanel.dataPanel.panel;

  request.panelindex = panel.panelindex;
  request.bayindex = panel.bayindex;
  request.registers = [];

  if ( panel.registers ) {
    for (i = 0; i < panel.registers.length; i++) {
      registerInput = document.getElementById("RegInput " + panel.registers[i].valuetype);
      if ( registerInput.dataOriginalValue != registerInput.value ) {
        n = new Object();
        n.valuetype = panel.registers[i].valuetype;
        n.value = registerInput.value;
        request.registers.push(n);
      }
    }
    WebSocketIFSendUpdatePanelRegValuesRequest(panel.bayindex, panel.panelindex, request);
  }
  BayEditAreaHide(InBay);
}

