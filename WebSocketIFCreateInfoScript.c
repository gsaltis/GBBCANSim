/*****************************************************************************!
 * Function : WebSocketIFCreateInfoScript
 *****************************************************************************/
void
WebSocketIFCreateInfoScript
()
{
  string                                address;
  int                                   fd;
  struct                                ifreq ifr;
  FILE*                                 file;
  struct sockaddr_in*                   sa;
  int                                   i, j;
  int					interfacesSize;
  char                                  b;
  char*					interfaces[] = { "eth0", "wlan0" };
  bool					found;
  string				interface;

  found = false;
  interfacesSize = sizeof(interfaces) / sizeof(char*);

  printf("Create HTML WebSocket Script ");
  fflush(stdout);
  //! We only want to do this when we have a value 192. address
  //  So we loop until we do or we eventually give up
  for (i = 0; i < WEBSOCKET_IF_MAX_ADDRESS_TRIES && !found; i++ ) { 
    for ( j = 0 ; j < interfacesSize && !found ; j++ ) {
      fd = socket(AF_INET, SOCK_DGRAM, 0);
      printf(".");
      fflush(stdout); 
      /* I want to get an IPv4 IP address */
      ifr.ifr_addr.sa_family = AF_INET;

      interface = interfaces[j];    
      /* I want IP address attached to "eth0" */
      strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);
    
      ioctl(fd, SIOCGIFADDR, &ifr);
      close(fd);
      sa = (struct sockaddr_in*)&ifr.ifr_addr;
      b = (char)(sa->sin_addr.s_addr & 0xFF);
  
      //! Check that we have a at least a C Address
      if ( b & 0xC0 ) {
        address = StringCopy(inet_ntoa(sa->sin_addr));
        file = fopen("www/websocketinfo.js", "wb");
 	if ( NULL == file ) {
	  return;
 	}
        fprintf(file, "var WebSocketIFAddress = \"%s\";\n", address);
        fprintf(file, "var WebSocketIFPort = \"%s\";\n", s_websocket_port);
        fclose(file);
		WebSocketIFAddress = StringCopy(address);
        FreeMemory(address);
		found = true;
      }
      if ( !found ) {
        //! We don't so sleep and try again
        sleep(WEBSOCKET_IF_ADDRESS_WAIT_PERIOD);
      }
    }
  }
  printf(" %screated\n", found ? "" : "not ");
}

