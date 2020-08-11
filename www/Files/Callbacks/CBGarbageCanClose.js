/*****************************************************************************!
 * Function : CBGarbageCanClose
 *****************************************************************************/
function
CBGarbageCanClose
(InEvent)
{
  var					e;
  var					can;

  e = document.getElementById("GarbageCanLid");
  e.className = "GarbageCanLid";

  can = document.getElementById("GarbageCan");
  can.style.visibility = "hidden";
  PanelBeingDragged = null;
}

