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

