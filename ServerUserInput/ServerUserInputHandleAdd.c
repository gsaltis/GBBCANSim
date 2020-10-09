/*****************************************************************************!
 * Function : ServerUserInputHandleAdd
 *****************************************************************************/
void
ServerUserInputHandleAdd
(struct mg_connection* InConnection, StringList* InCommandStrings)
{
  if ( InConnection == NULL || InCommandStrings == NULL ) {
    return;
  }
  if ( InCommandStrings->stringCount < 2 ) {
	return;
  }

  if ( StringEqualNoCase(InCommandStrings->strings[1], "DEVICE") ) {
    ServerUserInputHandleAddDevice(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "BAY") ) {
    ServerUserInputHandleAddBay(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "PANEL") ) {
    ServerUserInputHandleAddPanel(InConnection, InCommandStrings);
	return;
  }
}
