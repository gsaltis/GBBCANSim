/*****************************************************************************!
 * Function : BayTypesListPopulate 
 *****************************************************************************/
function
BayTypesListPopulate(InBayTypeName)
{
  var					displayarea;
  var					y, i, bayType;
  var					messageline;
  var					lineheight;
  var					topborder;

  topborder = null;
  displayarea = document.getElementById("EquipmentListArea");
  oddeven = "LineElementEven";
  y = 0;
  lineheight = 17;
  for (i = 0; i < MainBayTypes.length; i++) {
    baytype = MainBayTypes[i];
    paneltypes = PanelTypesGetByUsedIdListNumber(baytype.listnumber);
    
    messageline = document.createElement("div");
    messageline.id = baytype.listnumber;
    messageline.className = "LineElement LineElementEven";
    messageline.dataBayType = baytype;
    messageline.dataSelected = false;
    messageline.dataType = "Bay";

    messageline.draggable = true;
    messageline.ondragstart = function(event) { CBBayTypeDragStart(event); }
    messageline.ondragend   = function(event) { CBBayTypeDragEnd(event); }
    messageline.innerHTML = baytype.name;

    messageline.style.top = y;
    messageline.style.cursor = "pointer";
    if ( topborder ) {
      messageline.style.borderTop = topborder;
    }
    topborder = "solid thin black";
    displayarea.appendChild(messageline);
    y += lineheight;
    if ( InBayTypeName == null || InBayTypeName == baytype.name ) {
      for ( j = 0; j < paneltypes.length; j++ ) {
        paneltype = paneltypes[j];
        messageline = document.createElement("div");
        messageline.className = "LineElement LineElementOdd";
        messageline.id = baytype.listnumber + " " + paneltype.name;
        messageline.dataPanelType = paneltype;
        messageline.dataBayType = baytype;
        messageline.dataType = "Panel";
        messageline.ondragstart = function(event) { CBPanelTypeDragStart(event); }
        messageline.ondragend   = function(event) { CBPanelTypeDragEnd(event); }
        messageline.draggable = true;
        messageline.innerHTML = paneltype.name;

        messageline.style.top = y;
        messageline.style.cursor = "pointer";
        messageline.style.paddingLeft = "10px";
        displayarea.appendChild(messageline);
        y += lineheight;
      }   
      y += 5;
    }
  } 
}

