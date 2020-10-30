/*****************************************************************************!
 * Function : WebSocketIFHandleResponsePacket
 *****************************************************************************/
function
WebSocketIFHandleResponsePacket(InPacket)
{
  if ( InPacket.responseid != "OK" ) {
    MainDisplayMessage(InPacket.responsemessage);
    return;
  }
  if ( InPacket.type == "resbaytypes" ) {
    WebSocketIFHandleResponseBayTypes(InPacket.body.baytypes);
    WebSocketIFSendSimpleRequest("getpaneltypes");
  } else if ( InPacket.type == "respaneltypes" ) {
    WebSocketIFHandleResponsePanelTypes(InPacket.body.paneltypes);
    WebSocketIFSendSimpleRequest("getdevicedefs");
  } else if ( InPacket.type == "resdevicedefs" ) {
    WebSocketIFHandleDeviceDefs(InPacket.body.devicedefs);
    WebSocketIFSendDeviceDefRegRequestStart();
  } else if ( InPacket.type == "resdeviceregs" ) {
    WebSocketIFHandleDeviceRegs(InPacket.body);
  } else if ( InPacket.type == "resbays" ) {
    WebSocketIFHandleBays(InPacket.body.bays);
    MainInitializeDisplay();
  } else if ( InPacket.type == "resbayregvalues") {
    WebSocketIFHandleBayRegValues(InPacket.body.bayregvalues);
  } else if ( InPacket.type == "respanelregvalues") {
    WebSocketIFHandlePanelRegValues(InPacket.body.panelregvalues);
  } else if ( InPacket.type == "resremovepanel" ) {
    WebSocketIFHandleRemovePanelResponse(InPacket.responseid, InPacket.responsemessage, InPacket.body.bayindex, InPacket.body.panelindex);
  } else if ( InPacket.type == "resaddbay") {
	WebSocketIFSendSimpleRequest("getbays");
  }
}

