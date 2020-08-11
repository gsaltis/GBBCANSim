/*****************************************************************************!
 * Function : CBRegisterGroupChange
 *****************************************************************************/
function
CBRegisterGroupChange
(InEvent)
{
  var					element, groupname;
  var					i, deviceDef, register, regname;

  element = InEvent.srcElement;
  groupname = element.parentElement.dataGroup;

  value = element.parentElement.children[2].value;
  MainDisplayMessage(value);
  element.parentElement.children[1].style.visibility = "hidden";
  element.parentElement.children[2].style.visibility = "hidden";

  deviceDef = document.getElementById("RegisterArea").dataDeviceDef

  for (i = 0; i < deviceDef.registers.length; i++ ) {
    register = deviceDef.registers[i];
    if ( register.group == groupname ) {
      regname = "RegInput " + register.valuetype;
      document.getElementById(regname).value = value;
    } 
  }  
}

