/*****************************************************************************!
 * Function : MainDisplayMessageColor
 *****************************************************************************/
function
MainDisplayMessageColor
(InMessage, InColor)
{
  var					messageArewa;
  
  messageArea = document.getElementById("MessageLine");
  messageArea.style.color = InColor;
  MainDisplayTimedMessage(InMessage, MAIN_DEFAULT_MESSAGE_TIME);
}
