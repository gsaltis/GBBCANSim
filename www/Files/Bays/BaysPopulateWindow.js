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


