/*****************************************************************************!
 * Function : BayRepositionRegisterLines
 *****************************************************************************/
function
BayRepositionRegisterLines
()
{
  var					registerArea;
  var					y, i, register;
  var					groupstatus, currentgroup;

  registerArea = document.getElementById("RegisterArea");
  y = 0;
  for ( i = 0 ; i < registerArea.children.length; i++ ) {
    registerline = registerArea.children[i];
    registerline.style.top = y + "px";
    if ( registerline.className == "RegisterLine RegisterHeading" ) {
      y += 19;
      groupstatus = registerline.dataStatus;
      currentgroup = registerline.dataGroup;
    } else {
      if ( registerline.dataGroup == currentgroup ) {
        if ( groupstatus == "Open" ) {
          registerline.style.top = y + "px";
	  registerline.style.visibility = "visible";
          y += 19;
        } else {
	  registerline.style.visibility = "hidden";
        }
      } else {
        y += 19;
        registerline.style.visibility = "visible";
      }
    }
  }
}

