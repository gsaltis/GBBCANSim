/*****************************************************************************!
 * Function : ClientUserInputSetAddress
 *****************************************************************************/
void
ClientUserInputSetAddress
(string InAddress)
{
  if ( NULL == InAddress ) {
	return;
  }
  if ( ClientUserInputAddress ) {
	FreeMemory(ClientUserInputAddress);
  }
  ClientUserInputAddress = StringCopy(InAddress);
}
