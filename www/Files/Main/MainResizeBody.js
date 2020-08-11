/*****************************************************************************!
 * Function : MainResizeBody
 *****************************************************************************/
function
MainResizeBody
(InEvent)
{
  var					mainArea, clientWidth, clientHeight;
  var					i, bay, locationInfo;

  mainArea = document.getElementById("BayDisplayArea");

  clientWidth = mainArea.clientWidth;
  clientHeight = mainArea.clientHeight;

  console.log(clientWidth, clientHeight);

  for (i = 0; i < mainArea.children.length; i++) {
    bay = mainArea.children[i];
    BayResize(bay);
    locationInfo = GetPanelPositionsSizes(bay);
    for ( j = 0; j < bay.children.length; j++ ) {
      panel = bay.children[j];
      if ( panel.dataType == "EmptyPanel" || panel.dataType == "Panel" ) {
        panel.style.width = locationInfo.panelwidth;
        panel.style.height = locationInfo.panelheight;
        panel.style.top = locationInfo.panelYs[panel.dataIndex];
        n = locationInfo.panelXs[(panel.dataIndex - 1) % 2];
        panel.style.left = n;
      }
    }
  }
}
