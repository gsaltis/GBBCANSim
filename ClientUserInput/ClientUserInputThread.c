/*****************************************************************************!
 * Function : ClientUserInputThread
 *****************************************************************************/
void*
ClientUserInputThread
(void* InParameters)
{
  string								s, s2;
  StringList*							parameters;

  while (true) {
    s = linenoise("Enter Command : ");

    if ( s == NULL ) {
	  continue;
	}

    s2 = StringTrim(s);
	free(s);

    if ( s2 ) {
	  if ( *s2 == NUL ) {
		FreeMemory(s2);
	  	continue;
	  }
	}
    parameters = StringSplit(s2, " \t", true);
    ClientUserInputHandleCommand(parameters, s2);
	FreeMemory(s2);
    StringListDestroy(parameters);
  }
  return NULL;
}
