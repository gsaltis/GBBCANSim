/*****************************************************************************!
 * Function : ServerUserInputThread
 *****************************************************************************/
void*
ServerUserInputThread
(void* InParameters)
{
  while (true) {
    struct mg_connection *              nc;
    struct mg_mgr                       mgr;
    if ( MainVerbose ) {
      printf("Starting remote user input server thread on %s:%s\n", WebSocketIFAddress, ServerUserInputPort); 
    }
    mg_mgr_init(&mgr, NULL);

    nc = mg_bind(&mgr, ServerUserInputPort, ServerUserInputEventHandle);
    if (nc == NULL) {
      printf("Failed to create remote user input listener\n");
      return (void*)1;
    }
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = MainDefaultWebDirectory;
    s_http_server_opts.enable_directory_listing = "yes";

    for (;;) {
      mg_mgr_poll(&mgr, 20);            
    }
  }
  return NULL;
}
