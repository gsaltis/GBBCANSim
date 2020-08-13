/*****************************************************************************!
 * Function : ClientUserInputInit
 *****************************************************************************/
void
ClientUserInputInit
()
{
  ClientUserInputAddress = StringCopy("127.0.0.1");
  ClientUserInputPortNumber = GetMemory(8);
  sprintf(ClientUserInputPortNumber, "%d", SERVER_USER_INPUT_PORT); 
}
