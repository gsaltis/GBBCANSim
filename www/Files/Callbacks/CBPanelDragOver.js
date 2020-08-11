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

