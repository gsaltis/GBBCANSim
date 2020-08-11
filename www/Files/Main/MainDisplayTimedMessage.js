/*****************************************************************************!
 * Function : MainDisplayTimedMessage
 *****************************************************************************/
function
MainDisplayTimedMessage(InMessage, InTimeout)
{
  var					messageArea;

  MainMessageTimerID = setInterval(MainMessageTimeout, InTimeout * 1000);
  messageArea = document.getElementById("MessageLine");
  messageArea.innerHTML = InMessage;
}

