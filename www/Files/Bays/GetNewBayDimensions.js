/*****************************************************************************!
 * Function : GetNewBayDimensions
 *****************************************************************************/
function
GetNewBayDimensions
()
{
  var					dimensions = {};
  var					displayarea, displayareaheight;
  var					baytop;
 
  baytop = parseInt(getComputedStyle(document.body).getPropertyValue("--BayTop"), 10);

  displayarea = document.getElementById("BayDisplayArea");
  displayareaheight = displayarea.clientHeight;

  dimensions.bayx = BayDisplayXSkip;
  dimensions.bayheight = displayareaheight - (baytop * 2);
  dimensions.baywidth = dimensions.bayheight / 84 * 30;
  return dimensions;
}

