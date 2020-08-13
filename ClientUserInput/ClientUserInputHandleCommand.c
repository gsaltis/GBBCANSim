/*****************************************************************************!
 * Function : ClientUserInputHandleCommand
 *****************************************************************************/
void
ClientUserInputHandleCommand
(StringList* InParameters, string InCommandString)
{
  if ( InParameters == NULL || InParameters->stringCount == 0 ) {
    return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "EXIT") ) {
    ClientUserInputHandleExit(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "HELP") ) {
	ClientUserInputHandleHelp(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "ADD") ) {
	ClientUserInputHandleAdd(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "BAYS") ) {
	ClientUserInputHandleBays(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[0], "CLEAR") ) {
	ClientUserInputHandleClear(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "LIST") ) {
	ClientUserInputHandleList(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "OPTION") ) {
	ClientUserInputHandleOption(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "REMOVE") ) {
	ClientUserInputHandleRemove(InParameters, InCommandString);
	return;
  }
  if ( StringEqualNoCase(InParameters->strings[0], "PANELS") ) {
	ClientUserInputHandlePanels(InParameters, InCommandString);
	return;
  }
   if ( StringEqualNoCase(InParameters->strings[0], "SET") ) {
	ClientUserInputHandleSet(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[0], "SHOW") ) {
	ClientUserInputHandleShow(InParameters, InCommandString);
	return;
  }

  printf_safe("%s%s%s is an invalid command\n", ColorRed, InParameters->strings[0], ColorReset);
}
