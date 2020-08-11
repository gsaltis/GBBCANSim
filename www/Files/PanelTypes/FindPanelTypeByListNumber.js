/*****************************************************************************!
 * Function : FindPanelTypeByListNumber
 *****************************************************************************/
function
FindPanelTypeByListNumber
(InPanelTypeName)
{
  var					i;
  for ( i = 0; i < MainPanelTypes.length; i++ ) {
    if ( MainPanelTypes[i].listnumber == InPanelTypeName ) {
      return MainPanelTypes[i];
    }
  }
  return null;
}
