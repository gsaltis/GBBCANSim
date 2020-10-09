/*****************************************************************************!
 * Function : ClientUserInputHandleShow
 *****************************************************************************/
void
ClientUserInputHandleShow
(StringList* InParameters, string InCommandString)
{
  if ( NULL == InParameters || NULL == InCommandString ) {
    return;
  }

  if ( InParameters->stringCount < 2 ) {
    printf("%s%s%s requires a subcommand\n", ColorRed, InParameters->strings[0], ColorReset);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "ALL") ) {
	ClientUserInputHandleShowAll(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "BAY") ) {
	ClientUserInputHandleShowBay(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "DEVICE") ) {
	ClientUserInputHandleShowDevice(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "CAN") ) {
	ClientUserInputHandleShowCAN(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "GROUP") ) {
	ClientUserInputHandleShowGroup(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "MESSAGES") ) {
	ClientUserInputHandleShowMessages(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[1], "?") ) {
    printf("%s%s%s [ALL | BAY | CAN | DEVICE | GROUP | MESSAGES]\n", ColorGreen, InParameters->strings[0], ColorReset);
	return;
  }
  printf("%s%s%s is invalid subcommand to %s\n", ColorRed, InParameters->strings[0], ColorReset, InParameters->strings[0]);
}
