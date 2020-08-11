/*****************************************************************************!
 * Function : BayFindRegisterValueByValueType
 *****************************************************************************/
function
BayFindRegisterValueByValueType
(InBay, InValueType)
{
  var					i;

  if ( null == InBay || null == InBay.device.registers ) {
    return null;
  }

  for (i = 0; i < InBay.device.registers.length; i++) {
    if ( InBay.device.registers[i].valuetype == InValueType ) {
      return InBay.device.registers[i];
    }
  }
  return null;
}

