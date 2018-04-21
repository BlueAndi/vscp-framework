# VSCP L1 Framework

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)
[![Release](https://img.shields.io/github/release/BlueAndi/vscp-framework.svg)](https://github.com/BlueAndi/vscp-framework/releases)
[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/4825.svg)](https://scan.coverity.com/projects/4825?tab=overview)
[![Build Status](https://travis-ci.org/BlueAndi/vscp-framework.svg?branch=master)](https://travis-ci.org/BlueAndi/vscp-framework)

1. [VSCP](https://github.com/BlueAndi/vscp-framework#vscp)
2. [Framework](https://github.com/BlueAndi/vscp-framework#framework)
3. [Structure](https://github.com/BlueAndi/vscp-framework#structure)
4. [Getting started](https://github.com/BlueAndi/vscp-framework#getting-started)
5. [Bootloader](https://github.com/BlueAndi/vscp-framework/tree/master/vscp/bootloader)
6. [Typical infrastructure](https://github.com/BlueAndi/vscp-framework#typical-infrastructure)
7. [Issues, Ideas and bugs](https://github.com/BlueAndi/vscp-framework#issues-ideas-and-bugs)
8. [License](https://github.com/BlueAndi/vscp-framework#license)

## VSCP

<img src="https://github.com/grodansparadis/vscp_logo/raw/master/vscp_logo.jpg" width="200px" alt="Logo" />

The Very Simple Control Protocol (VSCP), an open and free protocol for IoT/m2m automation tasks.

More information can be found on the main site http://www.vscp.org

## Framework
The VSCP software framework for level 1 devices provides several layers according to the [VSCP specification](http://www.vscp.org/docs/vscpspec/doku.php).

### Core
![core-diagram](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/doxfiles/vscp_modules.png)

* The core functionality which has a built-in state machine to handle different use cases of the protocol and etc. (vscp\_core.[ch]). Right now it supports every mandatory event and some minor optional ones.
* The decision matrix is handled separately (vscp\_dm.[ch]). It contains the standard decision matrix, as described in the VSCP specification and contains an additional extension.
* The decision matrix next generation is supported too (vscp\_dm\_ng.[ch]). It eliminates the limitations of the standard decision matrix, incl. its extension.
* VSCP needs some mandatory persistent data, which can be modified during run time. This kind of data is in the persistent storage handled (vscp\_ps.[ch]).
* The device specific data is handled separatly (vscp\_dev\_data.[ch]). You can decide whether this data shall be constant and configured during compile time or its loaded from persistent storage and could be modified during run time.
* The transport layer has the possibility to loop events back (vscp_transport.[ch]).
    This can be configured for each data (vscp\_dev\_data_config.[ch]), except the firmware version.
* Functionality can be configured for your needs (vscp_config.[ch]).
* Some utility functions are separated (vscp\_util.[ch]) and used by different core modules or are maybe interested for the application too.
* Log functionaly is provided for debugging purposes (vscp\_logger.[ch]).

The framework is independent of the hardware and the used operating system. To achieve independence all of the following
layers have to be adapted to the system. This is supported by templates, which contains all necessary functions with nearly empty
bodys.

The following modules have to be adapted for your needs, because it depends on the hardware, the operating system or
how VSCP is integrated into your software:
* Transport adapter (vscp\_tp\_adapter.c)
* Timer driver (vscp\_timer.c)
* Persistent memory access driver (vscp\_ps\_access.c)
* Action module, used by the decision matrix (standard, extension and next generation) (vscp\_action.c)
* Application register access (vscp\_app\_reg.c)
* Callout functions, lamp handling and etc. (vscp\_portable.c)

Templates exists for all of them, which makes it much easier to adapt it and less time. See in the templates folder.

### Event abstraction
![core-diagram](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/doxfiles/vscp_event_modules.png)

Using only the core, you have to assemble the VSCP events by yourself. If you want to deal only with parameter, which are
VSCP independent, use the next upper layer, the event abstraction modules.

### Configuration parameters

#### Common

The following configuration parameters can be enable/disable/set in the
```
vscp_config_overwrite.h
```

| Feature switch | Default | Description |
| :------------: | :-----: | :---------: |
| <sub>VSCP\_CONFIG\_ENABLE\_LOGGER<sub> | <sub>disabled<sub> | <sub>Enable log functionality (CLASS1.Log). Use the macros in vscp\_logger.h to send log messages.<sub> |
| <sub>VSCP\_CONFIG\_SILENT\_NODE<sub> | <sub>disabled<sub> | <sub>Silent node configuration, which is used for e. g. RS-485 connections. This type of nodes only listen to traffic before they get initialized by a host. In this case the nickname discovery process is not started for a node when it is powered up for the first time. This type on node instead starts to listen for the CLASS1.PROTOCOL, Type=23 (GUID drop nickname-ID / reset device.) event. When this series of events is received and the GUID is the same as for the module the module starts the nickname discovery procedure as of above.<sub> |
| <sub>VSCP\_CONFIG\_HARD\_CODED\_NODE<sub> | <sub>disabled<sub> | <sub>Hard-coded node (fixed nickname id)<sub> |
| <sub>VSCP\_CONFIG\_HEARTBEAT\_SUPPORT\_SEGMENT<sub> | <sub>disabled<sub> | <sub>Enable segment controller heartbeat support for nodes.<sub> |
| <sub>VSCP\_CONFIG\_HEARTBEAT\_NODE<sub> | <sub>enabled<sub> | <sub>Enable sending node heartbeat (mandatory since 2015-09-10).<sub> |
| <sub>VSCP\_CONFIG\_IDLE\_CALLOUT<sub> | <sub>disabled<sub> | <sub>Enable idle callout. If VSCP stops working and enters idle state, the application will be notified.<sub> |
| <sub>VSCP\_CONFIG\_ERROR\_CALLOUT<sub> | <sub>disabled<sub> | <sub>Enable error callout. If VSCP stops working and enters error state, the application will be notified.<sub> |
| <sub>VSCP\_CONFIG\_BOOT\_LOADER\_SUPPORTED<sub> | <sub>disabled<sub> | <sub>Enable boot loader support.<sub> |
| <sub>VSCP\_CONFIG\_ENABLE\_DM<sub> | <sub>enabled<sub> | <sub>Enable decision matrix (standard).<sub> |
| <sub>VSCP\_CONFIG\_DM\_PAGED\_FEATURE<sub> | <sub>disabled<sub> | <sub>Enable decision matrix special paged feature.<sub> |
| <sub>VSCP\_CONFIG\_ENABLE\_DM\_EXTENSION<sub> | <sub>disabled<sub> | <sub>Enable the decision matrix extension to be able to compare to a configureable zone/sub-zone and event parameters.<sub> |
| <sub>VSCP\_CONFIG\_ENABLE\_DM\_NEXT\_GENERATION<sub> | <sub>disabled<sub> | <sub>Enable the decision matrix next generation.<sub> |
| <sub>VSCP\_CONFIG\_ENABLE\_LOOPBACK<sub> | <sub>disabled<sub> | <sub>Enable a loopback for all sent VSCP events. This feature is interesting to invoke decision matrix actions by own sent VSCP events.<sub> |
| <sub>VSCP\_CONFIG\_ENABLE\_SEGMENT\_TIME\_CALLOUT<sub> | <sub>disabled<sub> | <sub>Enable a time update callout for every received segment master heartbeat, in case the event contains a new time since epoch.<sub> |
| <sub>VSCP\_CONFIG\_PROTOCOL\_EVENT\_NOTIFICATION<sub> | <sub>disabled<sub> | <sub>Usually the core handles all protocol class events and they are not forwarded to the application. Enable this to forward the events as well. If application handles the event, the core won't handle it. Attention: Handling events which the core is waiting for can cause bad behaviour.<sub> |

| Parameter | Default | Description |
| :-------: |:------: | :---------: |
| <sub>VSCP\_CONFIG\_NODE\_SEGMENT\_INIT\_TIMEOUT<sub> | <sub>5000<sub> | <sub>Timeout in ms for the node segment initialization.<sub> |
| <sub>VSCP\_CONFIG\_PROBE\_ACK\_TIMEOUT<sub> | <sub>2000<sub> | <sub>Timeout in ms for the probe acknowledge.<sub> |
| <sub>VSCP\_CONFIG\_MULTI\_MSG\_TIMEOUT<sub> | <sub>1000<sub> | <sub>Timeout in ms to observe multi-message handling.<sub> |
| <sub>VSCP\_CONFIG\_HEARTBEAT\_NODE\_PERIOD<sub> | <sub>30000<sub> | <sub>Node heartbeat period in ms (recommended 30s - 60s).<sub> |
| <sub>VSCP\_CONFIG\_DM\_PAGE<sub> |<sub>1<sub> | <sub>Decision matrix location: First page of the decision matrix.<sub> |
| <sub>VSCP\_CONFIG\_DM\_OFFSET<sub> | <sub>0<sub> | <sub>Decision matrix location: Offset in the first page of the decision matrix.<sub> |
| <sub>VSCP\_CONFIG\_DM\_ROWS<sub> | <sub>10<sub> | <sub>Number of decision matrix rows.<sub> |
| <sub>VSCP\_CONFIG\_DM\_NG\_PAGE<sub> | <sub>2<sub> | <sub>Decision matrix next generation: Location in the application register space. Note that the dm ng always starts at the begin of the page! This design decision was just for simplification, nothing else.<sub> |
| <sub>VSCP\_CONFIG\_DM\_NG\_RULE\_SET\_SIZE<sub> | <sub>80<sub> | <sub>Decision matrix next generation: Maximum size in bytes of a rule set.<sub> |
| <sub>VSCP\_CONFIG\_LOOPBACK\_STORAGE\_NUM<sub> | <sub>4<sub> | <sub>Number of messages in the loopback cyclic buffer. Note, that if you want to store up to 3 events, you have to configure 4, because of the technical implementation of the cyclic buffer.<sub> |

#### Device data

The following device data can be enable/disable/set in the
```
vscp_dev_data_config_overwrite.h
```

| Feature switch | Default | Description |
| :------------: | :-----: | :---------: |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_FAMILY\_CODE<sub> | <sub>enabled<sub> | <sub>Enable the support of the family code.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_GUID\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the GUID from persistent storage.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_GUID\_STORAGE\_EXT<sub> | <sub>disabled<sub> | <sub>Enable this define to load the GUID from external storage, e.g. from MCU GUID.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_NODE\_ZONE\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the node zone from persistent storage.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_NODE\_SUB\_ZONE\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the node sub-zone from persistent storage.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_MANUFACTURER\_DEV\_ID\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the manufacturer device id from persistent storage.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_MANUFACTURER\_SUB\_DEV\_ID\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the manufacturer sub device id from persistent storage.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_MDF\_URL\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the MDF URL from persistent storage.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_STD\_DEV\_FAMILY\_CODE\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the standard device family code from persistent storage. Note, that the feature family code has to be enabled, otherwise this define won't be considered.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_STD\_DEV\_TYPE\_STORAGE\_PS<sub> | <sub>disabled<sub> | <sub>Enable this define to load the standard device type from persistent storage. Note, that the feature family code has to be enabled, otherwise this define won't be considered.<sub> |

| Parameter | Default | Description |
| :-------: |:------: | :---------: |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_NODE\_GUID<sub> | <sub>00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00<sub> | <sub>Node GUID<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_NODE\_ZONE<sub> | <sub>0xff<sub> | <sub>Node zone. Note, 0xff means all zones.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_NODE\_SUB\_ZONE<sub> | <sub>0xff<sub> | <sub>Node sub-zone. Note, 0xff means all sub zones.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_MANUFACTURER\_ID<sub> | <sub>0x0000<sub> | <sub>Manufacturer id (0x0000 means unknown)<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_MANUFACTURER\_DEVICE\_ID<sub> | <sub>0x00000000<sub> | <sub>Manufacturer device id<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_MANUFACTURER\_SUB\_DEVICE\_ID<sub> | <sub>0x00000000<sub> | <sub>Manufacturer sub device id<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_MDF\_URL<sub> | <sub>empty<sub> | <sub>MDF URL (only 32 characters are allowed!), without "http://" which is implied Note, the encoding is UTF-8!<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_VERSION\_MAJOR<sub> | <sub>0<sub> | <sub>Major firmware version (format: major.minor.sub-minor)<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_VERSION\_MINOR<sub> | <sub>1<sub> | <sub>Minor firmware version (format: major.minor.sub-minor)<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_VERSION\_SUB\_MINOR<sub> | <sub>0<sub> | <sub>Sub-minor firmware version (format: major.minor.sub-minor)<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_STANDARD\_DEVICE\_FAMILY\_CODE<sub> | <sub>0x00000000<sub> | <sub>Standard device family code.<sub> |
| <sub>VSCP\_DEV\_DATA\_CONFIG\_STANDARD\_DEVICE\_TYPE<sub> | <sub>0x00000000<sub> | <sub>Standard device type.<sub> |

## Structure

<pre>
+---common              (Common sourcecode, used for examples and projects)
|   +---avr             (Common sourcecode for Atmel AVR microcontrollers)
|   \---pc              (Common sourcecode for PC)
+---examples            (Examples which are showing how to use the VSCP framework)
|   +---arm             (Examples with ARM based microcontrollers)
|   +---avr             (Examples with Atmel AVR microcontrollers)
|   \---pc              (PC example for windows and linux)
+---projects            (Projects)
|   +---avr             (Projects with Atmel AVR microcontrollers)
|   \---pc              (PC projects)
+---tools               (General tools, used by examples and projects)
|   \---xslt            (XML transformation processor)
\---vscp                (VSCP framework)
    +---doc             (Documentation)
    |   +---doxfiles    (Doxygen related files)
    |   \---html        (Doxygen generated documentation in HTML)
    +---events          (These modules are using the VSCP core to send CLASS1 dedicated events)
    +---templates       (Templates of the files, which the user shall adapt to its needs)
    \---test            (Test of the VSCP framework)
</pre>

## Getting started

This part shows you how to get the VSCP framework working in a "minimal" way:

1. [Copy the VSCP framework to your project](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#copy-the-vscp-framework-to-your-project)
2. [Initialization of the VSCP framework](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#initialization-of-the-vscp-framework)
3. [Processing of the VSCP framework](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#processing-of-the-vscp-framework)
4. [VSCP framework timer](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#vscp-framework-timer)
5. [VSCP transport adaption](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#vscp-transport-adaption)
6. [Control the VSCP lamp](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#control-the-vscp-lamp)
7. [Connect the initialization button](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#connect-the-initialization-button)
8. [Persistent memory](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#persistent-memory)
9. [Ready to run](https://github.com/BlueAndi/vscp-framework/blob/master/README.md#ready-to-run)

### Copy the VSCP framework to your project

1. Copy or link the VSCP framework (./vscp and ./vscp/events) to your project.
2. Copy (!!do not link!!) all necessary template files (./vscp/templates) to your project. Recommended is a sub-directory "vscp_user".
3. Update your makefile or your project configuration.
 
### Initialization of the VSCP framework

The VSCP framework has to be initialized, before any function is used. This is simply done
by calling the function vscp\_core\_init() during start-up.

### Processing of the VSCP framework

The VSCP framework has to be called periodically to be able to react on incoming events.
Call the process routine vscp\_core\_process() in a constant cyclic period. The period should be
lower or equal than 100 ms, but depends on several factors, like the bitrate on the choosen
communication bus, the event load on the bus and etc.

The process routine handle all received VSCP events.

### VSCP framework timer

VSCP specifies several timing behaviour in different use cases. Therefore the framework needs some
timers to achieve it.

Because timer are usually hardware and software dependend, they have to be implemented by you for your needs.
Adapt the vscp_timer.c template file. Find all necessary informations in the module.

The timer handling can be processed by calling vscp\_timer\_process() in the same or in a different task as vscp\_core\_process() is called.
If a different task calls it, don't forget to make the timer functions reentrant!

Because the VSCP timer module needs to know the period of processing the timers, you have to call it with the
period time in ms as parameter, e.g. vscp\_timer\_process(1000);

Call the timer processing routine equal or lower than 1 s.

Note, that never call vscp\_process() with a lower period, than vscp\_timer\_process(). Because vscp\_process() reacts
on timer timeouts and vscp\_timer\_process() decrease only the timers, but doesn't do more.

### VSCP transport adaption

Now its time to connect the VSCP framework to the communication bus. This can be done by implementing the transport
adapter in the vscp\_tp\_adapter.c template file.

### Control the VSCP lamp

To see that something is happen on your embedded device, next step is to control the VSCP lamp (in most cases a LED).
Update the function vscp\_portable\_setLampState() in the vscp\_portable.c module.

### Connect the initialization button

According to the VSCP specification, every embedded device should have a button to start the segment initialization.
Hopefully you have one right now :-) and if it is pressed, call the function vscp\_core\_startNodeSegmentInit() in the
vscp\_core.c module.

### Persistent memory

It is important that the VSCP framework can store data in a persistent memory, e.g. an EEPROM. Implement in the
vscp\_ps\_access.c module the low level access to the persistent memory. Its quite easy, because only byte access
functions are used, so you have one read and one write function to adapt.

### Ready to run

Now the minimal sub set is done and your node hopefully starts up with a nickname discovery.

Have fun!

## Typical infrastructure

![typical-infrastructure](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/doxfiles/vscp_typical_infrastructure.png)

## Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
