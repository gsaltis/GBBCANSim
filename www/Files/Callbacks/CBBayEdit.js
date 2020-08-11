/*****************************************************************************!
 * Function : CBBayEdit
 *****************************************************************************/
function
CBBayEdit(InEvent)
{
  var					bayelement;
  bayelement = InEvent.srcElement.parentElement.parentElement;
  BayEdit(bayelement);
}

