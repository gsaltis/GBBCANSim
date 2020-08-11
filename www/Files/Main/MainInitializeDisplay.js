/*****************************************************************************!
 * Function : MainInitializeDisplay
 *****************************************************************************/
function
MainInitializeDisplay()
{
  var					name;
  name = null;

  if ( MainBays.length > 0 ) {
    name = MainBays[0].type;
  }
  
  BayTypesListPopulate(name);
}

