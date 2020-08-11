/*****************************************************************************!
 * Function : BayFindLowestPanel
 *****************************************************************************/
function
BayFindLowestPanel
(InBay)
{
  var					i, panel, panely, returnpanel, y;

  returnpanel = null;
  y = 0;
  for (i = 0; i < InBay.children.length; i++) {
    panel = InBay.children[i];
    if ( panel.dataType != "Panel" && panel.dataType != "EmptyPanel" ) {
      continue;
    }
    panely = parseInt(panel.style.top, 10);
    if ( panely > y ) {
      returnpanel = panel;
      y = panely;
    }
  }
  return returnpanel;
}


