/*****************************************************************************!
 * Function : BaysClearDisplayArea
 *****************************************************************************/
function
BaysClearDisplayArea
()
{
  var                                   d;
  d = document.getElementById("BayDisplayArea");
  while (d.children.length > 0) {
	d.removeChild(d.children[0]);
  }
}

