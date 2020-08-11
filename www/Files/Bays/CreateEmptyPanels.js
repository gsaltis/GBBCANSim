/*****************************************************************************!
 * Function : CreateEmptyPanels
 *****************************************************************************/
function
CreateEmptyPanels (InBay)
{
  var					i, x, n;

  var locationInfo = GetPanelPositionsSizes(InBay);
  for ( i = InBay.dataBay.type.panelcount; i > 0; i-- ) {
    n = (i - 1) % 2;
    x = locationInfo.panelXs[n];
    CreateEmptyPanel(InBay, i, x, locationInfo.panelYs[i], locationInfo.panelwidth, locationInfo.panelheight);
  }
}

