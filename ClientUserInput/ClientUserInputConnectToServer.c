/*****************************************************************************!
 * Function : ClientUserInputConnectToServer
 *****************************************************************************/
void
ClientUserInputConnectToServer
()
{
  struct mg_connection *nc;
  string url = StringMultiConcat("ws://", ClientUserInputAddress, ":", ClientUserInputPortNumber, NULL);
  mg_mgr_init(&ClientUserInputConnectionMgr, NULL);
  nc = mg_connect_ws(&ClientUserInputConnectionMgr, ClientUserInputEventHandler, url, "ws_client", NULL);
  if ( nc ) {
	ClientUserInputConnection = nc;
	printf("Connecting to %s\n", url);
  } else {
	fprintf(stderr, "Could not connect url %s\n", url);
  }
}
