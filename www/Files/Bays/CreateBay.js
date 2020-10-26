/*****************************************************************************!
 * Function : CreateNewBay 
 *****************************************************************************/
function
CreateBay(InBay)
{
  var					displayarea;
  var					bay;
  var					headerbutton;
  var					dimensions;
  var					header;

  console.log(InBay);
  baytype = InBay;
  displayarea = document.getElementById("BayDisplayArea");

  dimensions = GetNewBayDimensions();   
  
  bay = document.createElement("div");
  bay.className = "GeneralPane Bay";
  bay.id = "Bay " + InIndex;
  bay.dataBay = {};
  bay.dataBay.type = InBayType;
  bay.dataBay.index = InIndex;
  bay.dataBay.device = DeviceDefFindByDescription("Bay");
  bay.dataEdittingMode = "None";

  bay.ondragenter  = function(event) { CBBayDragEnter(event); }
  bay.ondragleave  = function(event) { CBBayDragLeave(event); }
  bay.style.width  = dimensions.baywidth;
  bay.style.height = dimensions.bayheight;
  bay.style.left   = dimensions.bayx;

  header = document.createElement("div");
  header.className = "GeneralPaneHeader";
  header.innerHTML = "Bay " + bay.dataBay.index;
  
  headerbutton = document.createElement("div");
  headerbutton.className = "GeneralPaneHeaderButton";
  headerbutton.onclick = function(event) { CBBayEdit(event); }
  header.appendChild(headerbutton);

  bay.appendChild(header);

  CreateEmptyPanels(bay);
  displayarea.appendChild(bay); 
  bay.addEventListener("transitionend", BayTransitionEnd, true);
  return bay;
}

