#VSCP L1 Framework

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)
[![Release](https://img.shields.io/github/release/BlueAndi/vscp-framework.svg)](https://github.com/BlueAndi/vscp-framework/releases)
[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/4825.svg)](https://scan.coverity.com/projects/4825?tab=overview)

1. [VSCP](https://github.com/BlueAndi/vscp-framework#vscp)
2. [Framework](https://github.com/BlueAndi/vscp-framework#framework)
3. [Structure](https://github.com/BlueAndi/vscp-framework#structure)
4. [Getting started](https://github.com/BlueAndi/vscp-framework#getting-started)
5. [Bootloader](https://github.com/BlueAndi/vscp-framework/tree/master/vscp/bootloader)
6. [Mailing list](https://github.com/BlueAndi/vscp-framework#mailing-list)
7. [Issues, Ideas and bugs](https://github.com/BlueAndi/vscp-framework#issues-ideas-and-bugs)
8. [License](https://github.com/BlueAndi/vscp-framework#license)

##VSCP

![VSCP logo](http://vscp.org/images/vscp_logo.jpg)

The Very Simple Control Protocol (VSCP), an open and free protocol for IoT/m2m automation tasks.

More information can be found on the main site http://www.vscp.org

##Framework
The VSCP software framework for level 1 devices provides several layers according to the [VSCP specification](http://www.vscp.org/docs/vscpspec/doku.php).

##Core
![core-diagram](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/doxfiles/vscp_modules.png)
![color-legend](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/doxfiles/vscp_modules_color_legend.png)

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

##Event abstraction
![core-diagram](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/doxfiles/vscp_event_modules.png)

Using only the core, you have to assemble the VSCP events by yourself. If you want to deal only with parameter, which are
VSCP independent, use the next upper layer, the event abstraction modules.

##Structure

<pre>
+---common              (Common sourcecode, used for examples and projects)
+---examples            (Examples which are showing how to use the VSCP framework)
|   +---avr             (Examples with Atmel AVR microcontrollers)
|   \---pc              (PC example for windows and linux)
+---projects            (Projects using the VSCP framework)
|   \---avr             (Projects with Atmel AVR microcontrollers)
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

##Getting started

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

###Copy the VSCP framework to your project

1. Copy or link the VSCP framework (./vscp and ./vscp/events) to your project.
2. Copy (!!do not link!!) all necessary template files (./vscp/templates) to your project. Recommended is a sub-directory "vscp_user".
3. Update your makefile or your project configuration.
 
###Initialization of the VSCP framework

The VSCP framework has to be initialized, before any function is used. This is simply done
by calling the function vscp\_core\_init() during start-up.

###Processing of the VSCP framework

The VSCP framework has to be called periodically to be able to react on incoming events.
Call the process routine vscp\_core\_process() in a constant cyclic period. The period should be
lower or equal than 100 ms, but depends on several factors, like the bitrate on the choosen
communication bus, the event load on the bus and etc.

The process routine handle all received VSCP events.

###VSCP framework timer

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

###VSCP transport adaption

Now its time to connect the VSCP framework to the communication bus. This can be done by implementing the transport
adapter in the vscp\_tp\_adapter.c template file.

###Control the VSCP lamp

To see that something is happen on your embedded device, next step is to control the VSCP lamp (in most cases a LED).
Update the function vscp\_portable\_setLampState() in the vscp\_portable.c module.

###Connect the initialization button

According to the VSCP specification, every embedded device should have a button to start the segment initialization.
Hopefully you have one right now :-) and if it is pressed, call the function vscp\_core\_startNodeSegmentInit() in the
vscp\_core.c module.

###Persistent memory

It is important that the VSCP framework can store data in a persistent memory, e.g. an EEPROM. Implement in the
vscp\_ps\_access.c module the low level access to the persistent memory. Its quite easy, because only byte access
functions are used, so you have one read and one write function to adapt.

###Ready to run

Now the minimal sub set is done and your node hopefully starts up with a nickname discovery.

Have fun!

##Mailing list

Are you interesting? Join us on the mailing list: https://groups.google.com/forum/#!forum/vscp

##Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

##License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
