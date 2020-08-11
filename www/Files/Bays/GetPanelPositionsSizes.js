/*****************************************************************************!
 * Function : GetPanelPositionsSizes
 *****************************************************************************/
function
GetPanelPositionsSizes
(InBay)
{
  var				n, width, height, panelverticalarea, m; 
  var				returnInfo = {};
  var				i, k, h;
 
  n = InBay.dataBay.type.panelcount;
  width = parseInt(InBay.style.width, 10);
  height = parseInt(InBay.style.height, 10);

  returnInfo.panelwidth        = (width - (BayDisplayPanelXSkip * 3)) / 2;
  panelverticalarea = height - (BayPanelTop + BayPanelBottom);
  m                 = Math.floor(n / 2);
  returnInfo.panelheight       = ((panelverticalarea - ((m - 1) * BayDisplayPanelYSkip)) / m);
  returnInfo.panelXs = [ BayDisplayPanelXSkip, (BayDisplayPanelXSkip * 2) + returnInfo.panelwidth ];
  k = n / 2 - 1;

  returnInfo.panelYs = [];
  returnInfo.panelYs.push(0);
  for ( i = k; i >= 0; i-- ) {
    h = BayPanelTop + (returnInfo.panelheight + BayDisplayPanelYSkip) * i;
    returnInfo.panelYs.push(h);
    returnInfo.panelYs.push(h);
  }
  return returnInfo;
}

