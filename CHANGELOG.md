## 0.6.0

  - VSCP framework
    - Features:
      - The decision matrix NG register read/write is now handled in the core as well. Therefore it is not necessary
        anymore to handle it in the vscp_app_reg.c manually.
      - CLASS1.CONTROL Type=40, All lamps on  and CLASS1.CONTROL Type=41, All lamps off added.
      - CLASS1.ALARM Type=10, Arm and CLASS1.ALARM Type=11, Disarm added.
      - CLASS1.ALARM now have byte 1 specified as '0=off. 1=on' in first byte.
      - CLASS1.MEASUREMENT Type=58 - 60 added.
      - CLASS1.INFORMATION Type=69 - 74 added.

  - Common
    - vscphelperlib updated to v1.12.4 (previous one was v1.1.0)

  - Projects
    - AVR
      - Relay board 01
        - Features:
          - Decision matrix NG size increased to 512 bytes.
        
## 0.5.0

  - Examples
    - AVR
      - Features:
        - Tx retry mechanismen implemented.
      - Bugfixes:
        - Watchdog was not disabled as soon as possible. That caused a permanent reset, after a watchdog reset.
    - PC
      - Features:
        - Command line arguments changed, please use -h or --help to see how.
      - Bugfixes:
        - Setting the GUID via command line argument fixed.
        - Fixed wrong compares for a empty strings.  
  
  - Projects
    - AVR
      - Bootloader jumper
        - Bugfixes:
          - Watchdog was not disabled as soon as possible. That caused a permanent reset, after a watchdog reset.
      - Bootloader VSCP
        - Bugfixes:
          - Watchdog was not disabled as soon as possible. That caused a permanent reset, after a watchdog reset.
      - Relay board 01
        - Features:
          - Tx retry mechanismen implemented.
        - Bugfixes:
          - Fixed a wrong compare in the shutter position detection.
          - Watchdog was not disabled as soon as possible. That caused a permanent reset, after a watchdog reset.
          - Driving a shutter infinite up or down was not possible.
    - PC
      - VSCP event logger for the command line introduced.
      - VSCP L1 programmer for the command line introduced.

  - VSCP framework
    - Features:
      - CLASS1_INFORMATION, Type = 9 (0x09) Node Heartbeat is now mandatory for all Level I nodes. Default period is set to 30s.
      - Bootloader can now handle shorter images too, but the image size has to be a multiple of the block size.
    - Bugfixes:
      - Bootloader CRC calculation of the whole image fixed.
      
## 0.4.0

Features:

  - VSCP framework
    - Segment initialization lamp handling improved: If the node starts up without nickname id, the lamp blinks slow. If the node enters nickname discovery, the lamp blinks fast. If the node discovered a nickname successfully, the lamp stays on.
    - Log events added.
    - Logger module added for easier logging during development phase or further. See the vscp\_loggger.h macros for more information.
    - Logger is integrated in VSCP core. Enable the VSCP\_CONFIG\_ENABLE\_LOGGER compiler switch to enable the log functionality.
    - CLASS1.INFORMATION type 59-68 events introduced.
    - CLASS1.CONTROL type 34-39 events introduced.
    - VSCP framework is now compliant to VSCP specification v1.10.16
    - CLASS1.DIAGNOSTIC and .ERROR added.
    - CLASS1.SECURITY event type 20 added.
    - Changed the priority for packet bursts of extended read responses to the lowest priority, so other frames win arbitration (e. g. on the CAN bus) when they are sent. This will help other events moving on the bus.
    - Changed functionality for extended page read request with optional byte. If the optional byte is 0, 256 registers will be read now, instead of 1.

  - Project: Relay board 01
    - CAN is monitored and any error stored in the VSCP alarm bitfield. See MDF for more information about each bit.
    - Scheduler is monitored by watchdog.
    - Several minor improvements.
    - Logger module integrated for easier debugging.
    
  - PC example
    - The node GUID can now be set via command line too, e. g. -guid00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:01
    
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
