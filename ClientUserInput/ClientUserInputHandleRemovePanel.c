/*****************************************************************************!
 * Function : ClientUserInputHandleRemovePanel
 *****************************************************************************/
void
ClientUserInputHandleRemovePanel
(StringList* InParameters, string InCommandString)
{
  string								s;

  if ( InParameters->stringCount != 4 ) {
 	printf("Too few parameters to %s %s\n", InParameters->strings[0], InParameters->strings[1]);
	return;
  }

  s = StringListToJSONArray(InParameters);
  ClientUserInputSendCommandMessage(s);
  FreeMemory(s);
}
