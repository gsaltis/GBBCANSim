/*****************************************************************************!
 * Function : DeviceDefSortRegsByGroupName
 *****************************************************************************/
function
DeviceDefSortRegsByGroupName
(InDevice)
{
   InDevice.registers.sort(function(a, b) { var n = a.groupsort.localeCompare(b.groupsort); 
					    if ( n == 0 ) {
					      n = a.displaylabel.localeCompare(b.displaylabel);
 					    }
					    return n;
					  });
}

