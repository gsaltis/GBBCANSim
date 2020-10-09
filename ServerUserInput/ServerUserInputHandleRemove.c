/*****************************************************************************!
 * Function : ServerUserInputHandleRemove
 *****************************************************************************/
void
ServerUserInputHandleRemove
(struct mg_connection* InConnection, StringList* InCommandStrings)
{
  if ( InConnection == NULL || InCommandStrings == NULL ) {
    return;
  }
  if ( InCommandStrings->stringCount < 2 ) {
	return;
  }

  if ( StringEqualNoCase(InCommandStrings->strings[1], "Panel") ) {
    ServerUserInputHandleRemovePanel(InConnection, InCommandStrings);
	return;
  }
  if ( StringEqualNoCase(InCommandStrings->strings[1], "BAY") ) {
    ServerUserInputHandleRemoveBay(InConnection, InCommandStrings);
	return;
  }

}
