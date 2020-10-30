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
  if ( editMode == "None" ) {
    PanelEdit(panelelement);
  } else if ( editMode == "Panel" ) {
    if ( edittingpanel == null || edittingpanel.dataPanel.index == panelelement.dataPanel.index ) {
      PanelEditClose(panelelement.parentElement);
    }
  }
}

