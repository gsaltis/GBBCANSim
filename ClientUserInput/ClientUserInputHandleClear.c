/*****************************************************************************!
 * Function : ClientUserInputHandleClear
 *****************************************************************************/
void
ClientUserInputHandleClear
(StringList* InParameters, string InCommandString)
{
  if ( InParameters == NULL || InCommandString == NULL ) {
  	return;
  }

  if ( InParameters->stringCount < 2 ) {
    printf("%s%s%s requires a subcommand\n", ColorRed, InParameters->strings[0], ColorReset);
 	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "DEVICES") ) {
  	ClientUserInputHandleClearDevices(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "DEVICE") ) {
	ClientUserInputHandleClearDevice(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "?") ) {
    printf("%s%s%s [ DEVICE | DEVICES ]\n", ColorGreen, InParameters->strings[0], ColorReset);
	return;
  }
  printf("%s%s%s is an invalid subcommand to %s\n", ColorRed, InParameters->strings[1], ColorReset, InParameters->strings[0]);
}
