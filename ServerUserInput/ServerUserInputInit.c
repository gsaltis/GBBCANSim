/*****************************************************************************!
 * Function : ServerUserInputInit
 *****************************************************************************/
void
ServerUserInputInit
()
{
  ServerUserInputPort = GetMemory(8);
  sprintf(ServerUserInputPort, "%d", SERVER_USER_INPUT_PORT);
  pthread_create(&ServerUserInputThreadID, NULL, ServerUserInputThread, NULL);
}
