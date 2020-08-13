/*****************************************************************************!
 * Function : ClientUserInputHandleRemove
 *****************************************************************************/
void
ClientUserInputHandleRemove
(StringList* InParameters, string InCommandString)
{
  if ( NULL == InParameters || NULL == InCommandString ) {
    return;
  }

  if ( InParameters->stringCount < 2 ) {
    printf("%s%s%s requires a subcommand\n", ColorRed, InParameters->strings[0], ColorReset);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "BAY") ) {
	ClientUserInputHandleRemoveBay(InParameters, InCommandString);
	return;
  }

  if ( StringEqualNoCase(InParameters->strings[1], "PANEL") ) {
	ClientUserInputHandleRemovePanel(InParameters, InCommandString);
	return;
  }

  printf("%s%s%s is an invalid subcommand to %s\n", ColorRed, InParameters->strings[1], ColorReset, InParameters->strings[0]);
}

