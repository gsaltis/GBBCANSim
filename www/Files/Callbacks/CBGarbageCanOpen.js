/*****************************************************************************!
 * Function : CBGarbageCanOpen
 *****************************************************************************/
function
CBGarbageCanOpen
(InEvent)
{
  var					e;
  var					can;
  var					bayarea;

  bayarea = document.getElementById("BayDisplayArea");
  bayareawidth = parseInt(bayarea.clientWidth, 10);
  bayareaheight = parseInt(bayarea.clientHeight, 10);

  can = document.getElementById("GarbageCan");
  can.style.visibility = "visible";
 
  canwidth = parseInt(can.clientWidth, 10);
  canheight = parseInt(can.clientHeight, 10);

  canx = (bayareawidth - canwidth) / 2;
  cany = (bayareaheight - canheight) / 2;

  can.style.top = cany + "px";
  can.style.left = canx + "px";
 
  e = document.getElementById("GarbageCanLid");
  e.className = "GarbageCanLid GarbageCanLidOpen";

  PanelBeingDragged = InEvent.currentTarget; 
}

