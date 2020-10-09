/*****************************************************************************!
 * Function : ClientUserInputHandleHelp
 *****************************************************************************/
void
ClientUserInputHandleHelp
(StringList* InParameters, string InCommandString)
{
  printf_safe("\n\nCommands: \n\n");
  printf_safe("%sADD DEVICE <device-name> <quantity>                                           %sAdd a device to the simulator\n", ColorRed, ColorReset);
  printf_safe("%sADD BAY <bayindex>                                                            %sAdd a bay to a system\n", ColorRed, ColorReset);
  printf_safe("%sADD PANEL <bayindex> [all | <panelindex>]                                     %sAdd a panel to a bay\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sREMOVE BAY <bayindex>                                                         %sRemove a bay from a system\n", ColorRed, ColorReset);
  printf_safe("%sREMOVE PANEL <bayindex> <panelindex>                                          %sRemove a panel from a bay\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sLIST DEVICES                                                                  %sto list devices\n", ColorRed, ColorReset);
  printf_safe("%sLIST DEVICETYPES <device-name>                                                %sto list all available devices types ('type' display all information for a specific type\n", ColorRed, ColorReset);
  printf_safe("%sLIST REGISTERS <device-name>                                                  %sto list all registers ('type' for registers for specific type\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sCLEAR DEVICES                                                                 %sto clear all regsiters in all devices\n", ColorRed, ColorReset);
  printf_safe("%sCLEAR DEVICE <device-name> <device-number>                                    %sto clear all registers in a specific device\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sSET                                                                           %sto get prompted for the input to set a register value in a device \n", ColorRed, ColorReset);
  printf_safe("%sSET <device-name> <device-number> <group-name> <abbreviation> <new-value>     %sto set a register value in a device \n", ColorRed, ColorReset);
  printf_safe("%sSETN bayindex panelindex registernumber{:registernumber} value                %sto set a register value in a device using register number \n", ColorRed, ColorReset);
  printf_safe("%sINCREMENT <device-name> <device-number> <group-name> <new-value> <increment>  %sto set a series of registers, increment can be 0 \n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sSHOW DEVICE <device-name> <device-number>                                     %sto show the information for a device\n", ColorRed, ColorReset);
  printf_safe("%sSHOW GROUP <device-name> <device-number> <group-name>                         %sto show the information for a group\n", ColorRed, ColorReset);
  printf_safe("%sSHOW ALL                                                                      %sto show all registers for all devices\n", ColorRed, ColorReset);
  printf_safe("%sSHOW MESSAGES                                                                 %sto show all messages for all devices\n", ColorRed, ColorReset);
  printf_safe("%sSHOW BAY <bay-index>                                                          %sto show the information for a Bay\n", ColorRed, ColorReset);
  printf_safe("%sSHOW CAN                                                                      %sto show the CAN Interface status\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sBAYS                                                                          %sList Bays\n", ColorRed, ColorReset);
  printf_safe("%sPANELS                                                                        %sList Panels\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sCLOSE <index>                                                                 %sClose a WWW connection\n", ColorRed, ColorReset);
  printf_safe("%sCONNECTIONS                                                                   %sList WWW Connections\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sOPTION SET {MONITORWEB} {ON|OFF}                                              %sMonitor WEB Input requests\n", ColorRed, ColorReset);
  printf_safe("%s           {MONITORCAN} {ON|OFF}                                              %sMonitor CAN Input requests\n", ColorRed, ColorReset);
  printf_safe("%sSTATUS                                                                        %s\n", ColorRed, ColorReset);
  printf_safe("%sEXIT                                                                          %sto exit\n\n\n", ColorRed, ColorReset);
}
