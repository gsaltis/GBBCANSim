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

