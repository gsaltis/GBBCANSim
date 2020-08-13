/*****************************************************************************!
 * Function : ClientUserInputCommThread
 *****************************************************************************/
void*
ClientUserInputCommThread
(void* InParameter)
{
  ClientUserInputConnectToServer();
  while (true) {
    mg_mgr_poll(&ClientUserInputConnectionMgr, 100);
  }
  return NULL;
}
