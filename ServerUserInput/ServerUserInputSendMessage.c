/*****************************************************************************!
 * Function : ServerUserInputSendMessage
 *****************************************************************************/
void
ServerUserInputSendMessage
(struct mg_connection* InConnection, string InMessage)
{
  mg_send_websocket_frame(InConnection, WEBSOCKET_OP_TEXT, InMessage, strlen(InMessage));
}
