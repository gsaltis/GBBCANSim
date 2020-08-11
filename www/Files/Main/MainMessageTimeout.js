/*****************************************************************************!
 * Function : MainMessageTimeout
 *****************************************************************************/
function
MainMessageTimeout()
{
  clearInterval(MainMessageTimerID)
  document.getElementById("MessageLine").innerHTML = "";
  MainMessageTimerID = null;
}

