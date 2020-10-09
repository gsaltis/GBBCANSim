/*****************************************************************************!
 * Function : ClientUserInputStartThread
 *****************************************************************************/
void
ClientUserInputStartThread
()
{
  pthread_create(&ClientUserInputCommThreadID, NULL, ClientUserInputCommThread, NULL);
  pthread_create(&ClientUserInputThreadID, NULL, ClientUserInputThread, NULL);
}
