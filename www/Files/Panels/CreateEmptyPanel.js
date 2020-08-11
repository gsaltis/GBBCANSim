/*****************************************************************************!
 * Function : CreateEmptyPanel
 *****************************************************************************/
function
CreateEmptyPanel(InBay, InPanelIndex, InX, InY, InPanelWidth, InPanelHeight)
{
  var				panel;

  panel = document.createElement("div");
  panel.className = "GeneralPane EmptyPanel";

  panel.id = "Empty Bay " + InBay.dataBay.index + " Panel " + InPanelIndex;
  panel.style.width = InPanelWidth;
  panel.style.height = InPanelHeight;
  panel.style.top = InY;
  panel.style.left = InX;
  panel.dataIndex = InPanelIndex;
  panel.dataBay = InBay.dataBay;
  panel.dataType = "EmptyPanel";
  panel.ondrop = function(event) { CBPanelDrop(event); }
  panel.ondragover = function(event) { CBPanelDragOver(event); }

  InBay.appendChild(panel);
}

