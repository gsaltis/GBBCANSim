/*****************************************************************************!
 * Function : ServerUserInputJSONArrayToStringList
 *****************************************************************************/
StringList*
ServerUserInputJSONArrayToStringList
(json_value* InJSONArray)
{
  int									i;
  StringList*							list;
  json_value*							v;
  string								s;

  if ( NULL == InJSONArray ) {
	return NULL;
  }

  list = StringListCreate();

  for ( i = 0; i < InJSONArray->u.array.length; i++ ) {
	v = InJSONArray->u.array.values[i];
	if ( v->type != json_string ) {
	  continue;
	}
	s = StringCopy(v->u.string.ptr);
	StringListAppend(list, s);
  }
  return list;
}
