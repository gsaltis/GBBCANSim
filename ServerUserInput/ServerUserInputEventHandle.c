/*****************************************************************************!
 * Function : ServerUserInputEventHandle
 *****************************************************************************/
void
ServerUserInputEventHandle
(struct mg_connection* nc, int ev, void* p)
{
  WebConnection*			connection;

  if ( ev == 0 ) {
    return;
  }
  switch (ev) {
    case MG_EV_CLOSE : {
      connection = WebConnectionListFind(mainWebConnections, nc);
      if ( connection ) {
        WebConnectionListRemove(mainWebConnections, connection);
    	WebConnectionDestroy(connection);
      }
      break;
    }

    case MG_EV_WEBSOCKET_FRAME: {
      connection = WebConnectionListFind(mainWebConnections, nc);
      struct websocket_message *wm = (struct websocket_message *) p;
      struct mg_str d = {(char *) wm->data, wm->size};
      ServerUserInputHandleRequest(nc, d);
      break;
    }
  }
}
