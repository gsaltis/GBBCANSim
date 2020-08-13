/*****************************************************************************!
 * Function : ClientUserInputSetPort
 *****************************************************************************/
void
ClientUserInputSetPort
(string InPortNumber)
{
  if ( NULL == InPortNumber ) {
	return;
  }
  if ( ClientUserInputPortNumber ) {
	FreeMemory(ClientUserInputPortNumber);
  }
  ClientUserInputPortNumber = StringCopy(InPortNumber);
}
