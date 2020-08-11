/*****************************************************************************!
 * Function : BayEditAreaHide
 *****************************************************************************/
function
BayEditAreaHide
(InBay)
{
  InBay.dataEdittingMode = "None";
  MainHideBlocker();
  BayLowerZIndex(InBay);
  InBay.style.width = InBay.dataBayWidth;
}

