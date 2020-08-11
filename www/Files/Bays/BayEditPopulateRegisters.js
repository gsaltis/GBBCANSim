/*****************************************************************************!
 * Function : BayEditPopulateRegisters
 *****************************************************************************/
function
BayEditPopulateRegisters
(InBayRegisterArea, InDeviceDef)
{
  var					iseven;
  var					lineY;
  var					lastgroupname;
  var					i, line, label, input;
  var					s, lineHeight;

  s = getComputedStyle(document.body).getPropertyValue("--RegisterLineHeight");
  lineHeight = parseInt(s, 10);
  iseven = true;
  lineY = 0;
  DeviceDefSortRegsByGroupName(InDeviceDef);
  lastgroupname = "";
  for (i = 0; i < InDeviceDef.registers.length; i++) {
    line = document.createElement("div");
    if ( lastgroupname != InDeviceDef.registers[i].group ) {
      line.className = "RegisterLine RegisterHeading";
      line.id = "RegisterLine " + InDeviceDef.registers[i].valuetype;
      line.innerHTML = InDeviceDef.registers[i].group;
      lastgroupname = InDeviceDef.registers[i].group;
      line.style.top = lineY;
      line.style.left = 0;

      button = document.createElement("div");
      button.className = "RegisterGroupCollapseButton RegisterGroupClosed";
      button.onclick = function(event) { CBRegisterGroupButton(event); };
      line.appendChild(button);

      setButton = document.createElement("div");
      setButton.className = "RegisterHeaderSetButton";
      setButton.onclick = function(event) { CBRegisterGroupChange(event); };
      setButton.innerHTML = "Set";
      line.appendChild(setButton);

      groupInput = document.createElement("input");
      groupInput.className = "RegisterLineInput RegisterHeaderInput";
      line.appendChild(groupInput);

      line.dataGroup = lastgroupname;
      line.dataStatus = "Closed";
      InBayRegisterArea.appendChild(line);
      line = document.createElement("div");
      lineY += lineHeight;
      iseven = true;
    }  
    if ( i == 0 ) {
      line.className = "RegisterLine RegisterLineEven RegisterLineFirst";
    } else {
      if ( iseven ) {
        line.className = "RegisterLine RegisterLineEven";
 	iseven = false;
      } else {
        line.className = "RegisterLine RegisterLineOdd";
        iseven = true;
      }
    } 
    line.dataGroup = InDeviceDef.registers[i].group;
    line.style.top = lineY;
    line.style.left = 0;
    line.dataRegisterDef = InDeviceDef.registers[i];
    InBayRegisterArea.appendChild(line);

    label = document.createElement("p");
    label.className = "RegisterLineLabel";
    label.innerHTML = InDeviceDef.registers[i].displaylabel + " :";
    line.appendChild(label);

    label = document.createElement("p");
    label.className = "RegisterLineValueTypeLabel";
    label.innerHTML = "0x" + InDeviceDef.registers[i].valuetype.toString(16).toString(16).toUpperCase();
    line.appendChild(label);

    input = document.createElement("input");
    input.className = "RegisterLineInput";
    input.dataChanged = false;
    input.dataValueType = InDeviceDef.registers[i].valuetype;
    input.dataValueRegister = InDeviceDef.registers[i];
    input.dataOriginalValue = 0;
    input.id = "RegInput " + InDeviceDef.registers[i].valuetype; 
    line.appendChild(input);

    lineY += lineHeight;
  }
  BayRepositionRegisterLines();
}

