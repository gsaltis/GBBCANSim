/*****************************************************************************!
 * Function : DeviceDefFindByName 
 *****************************************************************************/
function
DeviceDefFindByName(InDeviceDefName)
{
  var i;
  for (i = 0; i < MainDeviceDefs.length; i++) {
    if ( MainDeviceDefs[i].name == InDeviceDefName ) {
      return MainDeviceDefs[i];
    }
  }
  return null;
}

