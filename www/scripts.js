// FILE: ./Files/Main/MainDisplayBlocker.js
/*****************************************************************************!
 * Function : MainDisplayBlocker
 *****************************************************************************/
function
MainDisplayBlocker()
{
  document.getElementById("MainBlocker").style.visibility = "visible";
}

// FILE: ./Files/Main/MainInitializeDisplay.js
/*****************************************************************************!
 * Function : MainInitializeDisplay
 *****************************************************************************/
function
MainInitializeDisplay()
{
  var					name;
  name = null;

  if ( MainBays.length > 0 ) {
    name = MainBays[0].type;
  }
  
  BayTypesListPopulate(name);
}

// FILE: ./Files/Main/MainDisplayMessage.js
/*****************************************************************************!
 * Function : MainDisplayMessage
 *****************************************************************************/
function
MainDisplayMessage(InMessage)
{
  MainDisplayMessageColor(InMessage, "#000000"); 
}

// FILE: ./Files/Main/MainInitialize.js
/*****************************************************************************!
 * Function : MainInitialize
 *****************************************************************************/
function
MainInitialize()
{
  WebSocketIFInitialize();
}

// FILE: ./Files/Main/Main.js
/*****************************************************************************
 * FILE NAME    : script.js
 * DATE         : June 11 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var MAIN_DEFAULT_MESSAGE_TIME		= 10;

var MainBayTypes 			= null;
var MainPanelTypes			= null;
var MainFuseBreakerTypes		= null;
var MainBays				= [];
var MainDeviceDefs			= null;
var MainMessageTimerID			= null;

// FILE: ./Files/Main/MainDisplayTimedMessage.js
/*****************************************************************************!
 * Function : MainDisplayTimedMessage
 *****************************************************************************/
function
MainDisplayTimedMessage(InMessage, InTimeout)
{
  var					messageArea;

  MainMessageTimerID = setInterval(MainMessageTimeout, InTimeout * 1000);
  messageArea = document.getElementById("MessageLine");
  messageArea.innerHTML = InMessage;
}

// FILE: ./Files/Main/MainHideBlocker.js
/*****************************************************************************!
 * Function : MainHideBlocker
 *****************************************************************************/
function
MainHideBlocker()
{
  document.getElementById("MainBlocker").style.visibility = "hidden";
}

// FILE: ./Files/Main/MainDisplayMessageColor.js
/*****************************************************************************!
 * Function : MainDisplayMessageColor
 *****************************************************************************/
function
MainDisplayMessageColor
(InMessage, InColor)
{
  var					messageArewa;
  
  messageArea = document.getElementById("MessageLine");
  messageArea.style.color = InColor;
  MainDisplayTimedMessage(InMessage, MAIN_DEFAULT_MESSAGE_TIME);
}
// FILE: ./Files/Main/MainResizeBody.js
/*****************************************************************************!
 * Function : MainResizeBody
 *****************************************************************************/
function
MainResizeBody
(InEvent)
{
  var					mainArea, clientWidth, clientHeight;
  var					i, bay, locationInfo;

  mainArea = document.getElementById("BayDisplayArea");

  clientWidth = mainArea.clientWidth;
  clientHeight = mainArea.clientHeight;

  console.log(clientWidth, clientHeight);

  for (i = 0; i < mainArea.children.length; i++) {
    bay = mainArea.children[i];
    BayResize(bay);
    locationInfo = GetPanelPositionsSizes(bay);
    for ( j = 0; j < bay.children.length; j++ ) {
      panel = bay.children[j];
      if ( panel.dataType == "EmptyPanel" || panel.dataType == "Panel" ) {
        panel.style.width = locationInfo.panelwidth;
        panel.style.height = locationInfo.panelheight;
        panel.style.top = locationInfo.panelYs[panel.dataIndex];
        n = locationInfo.panelXs[(panel.dataIndex - 1) % 2];
        panel.style.left = n;
      }
    }
  }
}
// FILE: ./Files/Main/MainMessageTimeout.js
/*****************************************************************************!
 * Function : MainMessageTimeout
 *****************************************************************************/
function
MainMessageTimeout()
{
  clearInterval(MainMessageTimerID)
  document.getElementById("MessageLine").innerHTML = "";
  MainMessageTimerID = null;
}

// FILE: ./Files/PanelTypes/FindPanelTypeByListNumber.js
/*****************************************************************************!
 * Function : FindPanelTypeByListNumber
 *****************************************************************************/
function
FindPanelTypeByListNumber
(InPanelTypeName)
{
  var					i;
  for ( i = 0; i < MainPanelTypes.length; i++ ) {
    if ( MainPanelTypes[i].listnumber == InPanelTypeName ) {
      return MainPanelTypes[i];
    }
  }
  return null;
}
// FILE: ./Files/PanelTypes/PanelTypesGetByUsedIdListNumber.js
/*****************************************************************************!
 * Function : PanelTypesGetByUsedIdListNumber 
 *****************************************************************************/
function
PanelTypesGetByUsedIdListNumber(InListNumber)
{
  var					returnTypes = [];
  var					i;

  for (i = 0; i < MainPanelTypes.length; i++) {
    if ( InListNumber == MainPanelTypes[i].usedin ) {
      returnTypes.push(MainPanelTypes[i]);
    }
  }
  return returnTypes;
}

// FILE: ./Files/PanelTypes/PanelTypes.js
/*****************************************************************************
 * FILE NAME    : PanelTypes.js
 * DATE         : June 15 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

// FILE: ./Files/DeviceDefs/DeviceDefFindByName.js
/*****************************************************************************!
 * Function : DeviceDefFindByName 
 *****************************************************************************/
function
DeviceDefFindByName(InDeviceDefName)
{
  var i;
  for (i = 0; i < MainDeviceDefs.length; i++) {
    if ( MainDeviceDefs[i].name == InDeviceDefName ) {
      return MainDeviceDefs[i];
    }
  }
  return null;
}

// FILE: ./Files/DeviceDefs/DeviceDefs.js
/*****************************************************************************
 * FILE NAME    : DeviceDefs.js
 * DATE         : June 16 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

// FILE: ./Files/DeviceDefs/DeviceDefSortRegsByGroupName.js
/*****************************************************************************!
 * Function : DeviceDefSortRegsByGroupName
 *****************************************************************************/
function
DeviceDefSortRegsByGroupName
(InDevice)
{
   InDevice.registers.sort(function(a, b) { var n = a.groupsort.localeCompare(b.groupsort); 
					    if ( n == 0 ) {
					      n = a.displaylabel.localeCompare(b.displaylabel);
 					    }
					    return n;
					  });
}

// FILE: ./Files/DeviceDefs/DeviceDefFindByDescription.js
/*****************************************************************************!
 * Function : DeviceDefFindByDescription 
 *****************************************************************************/
function
DeviceDefFindByDescription(InDeviceDefDescription)
{
  var i;
  for (i = 0; i < MainDeviceDefs.length; i++) {
    if ( MainDeviceDefs[i].description == InDeviceDefDescription ) {
      return MainDeviceDefs[i];
    }
  }
  return null;
}

// FILE: ./Files/Bays/BayResize.js
/*****************************************************************************!
 * Function : BayResize
 *****************************************************************************/
function
BayResize
(InBay)
{
  var					registerAreaName, registerArea;
  var					registerAreaWidth, registerAreaHeight;

  dimensions = GetNewBayDimensions();
  MainDisplayMessage(dimensions.baywidth + " : " + dimensions.bayheight);
  registerAreaWidth  = 0;
  registerAreaHeight = 0;
  
  registerAreaName = "RegisterArea";
  registerArea = document.getElementById(registerAreaName);
  if ( registerArea ) {
    registerAreaWidth = registerArea.clientWidth;
    registerAreaHeight = registerArea.clientHeight;
  }
  InBay.style.width = dimensions.baywidth;
  InBay.style.height = dimensions.bayheight;
}
// FILE: ./Files/Bays/BayLowerZIndex.js
/*****************************************************************************!
 * Function : BayLowerZIndex
 *****************************************************************************/
function
BayLowerZIndex
(InBay)
{
  InBay.style.zIndex = 30;
}


// FILE: ./Files/Bays/BayRepositionRegisterLines.js
/*****************************************************************************!
 * Function : BayRepositionRegisterLines
 *****************************************************************************/
function
BayRepositionRegisterLines
()
{
  var					registerArea;
  var					y, i, register;
  var					groupstatus, currentgroup;

  registerArea = document.getElementById("RegisterArea");
  y = 0;
  for ( i = 0 ; i < registerArea.children.length; i++ ) {
    registerline = registerArea.children[i];
    registerline.style.top = y + "px";
    if ( registerline.className == "RegisterLine RegisterHeading" ) {
      y += 19;
      groupstatus = registerline.dataStatus;
      currentgroup = registerline.dataGroup;
    } else {
      if ( registerline.dataGroup == currentgroup ) {
        if ( groupstatus == "Open" ) {
          registerline.style.top = y + "px";
	  registerline.style.visibility = "visible";
          y += 19;
        } else {
	  registerline.style.visibility = "hidden";
        }
      } else {
        y += 19;
        registerline.style.visibility = "visible";
      }
    }
  }
}

// FILE: ./Files/Bays/Bays.js
/*****************************************************************************
 * FILE NAME    : Bays.js
 * DATE         : June 16 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
"use strict";

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var BayDisplayXSkip			= 20;
var BayDisplayMaxBaysCount		= 8;
var BayDisplayPanelXSkip		= 10;
var BayDisplayPanelYSkip		= 10;
var BayPanelTop				= 40;
var BayPanelBottom			= 20;


// FILE: ./Files/Bays/BayRaiseZIndex.js
/*****************************************************************************!
 * Function : BayRaiseZIndex
 *****************************************************************************/
function
BayRaiseZIndex
(InBay)
{
  InBay.style.zIndex = 30;
}


// FILE: ./Files/Bays/CreateBay.js
/*****************************************************************************!
 * Function : CreateNewBay 
 *****************************************************************************/
function
CreateBay(InBay)
{
  var					displayarea;
  var					bay;
  var					headerbutton;
  var					dimensions;
  var					header;

  console.log(InBay);
  baytype = InBay;
  displayarea = document.getElementById("BayDisplayArea");

  dimensions = GetNewBayDimensions();   
  
  bay = document.createElement("div");
  bay.className = "GeneralPane Bay";
  bay.id = "Bay " + InIndex;
  bay.dataBay = {};
  bay.dataBay.type = InBayType;
  bay.dataBay.index = InIndex;
  bay.dataBay.device = DeviceDefFindByDescription("Bay");
  bay.dataEdittingMode = "None";

  bay.ondragenter  = function(event) { CBBayDragEnter(event); }
  bay.ondragleave  = function(event) { CBBayDragLeave(event); }
  bay.style.width  = dimensions.baywidth;
  bay.style.height = dimensions.bayheight;
  bay.style.left   = dimensions.bayx;

  header = document.createElement("div");
  header.className = "GeneralPaneHeader";
  header.innerHTML = "Bay " + bay.dataBay.index;
  
  headerbutton = document.createElement("div");
  headerbutton.className = "GeneralPaneHeaderButton";
  headerbutton.onclick = function(event) { CBBayEdit(event); }
  header.appendChild(headerbutton);

  bay.appendChild(header);

  CreateEmptyPanels(bay);
  displayarea.appendChild(bay); 
  bay.addEventListener("transitionend", BayTransitionEnd, true);
  return bay;
}

// FILE: ./Files/Bays/BayFindLowestPanel.js
/*****************************************************************************!
 * Function : BayFindLowestPanel
 *****************************************************************************/
function
BayFindLowestPanel
(InBay)
{
  var					i, panel, panely, returnpanel, y;

  returnpanel = null;
  y = 0;
  for (i = 0; i < InBay.children.length; i++) {
    panel = InBay.children[i];
    if ( panel.dataType != "Panel" && panel.dataType != "EmptyPanel" ) {
      continue;
    }
    panely = parseInt(panel.style.top, 10);
    if ( panely > y ) {
      returnpanel = panel;
      y = panely;
    }
  }
  return returnpanel;
}


// FILE: ./Files/Bays/BayResizeLarger.js
/*****************************************************************************!
 * Function : BayResizeLarger.js
 *****************************************************************************/
function
BayResizeLarger(InBay)
{
  var					bayareawidth, left, baywidth;

  bayarea = document.getElementById("BayDisplayArea");
  bayareawidth = bayarea.clientWidth;
  
  
  InBay.dataBayWidth = InBay.style.width;
  left = parseInt(InBay.style.left, 10);
  
  baywidth = parseInt(InBay.style.width, 10) + 500;
  m = (left + baywidth) - 10;
  if ( m > bayareawidth ) {
    baywidth = bayareawidth - left;
  }
  InBay.style.width = baywidth + "px";
}

// FILE: ./Files/Bays/BaysFindPanelByIndex.js
/*****************************************************************************!
 * Function : BayFindPanelByIndex
 *****************************************************************************/
function
BayFindPanelByIndex
(InBay, InPanelIndex)
{
  var					i;
  if ( null == InBay ) {
    return null;
  }
  for (i = 0; i < InBay.panels.length; i++) {
    if ( InBay.panels[i].panelindex == InPanelIndex ) {
      return InBay.panels[i];
    }
  }
  return null;
}
// FILE: ./Files/Bays/GetNewBayDimensions.js
/*****************************************************************************!
 * Function : GetNewBayDimensions
 *****************************************************************************/
function
GetNewBayDimensions
()
{
  var					dimensions = {};
  var					displayarea, displayareaheight;
  var					baytop;
 
  baytop = parseInt(getComputedStyle(document.body).getPropertyValue("--BayTop"), 10);

  displayarea = document.getElementById("BayDisplayArea");
  displayareaheight = displayarea.clientHeight;

  dimensions.bayx = BayDisplayXSkip;
  dimensions.bayheight = displayareaheight - (baytop * 2);
  dimensions.baywidth = dimensions.bayheight / 84 * 30;
  return dimensions;
}

// FILE: ./Files/Bays/BayEditPopulateRegisters.js
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

// FILE: ./Files/Bays/BayTransitionEnd.js
/*****************************************************************************!
 * Function : BayTransitionEnd
 *****************************************************************************/
function
BayTransitionEnd
(InEvent)
{
  var					bay;
  bay = InEvent.srcElement;

  if ( bay.style.width == bay.dataBayWidth ) {
    // Only remove the children when the bay is closed
    bay.removeChild(bay.editregisterarea);
    bay.removeChild(bay.editinfoarea);
    bay.editregisterarea = null;
    bay.editinfoarea = null;
  }
}

// FILE: ./Files/Bays/CreateNewBay.js
/*****************************************************************************!
 * Function : CreateNewBay 
 *****************************************************************************/
function
CreateNewBay(InBayType, InIndex)
{
  var					displayarea;
  var					bay;
  var					displayareawidth, displayareaheight;
  var					baywidth, bayheight, bayareawidth;
  var					headerbutton;
  var					bayx;
  var					header, baytop;
  var					dimensions;

  dimensions = GetNewBayDimensions();

  displayarea = document.getElementById("BayDisplayArea");
   
  bay = document.createElement("div");
  bay.className = "GeneralPane Bay";
  bay.id = "Bay " + InIndex;

  bay.dataBay = {};
  bay.dataBay.type = InBayType;
  bay.dataBay.index = InIndex;
  bay.dataBay.device = DeviceDefFindByDescription("Bay");
  bay.dataEdittingMode = "None";

  bay.ondragenter  = function(event) { CBBayDragEnter(event); }
  bay.ondragleave  = function(event) { CBBayDragLeave(event); }

  bay.style.width  = dimensions.baywidth;
  bay.style.height = dimensions.bayheight;
  bay.style.left   = dimensions.bayx;

  header = document.createElement("div");
  header.className = "GeneralPaneHeader";
  header.innerHTML = "Bay " + bay.dataBay.index;
  
  headerbutton = document.createElement("div");
  headerbutton.className = "GeneralPaneHeaderButton";
  headerbutton.onclick = function(event) { CBBayEdit(event); }
  header.appendChild(headerbutton);

  bay.appendChild(header);

  CreateEmptyPanels(bay);
  displayarea.appendChild(bay); 
  bay.addEventListener("transitionend", BayTransitionEnd, true);
  return bay;
}


// FILE: ./Files/Bays/BayEditAreaHide.js
/*****************************************************************************!
 * Function : BayEditAreaHide
 *****************************************************************************/
function
BayEditAreaHide
(InBay)
{
  InBay.dataEdittingMode = "None";
  MainHideBlocker();
  BayLowerZIndex(InBay);
  InBay.style.width = InBay.dataBayWidth;
}

// FILE: ./Files/Bays/BayEdit.js
/*****************************************************************************!
 * Function : BayEdit 
 *****************************************************************************/
function
BayEdit(InBay)
{
  var					devicedef;
  var					baywidth, bay;
  var					lineX;
  var					lineY;
  var					lineHeight;
  var					s, y;
  var					line, label, bayregisterarea;
  var					i;
  var					panel, bottom, bayregisterareatop;
  var					data;
  var					dataLabels = ["Name", "Description", "Index"];
  var					dataProps  = ["name", "description"];
  var					dataValues = ["index"];
  var					bayinfoarea;
  var					iseven;
  var					lastgroupname, input;
  var					value;

  s = getComputedStyle(document.body).getPropertyValue("--RegisterLineHeight");
  lineHeight = parseInt(s, 10);

  devicedef = InBay.dataBay.device;
  lineX = parseInt(InBay.style.width, 10);
  if ( InBay.dataEdittingMode == "Bay" ) {
    BayEditClose(InBay);
    return;
  }
  if ( InBay.dataEdittingMode == "Panel" ) {
    PanelEditClose(InBay);
    return;
  }

  BaysNormalizeZIndex();
  BayRaiseZIndex(InBay);
  MainDisplayBlocker();
 
  InBay.dataEdittingMode = "Bay";
  BayResizeLarger(InBay);
  bayinfoarea = document.createElement("div");
  bayinfoarea.className = "GeneralPane BayInfoArea";
  bayinfoarea.style.left = lineX + "px";
  bayinfoarea.style.height = lineHeight * 3.5;
  bayinfoarea.style.top = BayPanelTop;
  bayinfoarea.style.visibility = "visible";

  y = 0
  for (i = 0; i < dataProps.length; i++ ) {
    label = document.createElement("p");
    label.className = "GeneralPaneLabel";
    label.style.top = (5 + (y * lineHeight)) + "px";
    label.innerHTML = dataLabels[y] + " :"
    label.style.width = "150px";
    bayinfoarea.appendChild(label);

    data = document.createElement("p");
    data.className = "GeneralPaneData";
    data.style.top = (7 + (y * lineHeight)) + "px";
    data.style.left = "160px";
    data.innerHTML = devicedef[dataProps[i]];
    bayinfoarea.appendChild(data);
    y++;
  }
  for (i = 0; i < dataValues.length; i++ ) {
    label = document.createElement("p");
    label.className = "GeneralPaneLabel";
    label.style.top = (5 + (y * lineHeight)) + "px";
    label.innerHTML = dataLabels[y] + " :"
    label.style.width = "150px";
    bayinfoarea.appendChild(label);

    data = document.createElement("p");
    data.className = "GeneralPaneData";
    data.style.top = (7 + (y * lineHeight)) + "px";
    data.style.left = "160px";
    data.innerHTML = InBay.dataBay[dataValues[i]];
    bayinfoarea.appendChild(data);
    y++;
  }


  bayregisterareatop = BayPanelTop;
  bayregisterareatop += lineHeight * 4;
  panel = BayFindLowestPanel(InBay);

  bayname = "Bay " + InBay.dataBay.index;
  bay = document.getElementById(bayname);
  bayHeight = bay.clientHeight;
  height = bayHeight - (BayPanelTop + parseInt(panel.style.height, 10)) + lineHeight * 5 - 8; 
  dimensions = GetPanelPositionsSizes(bay);
  bottom = 0;
  for (i = 0; i < dimensions.panelYs.length; i++) {
    if ( bottom < dimensions.panelYs[i] ) {
      bottom = dimensions.panelYs[i];
    }
  }
  bottom += dimensions.panelheight;
  height = bottom - bayregisterareatop;

  bayregisterarea = document.createElement("div");
  bayregisterarea.id = "RegisterArea";
  bayregisterarea.className = "GeneralPane BayRegisterArea";
  bayregisterarea.style.left = lineX + "px";
  bayregisterarea.style.height = height;
  bayregisterarea.dataDeviceDef = devicedef;
  bayregisterarea.style.top = bayregisterareatop;
  InBay.editregisterarea  = bayregisterarea;
  InBay.editinfoarea = bayinfoarea;  
  InBay.appendChild(bayregisterarea);
  InBay.appendChild(bayinfoarea);

  bayregisterarea.style.visibility = "visible";
  iseven = true;
  lineY = 0;
  BayEditPopulateRegisters(bayregisterarea, devicedef)
  DeviceDefSortRegsByGroupName(devicedef);
  WebSocketIFSendBayRegValuesRequest(InBay.dataBay.index);
}


// FILE: ./Files/Bays/BaysNormalizeZIndex.js
/*****************************************************************************!
 * Function : BaysNormalizeZIndex
 *****************************************************************************/
function
BaysNormalizeZIndex
()
{
  var					displayarea, i;
  displayarea = document.getElementById("BayDisplayArea");
  for (i = 0; i < displayarea.children.length; i++) {
    displayarea.children[i].style.zIndex = 20;
  }
}


// FILE: ./Files/Bays/BaysPopulateWindow.js
/*****************************************************************************!
 * Function : BaysPopulateWindow
 *****************************************************************************/
function
BaysPopulateWindow
()
{
  var 					j, i, baytype;
  var					panel, panelType, emptyPanelName, emptyPanel;
  var					parentElement;

  for ( i = 0 ; i < MainBays.length; i++) {
    baytype = BayTypeFindByName(MainBays[i].type);
    if ( null == baytype ) {
      continue;
    }
    CreateNewBay(baytype, MainBays[i].bayindex);
    for ( j = 0; j < MainBays[i].panels.length; j++ ) {
      panel = MainBays[i].panels[j];
      if ( panel.panelType == "Empty" ) {
        continue;
      }
      panelType = FindPanelTypeByListNumber(panel.listnumber);
      if ( panelType == null ) {
        continue;
      }
      emptyPanelName = "Empty Bay " + MainBays[i].bayindex + " Panel " + panel.panelindex;
      emptyPanel = document.getElementById(emptyPanelName);
      if ( null == emptyPanel ) {
        continue;
      }
      CreatePanelFromEmptyPanel(panelType, emptyPanel, panel);
      parentElement = emptyPanel.parentElement;
      parentElement.removeChild(emptyPanel);
    } 
  }
  if ( MainBays.length > 0 ) {
    BayTypesListPopulate(MainBays[0].type);
  }
}


// FILE: ./Files/Bays/GetPanelPositionsSizes.js
/*****************************************************************************!
 * Function : GetPanelPositionsSizes
 *****************************************************************************/
function
GetPanelPositionsSizes
(InBay)
{
  var				n, width, height, panelverticalarea, m; 
  var				returnInfo = {};
  var				i, k, h;
 
  n = InBay.dataBay.type.panelcount;
  width = parseInt(InBay.style.width, 10);
  height = parseInt(InBay.style.height, 10);

  returnInfo.panelwidth        = (width - (BayDisplayPanelXSkip * 3)) / 2;
  panelverticalarea = height - (BayPanelTop + BayPanelBottom);
  m                 = Math.floor(n / 2);
  returnInfo.panelheight       = ((panelverticalarea - ((m - 1) * BayDisplayPanelYSkip)) / m);
  returnInfo.panelXs = [ BayDisplayPanelXSkip, (BayDisplayPanelXSkip * 2) + returnInfo.panelwidth ];
  k = n / 2 - 1;

  returnInfo.panelYs = [];
  returnInfo.panelYs.push(0);
  for ( i = k; i >= 0; i-- ) {
    h = BayPanelTop + (returnInfo.panelheight + BayDisplayPanelYSkip) * i;
    returnInfo.panelYs.push(h);
    returnInfo.panelYs.push(h);
  }
  return returnInfo;
}

// FILE: ./Files/Bays/BayFindByIndex.js
/*****************************************************************************!
 * Function : BayFindByIndex
 *****************************************************************************/
function
BayFindByIndex
(InBayIndex)
{
  var					i;

  for ( i = 0; i < MainBays.length; i++ ) {
    if ( MainBays[i].bayindex == InBayIndex ) {
      return MainBays[i];
    }
  }
  return null;
}


// FILE: ./Files/Bays/BayEditClose.js
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
// FILE: ./Files/Bays/CreateEmptyPanels.js
/*****************************************************************************!
 * Function : CreateEmptyPanels
 *****************************************************************************/
function
CreateEmptyPanels (InBay)
{
  var					i, x, n;

  var locationInfo = GetPanelPositionsSizes(InBay);
  for ( i = InBay.dataBay.type.panelcount; i > 0; i-- ) {
    n = (i - 1) % 2;
    x = locationInfo.panelXs[n];
    CreateEmptyPanel(InBay, i, x, locationInfo.panelYs[i], locationInfo.panelwidth, locationInfo.panelheight);
  }
}

// FILE: ./Files/Bays/BayFindRegisterValueByValueType.js
/*****************************************************************************!
 * Function : BayFindRegisterValueByValueType
 *****************************************************************************/
function
BayFindRegisterValueByValueType
(InBay, InValueType)
{
  var					i;

  if ( null == InBay || null == InBay.device.registers ) {
    return null;
  }

  for (i = 0; i < InBay.device.registers.length; i++) {
    if ( InBay.device.registers[i].valuetype == InValueType ) {
      return InBay.device.registers[i];
    }
  }
  return null;
}

// FILE: ./Files/WebSocketIF/WebSocketIFSendUpdateBayRegValuesRequest.js
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
// FILE: ./Files/WebSocketIF/WebSocketIFHandleBays.js
/*****************************************************************************!
 * Function : WebSocketIFHandleBays
 *****************************************************************************/
function
WebSocketIFHandleBays(InBays)
{
  MainBays = InBays;
  BaysPopulateWindow();
}


// FILE: ./Files/WebSocketIF/WebSocketIFSendUpdatePanelRegValuesRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendUpdatePanelRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendUpdatePanelRegValuesRequest
(InBayIndex, InPanelIndex, InBody)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBayIndex;
  request.panelindex = InPanelIndex
  request.type = "setpanelregvalues";
  request.body = InBody;

  WebSocketIFSendGeneralRequest(request);
}
// FILE: ./Files/WebSocketIF/WebSocketIFSendSimpleRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendSimpleRequest
 *****************************************************************************/
function
WebSocketIFSendSimpleRequest(InRequest)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.type = InRequest;
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}

// FILE: ./Files/WebSocketIF/WebSocketIFSendAddBayRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendAddBayRequest
 *****************************************************************************/
function
WebSocketIFSendAddBayRequest
(InBay)
{
  var                                   request;

  request = {};
  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBay.dataBay.index;
  request.type = "addbay";
  request.body = {};
  request.body.bayindex = InBay.dataBay.index;
  request.body.bayinfo = {};
  request.body.bayinfo.panelcount = InBay.dataBay.type.panelcount;
  request.body.bayinfo.bayindex = InBay.dataBay.index;
  request.body.bayinfo.name = InBay.id;
  request.body.bayinfo.ipaddress = "127.0.0.1";
  request.body.bayinfo.type = InBay.dataBay.type.name;
  request.body.bayinfo.portnumber = 8001;
  request.body.bayinfo.canaddress = 0;
  request.body.bayinfo.listnumber = InBay.dataBay.type.listnumber;
  WebSocketIFSendGeneralRequest(request); 
}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleInputPacket.js
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

// FILE: ./Files/WebSocketIF/WebSocketIFSendBayRegValuesRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendBayRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendBayRegValuesRequest(InBayIndex)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBayIndex;
  request.type = "getbayregvalues";
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}


// FILE: ./Files/WebSocketIF/WebSocketIFSendDeviceDefRegRequestNext.js
/*****************************************************************************!
 * Function : WebSocketIFSendDeviceDefRegRequestNext
 *****************************************************************************/
function
WebSocketIFSendDeviceDefRegRequestNext
()
{
  WebSocketIFDeviceIndex++;
  if ( WebSocketIFDeviceIndex >= MainDeviceDefs.length ) {
    WebSocketIFSendSimpleRequest("getbays");
  } else {
    WebSocketIFSendDeviceDefRegRequest();
  }
}


// FILE: ./Files/WebSocketIF/WebSocketIFSendGeneralRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendGeneralRequest
 *****************************************************************************/
function 
WebSocketIFSendGeneralRequest(InRequest) {
  console.log(InRequest);
  if ( WebSocketIFConnection ) {
    WebSocketIFConnection.send(JSON.stringify(InRequest));
  }
}

// FILE: ./Files/WebSocketIF/WebSocketIFGetNextID.js
/*****************************************************************************!
 * Function : WebSocketIFGetNextID
 *****************************************************************************/
function 
WebSocketIFGetNextID()
{
  WebSocketIFNextID++;
  return WebSocketIFNextID;
}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleResponsePacket.js
/*****************************************************************************!
 * Function : WebSocketIFHandleResponsePacket
 *****************************************************************************/
function
WebSocketIFHandleResponsePacket(InPacket)
{
  if ( InPacket.responseid != "OK" ) {
    MainDisplayMessage(InPacket.responsemessage);
    return;
  }
  if ( InPacket.type == "resbaytypes" ) {
    WebSocketIFHandleResponseBayTypes(InPacket.body.baytypes);
    WebSocketIFSendSimpleRequest("getpaneltypes");
  } else if ( InPacket.type == "respaneltypes" ) {
    WebSocketIFHandleResponsePanelTypes(InPacket.body.paneltypes);
    WebSocketIFSendSimpleRequest("getdevicedefs");
  } else if ( InPacket.type == "resdevicedefs" ) {
    WebSocketIFHandleDeviceDefs(InPacket.body.devicedefs);
    WebSocketIFSendDeviceDefRegRequestStart();
  } else if ( InPacket.type == "resdeviceregs" ) {
    WebSocketIFHandleDeviceRegs(InPacket.body);
  } else if ( InPacket.type == "resbays" ) {
    WebSocketIFHandleBays(InPacket.body.bays);
    MainInitializeDisplay();
  } else if ( InPacket.type == "resbayregvalues") {
    WebSocketIFHandleBayRegValues(InPacket.body.bayregvalues);
  } else if ( InPacket.type == "respanelregvalues") {
    WebSocketIFHandlePanelRegValues(InPacket.body.panelregvalues);
  } else if ( InPacket.type == "resremovepanel" ) {
    WebSocketIFHandleRemovePanelResponse(InPacket.responseid, InPacket.responsemessage, InPacket.body.bayindex, InPacket.body.panelindex);
  }
}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleFuseBreakerTypes.js
/*****************************************************************************!
 * Function : WebSocketIFHandleFuseBreakerTypes
 *****************************************************************************/
function
WebSocketIFHandleFuseBreakerTypes(InFuseBreakerTypes)
{
  MainFuseBreakerTypes = InFuseBreakerTypes;
}

// FILE: ./Files/WebSocketIF/WebSocketIFSendPanelRegValuesRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendPanelRegValuesRequest
 *****************************************************************************/
function
WebSocketIFSendPanelRegValuesRequest(InBayIndex, InPanelIndex)
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.panelindex = InPanelIndex;
  request.bayindex = InBayIndex;
  request.type = "getpanelregvalues";
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}


// FILE: ./Files/WebSocketIF/WebSocketIFSendDeviceDefRequestStart.js
/*****************************************************************************!
 * Function : WebSocketIFSendDeviceDefRegRequestStart
 *****************************************************************************/
function
WebSocketIFSendDeviceDefRegRequestStart
()
{
  WebSocketIFDeviceIndex = 0;
  WebSocketIFSendDeviceDefRegRequest();
}


// FILE: ./Files/WebSocketIF/WebSocketIFSendDeviceDegRegRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendDeviceDefRegRequest 
 *****************************************************************************/
function
WebSocketIFSendDeviceDefRegRequest
()
{
  var                           request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.devicename = MainDeviceDefs[WebSocketIFDeviceIndex].name;
  request.type = "getdeviceregs";
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}


// FILE: ./Files/WebSocketIF/WebSocketIF.js
/*****************************************************************************
 * FILE NAME    : WebSocketIF.js
 * DATE         : June 11 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Local Data 
 *****************************************************************************/
var WebSocketIFConnection = null;
var WebSocketIFNextID = 1;
var WebSocketIFDeviceIndex = 0;

// FILE: ./Files/WebSocketIF/WebSocketIFSendAddPanelRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendAddPanelRequest
 *****************************************************************************/
function
WebSocketIFSendAddPanelRequest
(InBayIndex, InPanelIndex, InListNumber)
{
  var						request;

  request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.panelindex = InPanelIndex;
  request.bayindex = InBayIndex;
  request.type = "addpanel";
  request.body = {};
  request.body.listnumber = InListNumber
  request.body.panelindex = InPanelIndex;
  request.body.bayindex = InBayIndex;

  WebSocketIFSendGeneralRequest(request);
}
// FILE: ./Files/WebSocketIF/WebSocketIFSendRemovePanelRequest.js
/*****************************************************************************!
 * Function : WebSocketIFSendRemovePanelRequest
 *****************************************************************************/
function
WebSocketIFSendRemovePanelRequest
(InBayIndex, InPanelIndex)
{
  var					request;
  request = {};

  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.type = "removepanel";
  request.body = {};
  request.body.panelindex = InPanelIndex;
  request.body.bayindex = InBayIndex;

  WebSocketIFSendGeneralRequest(request); 

}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleBayRegValues.js
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


// FILE: ./Files/WebSocketIF/WebSocketIFRequestBayTypes.js
/*****************************************************************************!
 * Function : WebSocketIFRequestGetBayTypes
 *****************************************************************************/
function
WebSocketIFRequestGetBayTypes()
{
   WebSocketIFSendSimpleRequest("getbaytypes");
}


// FILE: ./Files/WebSocketIF/WebSocketIFHandleDeviceDefs.js
/*****************************************************************************!
 * Function : WebSocketIFHandleDeviceDefs
 *****************************************************************************/
function
WebSocketIFHandleDeviceDefs(InDeviceDefs)
{
  MainDeviceDefs = InDeviceDefs;
}


// FILE: ./Files/WebSocketIF/WebSocketIFHandlePanelRegValues.js
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


// FILE: ./Files/WebSocketIF/WebSocketIFInitialize.js
/*****************************************************************************!
 * Function : WebSocketIFInitialize
 *****************************************************************************/
function 
WebSocketIFInitialize()
{
  var hostAddress = "ws://" + WebSocketIFAddress + ":" + WebSocketIFPort;

  // ShowDeviceList();

  WebSocketIFConnection = new WebSocket(hostAddress);

  WebSocketIFConnection.onopen = function () {
    WebSocketIFRequestGetBayTypes();
    MainHideBlocker();
    MainDisplayMessage("Connected");
  };

  // Log errors
  WebSocketIFConnection.onerror = function (error) {
  };

  // 
  WebSocketIFConnection.onclose = function() {
    MainHideBlocker();
  }
  
  // Log messages from the server
  WebSocketIFConnection.onmessage = function (e) {
    WebSocketIFHandleInputPacket(e.data);
  };
}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleResponsePanelTypes.js
/*****************************************************************************!
 * Function : WebSocketIFHandleResponsePanelTypes
 *****************************************************************************/
function
WebSocketIFHandleResponsePanelTypes(InPanelTypes)
{
  MainPanelTypes = InPanelTypes;
}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleDeviceRegs.js
/*****************************************************************************!
 * Function : WebSocketIFHandleDeviceRegs
 *****************************************************************************/
function
WebSocketIFHandleDeviceRegs
(InPacket)
{
  var					device;

  device = DeviceDefFindByName(InPacket.devicename);
  if ( device ) {
    device.registers = InPacket.registers;
  }
  WebSocketIFSendDeviceDefRegRequestNext();
}
 
// FILE: ./Files/WebSocketIF/WebSocketIFHandleRemovePanelResponse.js
/*****************************************************************************!
 * Function : WebSocketIFHandleRemovePanelResponse
 *****************************************************************************/
function
WebSocketIFHandleRemovePanelResponse
(InID, InMessage, InBayIndex, InPanelIndex)
{
  RemovePanel(InBayIndex, InPanelIndex);
  MainDisplayMessageColor(InMessage, "#080");

  bay = document.getElementById("Bay " + InBayIndex);
  locationInfo = GetPanelPositionsSizes(bay);

  CreateEmptyPanel(bay, InPanelIndex, 
                   locationInfo.panelXs[(InPanelIndex - 1) % 2], locationInfo.panelYs[InPanelIndex],
                   locationInfo.panelwidth, locationInfo.panelheight); 
}

// FILE: ./Files/WebSocketIF/WebSocketIFHandleResponseBays.js
/*****************************************************************************!
 * Function : WebSocketIFHandleResponseBayTypes
 *****************************************************************************/
function
WebSocketIFHandleResponseBayTypes(InBayTypes)
{
  MainBayTypes = InBayTypes;
}

// FILE: ./Files/Panels/PanelEdit.js
/*****************************************************************************!
 * Function : PanelEdit
 *****************************************************************************/
function
PanelEdit(InPanel)
{
  var					bay;
  var					panelinfoarea, panelregisterarea;
  var					panel, bottom;
  var					lineHeight, lineX;
  var					s;
  var					infoLabels = [ "Name", "Type", "Description", "CAN Address" ];
  var					panelInfo = [];
  var					y, data;

  InPanel.style.border = "solid 2px red";
  s = getComputedStyle(document.body).getPropertyValue("--RegisterLineHeight");
  lineHeight = parseInt(s, 10);

  bay = InPanel.parentElement;
  if ( bay.dataEdittingMode == "Panel" ) {
    PanelEditClose(bay);
    return;
  }
  if ( bay.dataEdittingMode != "None" ) {
    return;
  }
  bay.dataEdittingMode = "Panel";

  lineX = parseInt(bay.style.width, 10);

  BaysNormalizeZIndex();
  BayRaiseZIndex(bay);
  MainDisplayBlocker();
 
  BayResizeLarger(bay);

  panelInfo  = [ "", "", "", "" ];

  panelInfo[0] = "Panel " + InPanel.dataPanel.index;
  if ( InPanel.dataPanel.devicedef ) {
    panelInfo[1] = InPanel.dataPanel.devicedef.name;
  } else {
    panelInfo[1] = "None";
  }
  panelInfo[2] = InPanel.dataPanel.type.name;
  if ( InPanel.dataPanel.panel ) {
    panelInfo[3] = InPanel.dataPanel.panel.canaddress;
  } else {
    panelInfo[3] = 0;
  }

  panelinfoarea = document.createElement("div");
  panelinfoarea.className = "GeneralPane BayInfoArea";
  panelinfoarea.style.left = lineX + "px";
  panelinfoarea.style.height = lineHeight * 4.5;
  panelinfoarea.style.top = BayPanelTop;
  panelinfoarea.style.visibility = "visible";

  y = 0
  for (i = 0; i < infoLabels.length; i++ ) {
    label = document.createElement("p");
    label.className = "GeneralPaneLabel";
    label.style.top = (5 + (y * lineHeight)) + "px";
    label.innerHTML = infoLabels[y] + " :"
    label.style.width = "150px";
    panelinfoarea.appendChild(label);

    data = document.createElement("p");
    data.className = "GeneralPaneData";
    data.style.top = (7 + (y * lineHeight)) + "px";
    data.style.left = "160px";
    data.innerHTML = panelInfo[i];
    panelinfoarea.appendChild(data);
    y++;
  }

  panelregisterareatop = BayPanelTop;
  panelregisterareatop += lineHeight * 5;
  panel = BayFindLowestPanel(bay);
  bayname = "Bay " + InPanel.dataPanel.panel.bayindex;
  
  bay = document.getElementById(bayname);
  bottom = 0;
  dimensions = GetPanelPositionsSizes(bay);
  for (i = 0; i < dimensions.panelYs.length; i++) {
    if ( bottom < dimensions.panelYs[i] ) {
      bottom = dimensions.panelYs[i];
    }
  }
  bottom += dimensions.panelheight;
  height = bottom - panelregisterareatop;

  panelregisterarea = document.createElement("div");
  panelregisterarea.id = "RegisterArea";
  panelregisterarea.className = "GeneralPane BayRegisterArea";
  panelregisterarea.style.left = lineX + "px";
  panelregisterarea.style.height  = height + "px";
  panelregisterarea.style.top = panelregisterareatop;
  panelregisterarea.style.visibility = "visible";
  panelregisterarea.dataPanel = InPanel;

  bay.editinfoarea = panelinfoarea;
  bay.editregisterarea = panelregisterarea; 
  bay.appendChild(panelinfoarea);
  bay.appendChild(panelregisterarea);

  if ( InPanel.dataPanel.type && InPanel.dataPanel.type.usesSMDUH2 ) {
    panelregisterarea.dataDeviceDef = InPanel.dataPanel.devicedef;
    BayEditPopulateRegisters(panelregisterarea, InPanel.dataPanel.devicedef);
    WebSocketIFSendPanelRegValuesRequest(InPanel.dataPanel.panel.bayindex, InPanel.dataPanel.panel.panelindex);
  }
}

// FILE: ./Files/Panels/RemovePanel.js
/*****************************************************************************!
 * Function : RemovePanel
 *****************************************************************************/
function
RemovePanel
(InBayIndex, InPanelIndex)
{
  var					id, panel, parent;

  id = "Panel Bay " + InBayIndex + " Panel " + InPanelIndex;

  panel = document.getElementById(id);
  if ( null == panel ) {
    return;
  }

  parent = panel.parentElement;

  parent.removeChild(panel);
}
// FILE: ./Files/Panels/CreateEmptyPanel.js
/*****************************************************************************!
 * Function : CreateEmptyPanel
 *****************************************************************************/
function
CreateEmptyPanel(InBay, InPanelIndex, InX, InY, InPanelWidth, InPanelHeight)
{
  var				panel;

  panel = document.createElement("div");
  panel.className = "GeneralPane EmptyPanel";

  panel.id = "Empty Bay " + InBay.dataBay.index + " Panel " + InPanelIndex;
  panel.style.width = InPanelWidth;
  panel.style.height = InPanelHeight;
  panel.style.top = InY;
  panel.style.left = InX;
  panel.dataIndex = InPanelIndex;
  panel.dataBay = InBay.dataBay;
  panel.dataType = "EmptyPanel";
  panel.ondrop = function(event) { CBPanelDrop(event); }
  panel.ondragover = function(event) { CBPanelDragOver(event); }

  InBay.appendChild(panel);
}

// FILE: ./Files/Panels/CreatePanelFromEmptyPanel.js
/*****************************************************************************!
 * Function : CreatePanelFromEmptyPanel
 *****************************************************************************/
function
CreatePanelFromEmptyPanel(InPanelType, InEmptyPanel, InPanel)
{
  var					panel;

  panel = document.createElement("div");
  if ( InPanelType.usesSMDUH2 ) {
    panel.className    = "GeneralPane Panel AdvancedPanel";
  } else {
    panel.className    = "GeneralPane Panel SimplePanel";
  }
  panel.draggable = true;
  panel.ondragstart = function(event) { CBGarbageCanOpen(event); };
  panel.ondragend   = function(event) { CBGarbageCanClose(event); };
  panel.dataPanel    = {};
  panel.style.width  = InEmptyPanel.style.width;
  panel.style.height = InEmptyPanel.style.height;
  panel.style.top    = InEmptyPanel.style.top;
  panel.style.left   = InEmptyPanel.style.left;
  panel.id = "Panel Bay " + InEmptyPanel.dataBay.index + " Panel " + InEmptyPanel.dataIndex;
  panel.dataPanel.bay   = InEmptyPanel.dataBay;
  panel.dataPanel.index = InEmptyPanel.dataIndex;
  panel.dataIndex = InEmptyPanel.dataIndex;
  panel.dataPanel.type  = InPanelType;
  panel.dataType = "Panel";
  panel.dataPanel.panel = InPanel;
  if ( InPanelType.usesSMDUH2 ) {
    panel.dataPanel.devicedef = DeviceDefFindByName("SMDUH2");
  }
  label = document.createElement("div");
  label.className = "GeneralPaneHeader PanelHeader";
  label.innerHTML = "Panel " + panel.dataPanel.index;
  panel.appendChild(label);
 
  InEmptyPanel.parentElement.appendChild(panel);
  panel.onclick = function(ev) { CBPanelEdit(ev); };
  return panel;
}

// FILE: ./Files/Panels/PanelEditClose.js
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

// FILE: ./Files/Panels/Panels.js
/*****************************************************************************
 * FILE NAME    : Panels.js
 * DATE         : June 16 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by 2020
 ****************************************************************************/

var PanelBeingDragged = null;
// FILE: ./Files/Callbacks/CBBayTypeDragStart.js
/*****************************************************************************!
 * Function : CBBayTypeDragStart 
 *****************************************************************************/
function
CBBayTypeDragStart(InEvent)
{
   BayTypeDragElement = InEvent.currentTarget.dataBayType;
   DragElementType = InEvent.currentTarget.dataType;
}

// FILE: ./Files/Callbacks/CBBayDragEnter.js
/*****************************************************************************!
 * Function : CBBayDragEnter 
 *****************************************************************************/
function
CBBayDragEnter(InEvent)
{
}

// FILE: ./Files/Callbacks/CBBayDragLeave.js
/*****************************************************************************!
 * Function : CBBayDragLeave 
 *****************************************************************************/
function
CBBayDragLeave(InEvent)
{
}

// FILE: ./Files/Callbacks/CBPanelEdit.js
/*****************************************************************************!
 * Function : CBPanelEdit
 *****************************************************************************/
function
CBPanelEdit(InEvent)
{
  var					panelElement;
  var					editMode;
  var					panelregisterarea;
  var					edittingpanel;

  panelregisterarea = document.getElementById("RegisterArea");

  if ( panelregisterarea ) {
    edittingpanel = panelregisterarea.dataPanel;
  } else {
    edittingpanel = null;
  }
  panelelement = InEvent.srcElement;
  editMode = panelelement.parentElement.dataEdittingMode;
  console.log(editMode);
  if ( editMode == "None" ) {
    PanelEdit(panelelement);
  } else if ( editMode == "Panel" ) {
    if ( edittingpanel == null || edittingpanel.dataPanel.index == panelelement.dataPanel.index ) {
      PanelEditClose(panelelement.parentElement);
    }
  }
}

// FILE: ./Files/Callbacks/CBBayTypeDragEnd.js
/*****************************************************************************!
 * Function : CBBayTypeDragEnd
 *****************************************************************************/
function
CBBayTypeDragEnd(InEvent)
{
  BayTypeDragElement = null;
  DragElementType = null;
}

// FILE: ./Files/Callbacks/CBGarbageCanDrop.js
/*****************************************************************************!
 * Function : CBGarbageCanDrop
 *****************************************************************************/
function
CBGarbageCanDrop
(InEvent)
{
  if ( PanelBeingDragged ) {
    console.log(PanelBeingDragged);
    WebSocketIFSendRemovePanelRequest(PanelBeingDragged.dataPanel.bay.index, PanelBeingDragged.dataPanel.index); 
  }
  PanelBeingDragged = null;
}

// FILE: ./Files/Callbacks/CBGarbageCanClose.js
/*****************************************************************************!
 * Function : CBGarbageCanClose
 *****************************************************************************/
function
CBGarbageCanClose
(InEvent)
{
  var					e;
  var					can;

  e = document.getElementById("GarbageCanLid");
  e.className = "GarbageCanLid";

  can = document.getElementById("GarbageCan");
  can.style.visibility = "hidden";
  PanelBeingDragged = null;
}

// FILE: ./Files/Callbacks/CBPanelDragOver.js
/*****************************************************************************!
 * Function : CBPanelDragOver
 *****************************************************************************/
function
CBPanelDragOver(InEvent)
{
  if ( DragElementType == "Panel" ) {
    InEvent.preventDefault();
  }
}

// FILE: ./Files/Callbacks/CBRegisterGroupButton.js
/*****************************************************************************!
 * Function : CBRegisterGroupButton
 *****************************************************************************/
function
CBRegisterGroupButton
(InEvent)
{
  var					element;

  element = InEvent.srcElement;

  if ( InEvent.ctrlKey == true ) {
    e = element.parentElement;
    e.children[1].style.visibility = "visible";
    e.children[2].style.visibility = "visible";
    // Gang change all element in the group
    return;
  }
  // Toggle the registers display
  if ( element.parentElement.dataStatus == "Open" ) {
    element.parentElement.dataStatus = "Closed";
    element.className = "RegisterGroupCollapseButton RegisterGroupClosed";
  } else {
    element.parentElement.dataStatus = "Open";
    element.className = "RegisterGroupCollapseButton RegisterGroupOpen";
  }
  BayRepositionRegisterLines();
} 
// FILE: ./Files/Callbacks/CBPanelDrop.js
/*****************************************************************************!
 * Function : CBPanelDrop
 *****************************************************************************/
function
CBPanelDrop(InEvent)
{
  var					emptypanel;
  var					paneltemplate = {};

 
  if ( DragElementType != "Panel" ) {
    return;
  }
  InEvent.preventDefault();
  emptypanel = InEvent.target;
  paneltemplate.panelindex = emptypanel.dataIndex;
  paneltemplate.bayindex = emptypanel.dataBay.index; 
  paneltemplate.listnumber = PanelTypeDragElement.listnumber;
  if ( PanelTypeDragElement.usesSMDUH2 ) {
    devicedef = DeviceDefFindByName("SMDUH2");
    paneltemplate.canaddress = devicedef.startcanaddress + emptypanel.dataIndex - 1;
  } else {
    paneltemplate.canaddress = 0;
  }
  panel = CreatePanelFromEmptyPanel(PanelTypeDragElement, emptypanel, paneltemplate);
  parentelement = emptypanel.parentElement;
  parentelement.removeChild(emptypanel);
  WebSocketIFSendAddPanelRequest(panel.dataPanel.bay.index, panel.dataPanel.index, panel.dataPanel.type.listnumber);
}

// FILE: ./Files/Callbacks/CBPanelTypeDragStart.js
/*****************************************************************************!
 * Function : CBPanelTypeDragStart 
 *****************************************************************************/
function
CBPanelTypeDragStart(InEvent)
{
  PanelTypeDragElement = InEvent.currentTarget.dataPanelType;
  DragElementType = InEvent.currentTarget.dataType;
}

// FILE: ./Files/Callbacks/CBBayDrop.js
/*****************************************************************************!
 * Function : CBBayDrop
 *****************************************************************************/
function
CBBayDrop(InEvent)
{
  var					bay;

  if ( DragElementType == "Bay" ) {
    bay = CreateNewBay(BayTypeDragElement, MainBays.length+1);
	console.log(bay.dataBay);
	WebSocketIFSendAddBayRequest(bay);
    MainBays.push(bay.dataBay);
  }
}


// FILE: ./Files/Callbacks/CBGarbageCanDragOver.js
/*****************************************************************************!
 * Function : CBGarbageCanDragOver
 *****************************************************************************/
function
CBGarbageCanDragOver
(InEvent)
{
  if ( PanelBeingDragged ) {
    InEvent.preventDefault();
  }
}

// FILE: ./Files/Callbacks/CBRegisterGroupChange.js
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

// FILE: ./Files/Callbacks/Callbacks.js
/*****************************************************************************
 * FILE NAME    : Callbacks.js
 * DATE         : June 15 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
"use strict";

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var DragElementType			= null;
var PanelTypeDragElement		= null;

// FILE: ./Files/Callbacks/CBPanelTypeDragEnd.js
/*****************************************************************************!
 * Function : CBPanelTypeDragEnd
 *****************************************************************************/
function
CBPanelTypeDragEnd(InEvent)
{
  PanelTypeDragElement = null;
  DragElementType = null;
}


// FILE: ./Files/Callbacks/CBGarbageCanOpen.js
/*****************************************************************************!
 * Function : CBGarbageCanOpen
 *****************************************************************************/
function
CBGarbageCanOpen
(InEvent)
{
  var					e;
  var					can;
  var					bayarea;

  bayarea = document.getElementById("BayDisplayArea");
  bayareawidth = parseInt(bayarea.clientWidth, 10);
  bayareaheight = parseInt(bayarea.clientHeight, 10);

  can = document.getElementById("GarbageCan");
  can.style.visibility = "visible";
 
  canwidth = parseInt(can.clientWidth, 10);
  canheight = parseInt(can.clientHeight, 10);

  canx = (bayareawidth - canwidth) / 2;
  cany = (bayareaheight - canheight) / 2;

  can.style.top = cany + "px";
  can.style.left = canx + "px";
 
  e = document.getElementById("GarbageCanLid");
  e.className = "GarbageCanLid GarbageCanLidOpen";

  PanelBeingDragged = InEvent.currentTarget; 
}

// FILE: ./Files/Callbacks/CBBayEdit.js
/*****************************************************************************!
 * Function : CBBayEdit
 *****************************************************************************/
function
CBBayEdit(InEvent)
{
  var					bayelement;
  bayelement = InEvent.srcElement.parentElement.parentElement;
  BayEdit(bayelement);
}

// FILE: ./Files/Callbacks/CBBayDragOver.js
/*****************************************************************************!
 * Function : CBBayDragOver
 *****************************************************************************/
function
CBBayDragOver(InEvent)
{
  if ( DragElementType == "Bay" ) {
    InEvent.preventDefault();
  }
}

// FILE: ./Files/BayTypes/BayTypesListPopulate.js
/*****************************************************************************!
 * Function : BayTypesListPopulate 
 *****************************************************************************/
function
BayTypesListPopulate(InBayTypeName)
{
  var					displayarea;
  var					y, i, bayType;
  var					messageline;
  var					lineheight;
  var					topborder;

  topborder = null;
  displayarea = document.getElementById("EquipmentListArea");
  oddeven = "LineElementEven";
  y = 0;
  lineheight = 17;
  for (i = 0; i < MainBayTypes.length; i++) {
    baytype = MainBayTypes[i];
    paneltypes = PanelTypesGetByUsedIdListNumber(baytype.listnumber);
    
    messageline = document.createElement("div");
    messageline.id = baytype.listnumber;
    messageline.className = "LineElement LineElementEven";
    messageline.dataBayType = baytype;
    messageline.dataSelected = false;
    messageline.dataType = "Bay";

    messageline.draggable = true;
    messageline.ondragstart = function(event) { CBBayTypeDragStart(event); }
    messageline.ondragend   = function(event) { CBBayTypeDragEnd(event); }
    messageline.innerHTML = baytype.name;

    messageline.style.top = y;
    messageline.style.cursor = "pointer";
    if ( topborder ) {
      messageline.style.borderTop = topborder;
    }
    topborder = "solid thin black";
    displayarea.appendChild(messageline);
    y += lineheight;
    if ( InBayTypeName == null || InBayTypeName == baytype.name ) {
      for ( j = 0; j < paneltypes.length; j++ ) {
        paneltype = paneltypes[j];
        messageline = document.createElement("div");
        messageline.className = "LineElement LineElementOdd";
        messageline.id = baytype.listnumber + " " + paneltype.name;
        messageline.dataPanelType = paneltype;
        messageline.dataBayType = baytype;
        messageline.dataType = "Panel";
        messageline.ondragstart = function(event) { CBPanelTypeDragStart(event); }
        messageline.ondragend   = function(event) { CBPanelTypeDragEnd(event); }
        messageline.draggable = true;
        messageline.innerHTML = paneltype.name;

        messageline.style.top = y;
        messageline.style.cursor = "pointer";
        messageline.style.paddingLeft = "10px";
        displayarea.appendChild(messageline);
        y += lineheight;
      }   
      y += 5;
    }
  } 
}

// FILE: ./Files/BayTypes/BayTypes.js
/*****************************************************************************
 * FILE NAME    : BayTypes.js
 * DATE         : June 15 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var BayTypeDragElement			= null;


// FILE: ./Files/BayTypes/BayTypeListClear.js
/*****************************************************************************!
 * Function : BayTypeListClear
 *****************************************************************************/
function
BayTypeListClear
()
{
  var					list;

  list = document.getElementById("EquipmentListArea");

  while ( list.children.length ) {
    list.removeChild(list.children[0]);
  }
}

// FILE: ./Files/BayTypes/BayTypeFindByName.js
/*****************************************************************************!
 * Function : BayTypeFindByName
 *****************************************************************************/
function
BayTypeFindByName
(InBayTypeName)
{
  var					i;
  for (i = 0; i < MainBayTypes.length; i++) {
    if ( InBayTypeName == MainBayTypes[i].name ) {
      return MainBayTypes[i];
    }
  }
  return null;
}
// FILE: ./websocketinfo.js
var WebSocketIFAddress = "192.168.0.26";
var WebSocketIFPort = "8001";
