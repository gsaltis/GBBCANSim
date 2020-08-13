/*****************************************************************************!
 * Function : ClientUserInputHandleHelp
 *****************************************************************************/
void
ClientUserInputHandleHelp
(StringList* InParameters, string InCommandString)
{
  printf_safe("\n\nCommands: \n\n");
  printf_safe("%sADD DEVICE <device-name> <quantity>                             %sAdd a device to the simulator\n", ColorRed, ColorReset);
  printf_safe("%sADD BAY <bayindex>                                              %sAdd a bay to a system\n", ColorRed, ColorReset);
  printf_safe("%sADD PANEL <bayindex> [all | <panelindex>]                       %sAdd a panel to a bay\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sREMOVE BAY <bayindex>                                           %sRemove a bay from a system\n", ColorRed, ColorReset);
  printf_safe("%sREMOVE PANEL <bayindex> <panelindex>                            %sRemove a panel from a bay\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sLIST DEVICES                        %s to list devices\n", ColorRed, ColorReset);
  printf_safe("%sLIST DEVICETYPES <device-name>      %s to list all available devices types ('type' display all information for a specific type\n", ColorRed, ColorReset);
  printf_safe("%sLIST REGISTERS <device-name>        %s to list all registers ('type' for registers for specific type\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sCLEAR DEVICES                              %s to clear all regsiters in all devices\n", ColorRed, ColorReset);
  printf_safe("%sCLEAR DEVICE <device-name> <device-number> %s to clear all registers in a specific device\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sSET  %s                                                                          to get prompted for the input to set a register value in a device \n", ColorRed, ColorReset);
  printf_safe("%sSET <device-name> <device-number> <group-name> <abbreviation> <new-value> %s     to set a register value in a device \n", ColorRed, ColorReset);
  printf_safe("%sSETN bayindex panelindex registernumber{:registernumber} value  %s             to set a register value in a device using register number \n", ColorRed, ColorReset);
  printf_safe("%sINCREMENT <device-name> <device-number> <group-name> <new-value> <increment>  %s to set a series of registers, increment can be 0 \n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sSHOW DEVICE <device-name> <device-number>  %s            to show the information for a device\n", ColorRed, ColorReset);
  printf_safe("%sSHOW GROUP <device-name> <device-number> <group-name> %s to show the information for a group\n", ColorRed, ColorReset);
  printf_safe("%sSHOW ALL  %s                                             to show all registers for all devices\n", ColorRed, ColorReset);
  printf_safe("%sSHOW MESSAGES  %s                                        to show all messages for all devices\n", ColorRed, ColorReset);
  printf_safe("%sSHOW BAY <bay-index>	%s				  to show the information for a Bay\n", ColorRed, ColorReset);
  printf_safe("%sSHOW CAN%s                                               to show the CAN Interface status\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sOUT <filespec>  %s to monitor CAN traffic to an output file \n", ColorRed, ColorReset);
  printf_safe("%sWATCH  %s          to watch for REGISTER ADDRESS PROTOCOL or OFF in incoming/outgoing CAN messages\n", ColorRed, ColorReset);
  printf_safe("%sPING  %s           to send message to SMDUEwa\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%s@<filename>  %s    to execute a script file\n", ColorRed, ColorReset);
  printf_safe("%sDELAY <seconds> %s to delay for a number of seconds \n", ColorRed, ColorReset);
  printf_safe("%sPAUSE <string>  %s to wait for user input \n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sBAYS            						%s List Bays\n", ColorRed, ColorReset);
  printf_safe("%sPANELS          						%s List Panels\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sCLOSE <index>   %s Close a WWW connection\n", ColorRed, ColorReset);
  printf_safe("%sCONNECTIONS     %s List WWW Connections\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sOPTION SET {MONITORWEB} {ON|OFF} %s Monitor WEB Input requests\n", ColorRed, ColorReset);
  printf_safe("%s           {MONITORCAN} {ON|OFF} %s Monitor CAN Input requests\n", ColorRed, ColorReset);
  printf_safe("%sSTATUS%s\n", ColorRed, ColorReset);
  printf_safe("%sEXIT%s             to exit\n\n\n", ColorRed, ColorReset);
}
