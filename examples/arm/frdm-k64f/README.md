## Using the VSCP-Framework on an NXP FRDM-K64F
First of all many thanks goes to Iain Galloway from NXP Semiconductors, which provided the FRDM-K64F hardware to integrate the VSCP-Framework on it.
Additional in this example a FRDM-CAN-VSCP shield is used, which contains an CAN transceiver and an EEPROM. This was created by [Angus Galloway](https://github.com/gallowaa), therefore many thanks as well.

![board](https://github.com/BlueAndi/vscp-framework/blob/master/examples/arm/frdm-k64f/doc/FRDM-K64F-ANGLE.jpg)

### Hardware Details
| Board:  | FRDM-K64F |
| Device: | MK64F12 |
| Core Type: | Cortex-M4F |
| Max. Freq.: | 120 MHz |
| Flash: | 1024 kByte |
| RAM: | 256 kByte |

### Hardware Overview
![frdm_k64_hw_overview](https://github.com/BlueAndi/vscp-framework/blob/master/examples/arm/frdm-k64f/doc/frdm_k64f_hw_overview.png)

## Getting Started
I read the [Getting Started](https://www.nxp.com/products/microcontrollers-and-processors/arm-based-processors-and-mcus/kinetis-cortex-m-mcus/k-seriesperformancem4/k2x-usb/freedom-development-platform-for-kinetis-k64-k63-and-k24-mcus:FRDM-K64F?tab=In-Depth_Tab) guide on the NXP homepage, which helped me to get a first impression about the hardware and how to get a "hello world" running.

## Software
Original the microcontroller was introduced with the Kinetis Design Studio IDE, by Freescale Semiconductor. On December 7, 2015, Freescale Semiconductor merged with NXP Semiconductors. The Kinetis Design Studio IDE could still be used, but the MCUXpresso IDE is the successor. Therefore I decided to use MCUXpresso IDE and its tools around.

A short overview about the software items:
* **MCUXpresso IDE v10.0.2** - The integrated development environment, based on Eclipse. 
* **MCUXpresso SDK for FRDM-K64F V2.2** - The software development kit contains all meta information about the MCU and board.
* **MCUXpresso Config Tools V3.0** - Configuration tool of the peripheral, pins, clocks, etc.

This [short video introduction](https://www.nxp.com/video/:MCUXPRESSO-SW-TOOLS-OVERVIEW) about the software items shows how they are working together.

### Short story about the software installation and first setup
The following steps showing what I did to create a project and adapt it step by step to get the VSCP-Framework running.

#### Get the software items and install them
* Download and install the [MCUXpresso IDE](https://www.nxp.com/support/developer-resources/run-time-software/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE?tab=Design_Tools_Tab).
* Download and extract the [MCUXpresso SDK for FRDM-K64F](https://mcuxpresso.nxp.com/en/license?hash=7352a00d5166ebf9cc83f1442c02d98e&hash_download=1).
* Download and install [MCUXpresso Config Tools](https://www.nxp.com/support/developer-resources/run-time-software/mcuxpresso-software-and-tools/mcuxpresso-config-tools:MCUXpresso-Config-Tools?tab=Design_Tools_Tab).

#### Create a new project
* Start the MCUXpresso IDE.
* Drag'n drop the SDK as .zip file to the "Installed SDKs" window.
* Set "Insert spaces for tabs" in *Windows -> Preferences -> General -> Editors -> Text Editors*
* Create a new MCUXpresso IDE C/C++ project.
* Choose the FRDM-K64F board.
* Click on "Next".
* Configure the project
  * Enter a project name, but don't use '-' character, because it will cause problems in the MCUXpresso Config Tools later. In this example we use "vscp_frdm_k64f".
  * Disable "Use default location" and enter the path to the "<vscp-framework>/examples/arm/frdm-k64f" folder.
  * Select default board files.
  * Additional select the flexcan and pit driver.
  * Disable "Enable semihost".
* Click on "Next".
* Disable Redirect SDK "PRINTF" to C library "print".
* Click on "Finish".

#### Add the VSCP-Framework files
In the MCUXpresso IDE:
* Create a vscp_user source folder in the project and copy the templates from "<vscp-framework>/vscp/templates" to it.
* Link the "<vscp-framework>/vscp" folder into the project (*file -> new -> folder*).
  * Use "Link to alternate location" and enter "${PROJECT_LOC}\\..\\..\\..\\..\\..\\vscp-framework\\vscp"
  * Set a resource filter for folder "events". This will include only the sources inside the "vscp" folder and "events" folder.
* Set "vscp" folder as source folder in *project properties -> C/C++ General -> Paths and Symbols -> Source Location*.
* Add the include paths in *project properties -> C/C++ Build -> Settings -> Includes*:
  * "${workspace_loc:/${ProjName}/vscp}"
  * "${workspace_loc:/${ProjName}/vscp/events}"
  * "${workspace_loc:/${ProjName}/vscp_user}"

#### Overwrite board configuration
The initial board configuration was created within the MCUXpresso IDE. But further configuration shall take place in the MCUXpresso Config Tool. Unfortunately, the MCUXpresso IDE doesn't create a configuration file for the MCUXpresso Config Tool. Therefore it has to be created separately. And unfortunately, you can not create it directly in the project, created via MCUXpresso IDE.

The workaround is to create a MCUXpresso Config Tool configuration in a different location and copy it afterwards (including the generated files) to the project:
* Start the MCUXpresso Config Tools.
  * Select the SDK folder.
  * Click on "Next".
  * Select "New FRDM-K64F configuration".
  * Click on "Finish".
  * Configure as you like and store the configuration somewhere with the project name "vscp_frdm_k64f".
* Close MCUXpresso Config Tool.
* Copy the MCUXpresso Config Tool configuration to the "vscp_frdm_k64f" project folder.
* The new "board" folder replaces the "frdmk64f" folder, therefore delete it.
* Delete the "main.c" in the "source" folder as well.
* Back to the MCUXpresso IDE.
  * Set the "board" folder as source folder in *project properties -> C/C++ General -> Paths and Symbols -> Source Location*.
  * Adapt the "source/vscp_frdm_k64f.c" to your needs.

### Setup of VSCP-Framework inside the project

![simple_vscp-framework_processing](https://github.com/BlueAndi/vscp-framework/blob/master/examples/arm/frdm-k64f/doc/simple_vscp-framework_processing.png)

#### Initialization of the VSCP framework
Adapt the "source/vscp_frdm_k64f.c" to initialize the VSCP framework by calling "vscp_core_init()".

#### Processing of the VSCP framework
The processing is done by calling "vscp_core_process()" in a constant cyclic period of 100 ms.
The PIT (Periodic Interrupt Timer) driver supports this and we will generate a 1 ms tick.

The problem we are facing now is how to get PIT support?
It looks like it is not possible to select in the MCUXpresso IDE, therefore I opened the MCUXpresso Config Tool and step to the project generator. There select the PIT driver and generate again.

Now the expectation is that the PIT driver was added, but this doesn't happen. A short look to the content description of the MCUXpresso Config Tool shows that the MCUXpresso IDE is not fully supported. :-(

Let's do it simple by just copy the PIT driver from "SDK_2.2_FRDM-K64F/devices/MK64F12/drivers/fsl_pit.*" to the "driver" folder.

To see how to initialize the PIT driver, just import the pit driver example from the SDK in the MCUXpresso IDE and use it accordingly.

To say the truth, I added also a simple driver for software timers, which are processed by the periodic interrupt timer. This makes it easier for the example. :-)

### Using a button to start the VSCP init sequence
We will use the SW2 button on the board for this. The "pin_mux.h" contains the necessary preprocessor defines, starting all with "BOARD_SW2_..." and note that the button is low active.

Code snipet:
```
/**
 * This function returns the current init button state.
 * Note, this state is not debounced!
 *
 * @return State of button
 * @retval FALSE	Button released
 * @retval TRUE		Button pressed
 */
static BOOL main_getInitButtonState(void)
{
	BOOL	result = FALSE;

	if (0 == (BOARD_SW2_GPIO->PDIR & (1 << BOARD_SW2_GPIO_PIN)))
	{
		result = TRUE;
	}

	return result;
}
```

Use the periodic interrupt timer to debounce it, just to go sure and indepedent whether there is a low pass filter in the schematic. ;-)

#### Let's blink
The onboard red LED shall be used to blink the *hello world* of VSCP. The "board.h" contains all necessary preprocessor defines, starting with "LED_RED_...".

#### Adapt the transport layer to CAN
The next driver we need is the FlexCAN driver. Let's do it simple again by just copy the FlexCAN driver from "SDK_2.2_FRDM-K64F/devices/MK64F12/drivers/fsl_flexcan.*" to the "driver" folder.

To see how to use the FlexCAN driver, just import the FlexCAN driver example from the SDK in the MCUXpresso IDE and adapt the "vscp_tp_adapter.c" accordingly.

The K64F supports 16 CAN message buffers. In this example 15 of them shall be used for receiving and one for transmitting.

#### Use the EEPROM as persistent storage

... to be continued ...

## Useful links

* [OPENSDA: OpenSDA Serial and Debug Adapter](https://www.nxp.com/support/developer-resources/run-time-software/kinetis-developer-resources/ides-for-kinetis-mcus/opensda-serial-and-debug-adapter:OPENSDA#FRDM-K64F).
* [ARMmbed/DAPLink github project](https://github.com/ARMmbed/DAPLink).
* [ARMmbed Platform FRDM-K64F](https://os.mbed.com/platforms/FRDM-K64F)