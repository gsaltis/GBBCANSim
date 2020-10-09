/*****************************************************************************!
 * Function : ServerUserInputHandleList
 *****************************************************************************/
void
ServerUserInputHandleList
(struct mg_connection* InConnection, StringList* InCommandStrings)
{
  if ( InConnection == NULL || InCommandStrings == NULL ) {
    return;
  }
  if ( InCommandStrings->stringCount < 2 ) {
	return;
  }

  if ( StringEqualNoCase(InCommandStrings->strings[1], "DEVICES") ) {
    ServerUserInputHandleListDevices(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "DEVICETYPES") ) {
    ServerUserInputHandleListDeviceTypes(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "REGISTERS") ) {
    ServerUserInputHandleListRegisters(InConnection, InCommandStrings);
	return;
  }
}
