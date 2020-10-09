/*****************************************************************************!
 * Function : ServerUserInputHandleRequest
 *****************************************************************************/
void
ServerUserInputHandleRequest
(struct mg_connection* InConnection, struct mg_str InMessageString)
{
  json_value*							value;
  string								type;
  json_value*							commandArray;
  json_value*							v;
  StringList*							list;

  value = json_parse((const json_char*)InMessageString.p, (size_t)InMessageString.len);
  type = JSONIFGetString(value, "type");
  if ( NULL == type ) {
    json_value_free(value);
	return;
  }
  do {
    if ( !StringEqual(type, "command") ) {
	  break;
    } 
    commandArray = JSONIFGetArray(value, "command");
    if ( NULL == commandArray ) {
	  break;
	}
    if ( commandArray->u.array.length < 1 ) {
	  break;
	}
    v = commandArray->u.array.values[0];
    if ( v->type != json_string ) {
	  break;
 	}
    list = ServerUserInputJSONArrayToStringList(commandArray);
    if ( StringEqualNoCase(list->strings[0], "ADD") ) {	 
	  ServerUserInputHandleAdd(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "REMOVE") ) {
	  ServerUserInputHandleRemove(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "LIST") ) {
	  ServerUserInputHandleList(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "CLEAR") ) {
	  ServerUserInputHandleClear(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "SET") ) {
	  ServerUserInputHandleSet(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "SHOW") ) {
	  ServerUserInputHandleShow(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "BAYS") ) {
	  ServerUserInputHandleBays(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "PANELS") ) {
	  ServerUserInputHandlePanels(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "CONNECTIONS") ) {
	  ServerUserInputHandleConnections(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "OPTIONS") ) {
	  ServerUserInputHandleOptions(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "CLOSE") ) {
	  ServerUserInputHandleClose(InConnection, list);
	  break;
	}
    if ( StringEqualNoCase(list->strings[0], "STATUS") ) {
	  ServerUserInputHandleStatus(InConnection, list);
	  break;
	}

	StringListDestroy(list);
  } while (false);

  json_value_free(value);
  FreeMemory(type);
}
