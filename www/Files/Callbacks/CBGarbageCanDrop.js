/*****************************************************************************!
 * Function : CBGarbageCanDrop
 *****************************************************************************/
function
CBGarbageCanDrop
(InEvent)
{
  if ( PanelBeingDragged ) {
    WebSocketIFSendRemovePanelRequest(PanelBeingDragged.dataPanel.bay.index, PanelBeingDragged.dataPanel.index); 
  }
  PanelBeingDragged = null;
}

