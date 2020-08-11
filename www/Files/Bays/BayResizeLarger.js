/*****************************************************************************!
 * Function : BayResizeLarger.js
 *****************************************************************************/
function
BayResizeLarger(InBay)
{
  var					bayareawidth, left, baywidth;

  bayarea = document.getElementById("BayDisplayArea");
  bayareawidth = bayarea.clientWidth;
  
  
  InBay.dataBayWidth = InBay.style.width;
  left = parseInt(InBay.style.left, 10);
  
  baywidth = parseInt(InBay.style.width, 10) + 500;
  m = (left + baywidth) - 10;
  if ( m > bayareawidth ) {
    baywidth = bayareawidth - left;
  }
  InBay.style.width = baywidth + "px";
}

