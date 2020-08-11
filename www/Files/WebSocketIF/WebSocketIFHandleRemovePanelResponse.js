/*****************************************************************************!
 * Function : WebSocketIFHandleRemovePanelResponse
 *****************************************************************************/
function
WebSocketIFHandleRemovePanelResponse
(InID, InMessage, InBayIndex, InPanelIndex)
{
  RemovePanel(InBayIndex, InPanelIndex);
  MainDisplayMessageColor(InMessage, "#080");

  bay = document.getElementById("Bay " + InBayIndex);
  locationInfo = GetPanelPositionsSizes(bay);

  CreateEmptyPanel(bay, InPanelIndex, 
                   locationInfo.panelXs[(InPanelIndex - 1) % 2], locationInfo.panelYs[InPanelIndex],
                   locationInfo.panelwidth, locationInfo.panelheight); 
}

