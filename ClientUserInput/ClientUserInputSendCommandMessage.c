/*****************************************************************************!
 * Function : ClientUserInputSendCommandMessage
 *****************************************************************************/
void
ClientUserInputSendCommandMessage
(string InCommandMessage)
{
  string								message;

  message = StringCopy("{ \"type\" : \"command\", \"command\" : ");
  message = StringConcatTo(message, InCommandMessage);
  message = StringConcatTo(message, " }");

  printf("%s %d\n", message, strlen(message));
  mg_send_websocket_frame(ClientUserInputConnection, WEBSOCKET_OP_TEXT, message, strlen(message));
  FreeMemory(message);
}
