## 0.4.0 (unreleased)

Features:

  - VSCP framework
    - Segment initialization lamp handling improved: If the node starts up without nickname id, the lamp blinks slow. If the node enters nickname discovery, the lamp blinks fast. If the node discovered a nickname successfully, the lamp stays on.
    - Log events added.
    - Logger module added for easier logging during development phase or further. See the vscp\_loggger.h macros for more information.

  - Project: Relay board 01
    - CAN is monitored and any error stored in the VSCP alarm bitfield. See MDF for more information about each bit.
    - Scheduler is monitored by watchdog.
    - Several minor improvements.
    - Logger module integrated for easier debugging.
    
Bugfixes:

  - VSCP framework
    - CRC of "activate new image" event was reconstructed wrong in the bootloader.

  - AT90CAN32 example
    - MDF updated
    
  - PC example
    - The L1 over L2 event to L1 event conversion missed to remove the interface GUID in case of 16 byte data.
    - Bootloader adapter programmed the page wrong (simulated).
    - MDF updated
    - Minor bugfixes

  - Project: Relay board 01
    - Bootloader support enabled
    - MDF updated
    
## 0.3.0

Features:

  - VSCP framework
    - Silent node behaviour is implemented, which is used for non-multimaster transport protocols, like RS-485. It can be enabled in the configuration.

  - PC example
    - The type of every received and transmitted class1.protocol is shown as user friendly string. This helps for better understanding.

Updates:

  - PC example
    - VSCP helper lib updated to VSCP v1.0.1 Neon release.

Bugfixes:

  - VSCP framework
    - L1 over L2 class C-define name fixed.
    - GUID check in the handling of enter boot loader mode event fixed.
    - CRC-CCITT calculation fixed, used by the boot loader.
    
  - PC example
    - Sending events on linux failed, because the binary used packed structures, but the vscphelperlib.so not.

## 0.2.2

Bugfixes:

  - VSCP framework
    - If the loopback is enabled, all messages will be written to it, except CLASS1.PROTOCOL. Because the VSCP core would interpret them.

## 0.2.1

Bugfixes:

  - PC example
    - Thread lock mechanism fixed.

## 0.2.0

Features:

  - VSCP framework
    - More events can now be sent on a higher abstraction. See ./vscp/events folder.
    - L1 over L2 event classes added.
    - Bootloader algorithm implemented. See ./vscp/bootloader folder.
    
  - PC example
    - The pc example loads the standard and extended decision matrix from a xml file.
    - L1 over L2 events are supported now.
    - The node sends now periodically simulated temperature values.
    - The node simulates 8 lamps, which can be controlled via decision matrix.
  
  - Bootloader Jumper is used for relay board 01 project in case of debugging purposes.
  
  - Bootloader VSCP is used for relay board 01 project in case of the VSCP bootloader algorithm.
  
Bugfixes:

  - VSCP framework
    - GUID MSB/LSB problem fixed.
    - Extended page register read sent only one response back. Now it sends one ore more, dependend on the requested number of register reads.
    - Node sub-zone address in the persistent memory fixed.
    - Exponent type in all class MEASUREZONE events fixed.

  - PC example
    - The EEPROM dump function of the pc example caused a segmentation fault.
    - The EEPROM dump function knows now about the node zone and sub-zone too.

## 0.1.0 (initial release)
