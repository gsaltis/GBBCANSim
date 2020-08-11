/*****************************************************************************!
 * Function : BayTypeListClear
 *****************************************************************************/
function
BayTypeListClear
()
{
  var					list;

  list = document.getElementById("EquipmentListArea");

  while ( list.children.length ) {
    list.removeChild(list.children[0]);
  }
}

