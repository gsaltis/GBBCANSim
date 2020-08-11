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

