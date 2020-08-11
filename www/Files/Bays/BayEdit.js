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


