/*****************************************************************************!
 * Function : ClientUserInputStartThread
 *****************************************************************************/
void
ClientUserInputStartThread
()
{
  pthread_create(&ClientUserInputThreadID, NULL, ClientUserInputThread, NULL);
  pthread_create(&ClientUserInputCommThreadID, NULL, ClientUserInputCommThread, NULL);
}
