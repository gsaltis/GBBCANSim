/*****************************************************************************!
 * Function : ClientUserInputHandleAdd
 *****************************************************************************/
void
ClientUserInputHandleAdd
(StringList* InParameters, string InCommandString)
{
  if ( InParameters == NULL || InCommandString == NULL ) {
    return;
  }

  if ( InParameters->stringCount < 2 ) {
    printf("%s%s%s command requires a subcommand", ColorRed, InParameters->strings[0], ColorReset);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "BAY") ) {
   	ClientUserInputHandleAddBay(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "PANEL") ) {
	ClientUserInputHandleAddPanel(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "DEVICE") ) {
	ClientUserInputHandleAddDevice(InParameters, InCommandString);
	return;
  }
  printf("%s%s%s is invalid subcommand to %s", ColorRed, InParameters->strings[1], ColorReset, InParameters->strings[0]);
}
