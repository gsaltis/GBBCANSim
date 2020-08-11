/*****************************************************************************!
 * Function : BayEditClose
 *****************************************************************************/
function
BayEditClose
(InBay)
{
  var					registerArea;
  var					panel, i, registerInput;
  var					request = {};

  registerArea = document.getElementById("RegisterArea");
  bay = InBay.dataBay;

  request.bayindex = bay.index;
  request.registers = [];

  for (i = 0; i < bay.device.registers.length; i++) {
    registerInput = document.getElementById("RegInput " + bay.device.registers[i].valuetype);
    if ( registerInput.dataOriginalValue != registerInput.value ) {
      n = new Object();
      n.valuetype = bay.device.registers[i].valuetype;
      n.value = registerInput.value;
      request.registers.push(n);
    }
  }
  WebSocketIFSendUpdateBayRegValuesRequest(bay.index, request);
  BayEditAreaHide(InBay);
} 
