/*****************************************************************************!
 * Function : ClientUserInputHandleAddDevice
 *****************************************************************************/
void
ClientUserInputHandleAddDevice
(StringList* InParameters, string InCommandString)
{
  string								s;

  s = StringListToJSONArray(InParameters);
  ClientUserInputSendCommandMessage(s);
  FreeMemory(s);
}
