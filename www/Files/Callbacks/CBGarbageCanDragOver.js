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

