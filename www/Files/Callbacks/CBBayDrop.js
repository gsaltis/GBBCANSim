/*****************************************************************************!
 * Function : CBBayDrop
 *****************************************************************************/
function
CBBayDrop(InEvent)
{
  var					bay;

  if ( DragElementType == "Bay" ) {
    bay = CreateNewBay(BayTypeDragElement, MainBays.length+1);
	WebSocketIFSendAddBayRequest(bay);
    MainBays.push(bay.dataBay);
  }
}


