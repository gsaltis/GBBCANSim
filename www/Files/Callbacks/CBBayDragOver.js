/*****************************************************************************!
 * Function : CBBayDragOver
 *****************************************************************************/
function
CBBayDragOver(InEvent)
{
  if ( DragElementType == "Bay" ) {
    InEvent.preventDefault();
  }
}

