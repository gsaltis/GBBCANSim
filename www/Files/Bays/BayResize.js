/*****************************************************************************!
 * Function : BayResize
 *****************************************************************************/
function
BayResize
(InBay)
{
  var					registerAreaName, registerArea;
  var					registerAreaWidth, registerAreaHeight;

  dimensions = GetNewBayDimensions();
  MainDisplayMessage(dimensions.baywidth + " : " + dimensions.bayheight);
  registerAreaWidth  = 0;
  registerAreaHeight = 0;
  
  registerAreaName = "RegisterArea";
  registerArea = document.getElementById(registerAreaName);
  if ( registerArea ) {
    registerAreaWidth = registerArea.clientWidth;
    registerAreaHeight = registerArea.clientHeight;
  }
  InBay.style.width = dimensions.baywidth;
  InBay.style.height = dimensions.bayheight;
}
