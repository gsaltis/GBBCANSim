/*****************************************************************************!
 * Function : ServerUserInputHandleShow
 *****************************************************************************/
void
ServerUserInputHandleShow
(struct mg_connection* InConnection, StringList* InCommandStrings)
{
  if ( InConnection == NULL || InCommandStrings == NULL ) {
    return;
  }
  if ( InCommandStrings->stringCount < 2 ) {
	return;
  }

  if ( StringEqualNoCase(InCommandStrings->strings[1], "DEVICE") ) {
    ServerUserInputHandleShowDevice(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "GROUP") ) {
    ServerUserInputHandleShowGroup(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "BAY") ) {
    ServerUserInputHandleShowBay(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "MESSAGES") ) {
    ServerUserInputHandleShowMessages(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "ALL") ) {
    ServerUserInputHandleShowAll(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "CAN") ) {
    ServerUserInputHandleShowCAN(InConnection, InCommandStrings);
	return;
  }
}
