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
