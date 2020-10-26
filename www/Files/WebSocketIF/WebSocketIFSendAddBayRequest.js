/*****************************************************************************!
 * Function : WebSocketIFSendAddBayRequest
 *****************************************************************************/
function
WebSocketIFSendAddBayRequest
(InBay)
{
  var                                   request;

  request = {};
  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = 0;
  request.bayindex = InBay.dataBay.index;
  request.type = "addbay";
  request.body = {};
  request.body.bayindex = InBay.dataBay.index;
  request.body.bayinfo = {};
  request.body.bayinfo.panelcount = InBay.dataBay.type.panelcount;
  request.body.bayinfo.bayindex = InBay.dataBay.index;
  request.body.bayinfo.name = InBay.id;
  request.body.bayinfo.ipaddress = "127.0.0.1";
  request.body.bayinfo.type = InBay.dataBay.type.name;
  request.body.bayinfo.portnumber = 8001;
  request.body.bayinfo.canaddress = 0;
  request.body.bayinfo.listnumber = InBay.dataBay.type.listnumber;
  WebSocketIFSendGeneralRequest(request); 
}

