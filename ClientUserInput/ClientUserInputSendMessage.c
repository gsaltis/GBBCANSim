/*****************************************************************************!
 * Function : ClientUserInputSendMessage
 *****************************************************************************/
void
ClientUserInputSendMessage
(string InMessage)
{
  mg_send_websocket_frame(ClientUserInputConnection, WEBSOCKET_OP_TEXT, InMessage, strlen(InMessage));
}
