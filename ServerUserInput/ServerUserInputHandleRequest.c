/*****************************************************************************!
 * Function : ServerUserInputHandleRequest
 *****************************************************************************/
void
ServerUserInputHandleRequest
(struct mg_connection* InConnection, struct mg_str InMessageString)
{
  json_value*						value;
  printf("%s %d : %s %d\n", __FILE__, __LINE__, InMessageString.p, InMessageString.len); 
}
