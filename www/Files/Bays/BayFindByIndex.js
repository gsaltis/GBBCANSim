/*****************************************************************************!
 * Function : BayFindByIndex
 *****************************************************************************/
function
BayFindByIndex
(InBayIndex)
{
  var					i;

  for ( i = 0; i < MainBays.length; i++ ) {
    if ( MainBays[i].bayindex == InBayIndex ) {
      return MainBays[i];
    }
  }
  return null;
}


