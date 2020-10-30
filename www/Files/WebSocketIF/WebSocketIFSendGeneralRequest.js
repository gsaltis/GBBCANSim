/*****************************************************************************!
 * Function : WebSocketIFSendGeneralRequest
 *****************************************************************************/
function 
WebSocketIFSendGeneralRequest(InRequest) {
  if ( WebSocketIFConnection ) {
	console.log(InRequest);
    WebSocketIFConnection.send(JSON.stringify(InRequest));
  }
}

