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
