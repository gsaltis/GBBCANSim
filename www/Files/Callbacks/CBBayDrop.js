/*****************************************************************************!
 * Function : CBBayDrop
 *****************************************************************************/
function
CBBayDrop(InEvent)
{
  var					bay;

  if ( DragElementType == "Bay" ) {
    bay = CreateNewBay(BayTypeDragElement, MainBays.length+1);
	console.log(bay.dataBay);
	WebSocketIFSendAddBayRequest(bay);
    MainBays.push(bay.dataBay);
  }
}


