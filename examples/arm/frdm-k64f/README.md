##Board

![board](https://github.com/BlueAndi/vscp-framework/blob/master/examples/arm/frdm-k64f/doc/FRDM-K64F-ANGLE.jpg)

Hardware Details

| Board:  | FRDM-K64F |
| Device: | MK64F12 |
| Core Type | Cortex-M4F |
| Max. Freq. | 120 MHz |
| Flash: | 1024 kByte |
| RAM: | 256 kByte |

##Installation

Follow the "getting started" guide on the NXP homepage, which is very useful:
https://www.nxp.com/products/microcontrollers-and-processors/arm-based-processors-and-mcus/kinetis-cortex-m-mcus/k-seriesperformancem4/k2x-usb/freedom-development-platform-for-kinetis-k64-k63-and-k24-mcus:FRDM-K64F?tab=In-Depth_Tab

Original the microcontroller was introduced with the Kinetis Design Studio IDE.
In the meantime the MCUXpresso IDE is the successor of the Kinetis Design Studio.
The Kinetis Design Studio was introduced original by Freescale Semiconductor.
On December 7, 2015, Freescale Semiconductor merged with NXP Semiconductors.

A short overview about the software:
* MCUXpresso IDE - The integrated development environment, based on Eclipse. 
* MCUXpresso SDK for FRDM-K64F - The software development kit contains all meta information about the MCU and board.
* MCUXpresso Config Tools - Configuration tool of the peripheral, pins, clocks, etc.

NXP provides a short video introduction about the software and how they are working together here:
https://www.nxp.com/video/:MCUXPRESSO-SW-TOOLS-OVERVIEW

* Serial port driver

##Start vom scratch
* Download and install the MCUXpresso IDE.
* Download and extract the MCUXpresso SDK for FRDM-K64F.
* Download and install MCUXpresso Config Tools.
* Start the MCUXpresso IDE.
** Drag'n drop the SDK as .zip file to the "Installed SDKs" window.
** Create a new MCUXpresso IDE C/C++ project.
** Choose the FRDM-K64F board.
** Click on "Next".
** Configure the project
*** Enter a project name, but don't use '-' character, because it will cause problems in the MCUXpresso Config Tools  later. In this example we use vscp_frdm_k64f.
*** Disable "Use default location" and enter the path to the vscp-framework/examples/arm/frdm-k64f folder.
*** Select default board files.
*** Additional select the flexcan driver.
*** Disable "Enable semihost".
** Click on "Next".
** Disable Redirect SDK "PRINTF" to C library "print".
** Click on "Finish".
** Create a vscp_user source folder and copy the templates to it.
** Link the vscp folder into the project (file -> new -> folder).
*** Use "Link to alternate location" and enter: ${PROJECT_LOC}\..\..\..\..\..\vscp-framework\vscp
*** Set a resource filter for folder "events".
** Set vscp as source folder in project properties -> C/C++ General -> Paths and Symbols -> Source Location
** Add the include paths in project properties -> C/C++ Build -> Settings -> Includes:
*** "${workspace_loc:/${ProjName}/vscp}"
*** "${workspace_loc:/${ProjName}/vscp/events}"
*** "${workspace_loc:/${ProjName}/vscp_user}"
* Start the MCUXpresso Config Tools.
** Select the SDK folder.
** Click on "Next".
** Select "New FRDM-K64F configuratin".
** Click on "Finish".
** Configure as you like and store the configuration somewhere.
** Replace in all files of the generated project the project name with vscp_frdm_k64f.
** Copy the configuration to the vscp_frdm_k64f folder.
** The new board folder replaces the frdmk64f folder, therefore delete it.
** Delete the main.c in the source folder as well.
* Back to the MCUXpresso IDE.
** Set board folder as source folder in project properties -> C/C++ General -> Paths and Symbols -> Source Location
** Adapt the source/vscp_frdm_k64f.c to your needs.
