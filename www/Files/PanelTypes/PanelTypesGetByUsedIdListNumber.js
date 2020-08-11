/*****************************************************************************!
 * Function : PanelTypesGetByUsedIdListNumber 
 *****************************************************************************/
function
PanelTypesGetByUsedIdListNumber(InListNumber)
{
  var					returnTypes = [];
  var					i;

  for (i = 0; i < MainPanelTypes.length; i++) {
    if ( InListNumber == MainPanelTypes[i].usedin ) {
      returnTypes.push(MainPanelTypes[i]);
    }
  }
  return returnTypes;
}

