/*****************************************************************************!
 * Function : DeviceDefFindByDescription 
 *****************************************************************************/
function
DeviceDefFindByDescription(InDeviceDefDescription)
{
  var i;
  for (i = 0; i < MainDeviceDefs.length; i++) {
    if ( MainDeviceDefs[i].description == InDeviceDefDescription ) {
      return MainDeviceDefs[i];
    }
  }
  return null;
}

