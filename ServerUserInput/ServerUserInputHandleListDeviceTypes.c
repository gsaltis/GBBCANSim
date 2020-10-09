/*****************************************************************************!
 * Function : ServerUserInputHandleListDeviceTypes
 *****************************************************************************/
void
ServerUserInputHandleListDeviceTypes
(struct mg_connection* InConnection, StringList* InCommandStrings)
{
  string								returnString;

  returnString = StringCopy("{ \"type\" : \"response\", \"devicetypes\" : [ ");
  for ( DeviceDef* deviceDef = mainDeviceDefs->defs; deviceDef; deviceDef = deviceDef->next ) {
	string s = StringMultiConcat("\"", deviceDef->name, "\"", NULL);
    returnString = StringConcatTo(returnString, s);
	FreeMemory(s);
	if ( deviceDef->next ) {
	  returnString = StringConcatTo(returnString, ", ");
	}
  }
  returnString = StringConcatTo(returnString, " ] }");
  printf("%s %d : %s\n", __FILE__, __LINE__, returnString);
  ServerUserInputSendMessage(InConnection, returnString);
  FreeMemory(returnString);
}
