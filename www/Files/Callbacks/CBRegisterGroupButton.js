/*****************************************************************************!
 * Function : CBRegisterGroupButton
 *****************************************************************************/
function
CBRegisterGroupButton
(InEvent)
{
  var					element;

  element = InEvent.srcElement;

  if ( InEvent.ctrlKey == true ) {
    e = element.parentElement;
    e.children[1].style.visibility = "visible";
    e.children[2].style.visibility = "visible";
    // Gang change all element in the group
    return;
  }
  // Toggle the registers display
  if ( element.parentElement.dataStatus == "Open" ) {
    element.parentElement.dataStatus = "Closed";
    element.className = "RegisterGroupCollapseButton RegisterGroupClosed";
  } else {
    element.parentElement.dataStatus = "Open";
    element.className = "RegisterGroupCollapseButton RegisterGroupOpen";
  }
  BayRepositionRegisterLines();
} 
