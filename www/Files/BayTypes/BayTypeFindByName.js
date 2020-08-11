/*****************************************************************************!
 * Function : BayTypeFindByName
 *****************************************************************************/
function
BayTypeFindByName
(InBayTypeName)
{
  var					i;
  for (i = 0; i < MainBayTypes.length; i++) {
    if ( InBayTypeName == MainBayTypes[i].name ) {
      return MainBayTypes[i];
    }
  }
  return null;
}
