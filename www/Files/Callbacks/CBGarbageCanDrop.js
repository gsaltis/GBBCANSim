/*****************************************************************************!
 * Function : CBGarbageCanDrop
 *****************************************************************************/
function
CBGarbageCanDrop
(InEvent)
{
  if ( PanelBeingDragged ) {
    console.log(PanelBeingDragged);
    WebSocketIFSendRemovePanelRequest(PanelBeingDragged.dataPanel.bay.index, PanelBeingDragged.dataPanel.index); 
  }
  PanelBeingDragged = null;
}

