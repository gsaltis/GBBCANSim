/*****************************************************************************!
 * Function : BayTransitionEnd
 *****************************************************************************/
function
BayTransitionEnd
(InEvent)
{
  var					bay;
  bay = InEvent.srcElement;

  if ( bay.style.width == bay.dataBayWidth ) {
    // Only remove the children when the bay is closed
	if ( bay.editregisterarea ) {
      bay.removeChild(bay.editregisterarea);
	}
	if ( bay.editinfoarea ) {
      bay.removeChild(bay.editinfoarea);
	}
    bay.editregisterarea = null;
    bay.editinfoarea = null;
  }
}

