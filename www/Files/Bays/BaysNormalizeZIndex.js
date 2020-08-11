/*****************************************************************************!
 * Function : BaysNormalizeZIndex
 *****************************************************************************/
function
BaysNormalizeZIndex
()
{
  var					displayarea, i;
  displayarea = document.getElementById("BayDisplayArea");
  for (i = 0; i < displayarea.children.length; i++) {
    displayarea.children[i].style.zIndex = 20;
  }
}


