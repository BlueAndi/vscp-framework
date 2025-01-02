# Changelog

## 2.1.0

  * Supports the new VSCP_CLASS_L1_PROTOCOL events:
    * Type=52 (0x34) - Block Data Chunk ACK.
    * Type=53 (0x35) - Block Data Chunk NACK.
    * Type=54 (0x36) - Bootloader CHECK.
  * Measurement related events updated.
  * Fixed VSCP_CLASS_L1_PROTOCOL Type=40 (0x28) Missing parameter node address added.

## 2.0.3

  * Add missing extern "C" sections to the event headers.
  * Avoid using C++ keywords as function parameter name.

## 2.0.2

  * Changed recommended nickname discovery timeout from five to one second.

## 2.0.1

  Updates the library.json to include the ./src/events folder too.

## 2.0.0

  VSCP-framework repository was restructured. The new structure is improved for providing it as PlatformIO libraray or to include it as git submodule.

  The following was moved to a dedicated repository:
  * [VSCP bootloader](https://github.com/BlueAndi/vscp-bootloader) (original in /vscp/bootloader)
  * [CLI tools](https://github.com/BlueAndi/vscp-cli-tools) (original in /projects/pc)
  * [My Projects](https://github.com/BlueAndi/RelayBoard01) (original in /projects/avr)

  - VSCP framework
    - New Features:
      - Firmware device code added to register map, which was introduced with VSCP specification 1.13.0
    - Improvements:
      - ```dataNum``` variable in the VSCP message renamed to ```dataSize```. You may need to update at least your transport layer adaption.
    - Bugfixes:
      - The process routine will return now TRUE until a extended page register read is complete #43. Thanks to Kamil!
  - Examples
    - avr90can
      - VSCode and PlatformIO configuration added.

## 1.3.0

  - Common
    - vscphelperlib updated to v14.0.2 (note, the windows vscphelperlib is still v13.0.0).

  - VSCP framework
    - Added CLASS1.ALARM VSCP_TYPE_ALARM_RESET event added.
    - Added CLASS1.MEASUREMENT VSCP_TYPE_MEASUREMENT_REACTIVE_POWER and VSCP_TYPE_MEASUREMENT_REACTIVE_ENERGY events added.
    - Added CLASS1.INFORMATION VSCP_TYPE_INFORMATION_PROXIMITY_DETECTED event added.
    - Bugfixes:
      - The event description (doxygen) fixed for all events, which used the frame references in the protocol definition.

## 1.2.0

  - Common
    - The AVR CAN library supports now the ATmega32/64/128M1 too. Thanks to onitake!
    - The AVR CAN library contains some deprecated stuff, based on older days with winavr. One of them was updated. Thanks to onitake!
    - The AVR CAN library updated to revision of 22th august 2016 of https://github.com/dergraaf/avr-can-lib

  - VSCP framework
    - Added CLASS1.CONFIGURATION events. Thanks troky for updating the protocol definition!
    - Added CLASS1.INFORMATION VSCP_TYPE_INFORMATION_INCREMENTED and VSCP_TYPE_INFORMATION_DECREMENTED events added. Thanks troky for updating the protocol definition!
    - Added CLASS1.CONTROL VSCP_TYPE_CONTROL_INCREMENT and VSCP_TYPE_CONTROL_DECREMENT events added. Thanks troky for updating the protocol definition!
    - Optional status parameter added to VSCP_TYPE_SECURITY_MOTION.
    - CLASS1.DIAGNOSTIC events VSCP_TYPE_DIAGNOSTIC_CHARGING_ON and VSCP_TYPE_DIAGNOSTIC_CHARGING_OFF added.
    - CLASS1.SECURITY events VSCP_TYPE_SECURITY_GAS, VSCP_TYPE_SECURITY_IN_MOTION, VSCP_TYPE_SECURITY_NOT_IN_MOTION and VSCP_TYPE_SECURITY_VIBRATION added.
    - CLASS1.MEASUREMENT events VSCP_TYPE_MEASUREMENT_RADIATION_DOSE_EQ, VSCP_TYPE_MEASUREMENT_RADIATION_DOSE_EXPOSURE and VSCP_TYPE_MEASUREMENT_POWER_FACTOR added.
    - CLASS1.MEASUREMENT event VSCP_TYPE_MEASUREMENT_ENERGY: Optional unit Wh added.

## 1.1.0

  - VSCP framework
    - Bugfixes:
      - CLASS1.DISPLAY Set LED fixed. Thanks troky!
      - CLASS1.INFORMATION Node Heartbeat fixed. Thanks troky!
      - Bug in uint32 function parameter handling in the event abstraction fixed. Thanks troky!
      - Bug in vscp event module generation fixed in case of float function parameters. Thanks troky!
    - Improvements:
      - If in the decision matrix the zone/sub-zone match is enabled and the received zone/sub-zone is 0xFF
        (which applies to all zones/sub-zones), the decision matrix row will be considered. This way there is
        no need anymore to add a separate decision matrix row to handle 0xFF zone/sub-zone. Thanks troky for the hint!
    - Features:
      - Added Enter/Exit CLASS1.INFORMATION events. Thanks troky!
      - Updated CLASS1.CONTROL protocol and generated the corresponding event abstraction modules. Thanks troky!
      - The vscp_core_process() function returns now TRUE if a received event was handled, otherwise FALSE.
        This can be used for a faster handling of received events, e.g. call it in a loop as long as events are
        handled. But be aware about the watchdog. ;-)

## 1.0.0

  - VSCP framework
    - Bugfixes:
      - Fixed CLASS1.CONTROL Timed pulse on/off to match specification. Thanks troky!
    - Improvements:
      - Optimized decision matrix (+ extension) access during normal operation.
        First its checked whether the decision matrix row is enabled or not.
        If its enabled, the whole decision matrix row is read at once from persistent memory.
        Please update your vscp_ps_access.[ch] files accordingly!
        #20
    - Features:
      - CLASS1.CONTROL, Type=44, "PWM" added. Thanks troky!
      - With VSCP_CONFIG_START_NODE_PROBE_NICKNAME you define where to start with the nickname probing. Thanks troky!
      - All VSCP type headers are generated now from vscp-protocol.xml and some of the types changed in its
        name. Please update your sources accordingly!
      - All VSCP event abstraction modules are generated now from vscp-protocol.xml. Their file names changed to vscp_evt_...
        Please update your sources accordingly!

## 0.9.0

  - VSCP framework
    - Bugfixes:
      - Fix of misspellings/typos, thanks to TomasRoj.
      - VSCP measurement events fixed, because internal the parameters unit and index in the vscp_data_coding_getFormatByte() call were reversed. Thanks to troky!

    - Features:
      - CLASS1.ALARM, Type=12, "Watchdog" added.
      - CLASS1.INFORMATION, Type=80, "Updated" added.
      - CLASS1.WEATHER/CLASS1.WEATHER_FORECAST Type=52, "UV Index" added.

## 0.8.0

  - Examples
    - PC
      - Fixed the GUID in the event, sent by the node. It must be always
        a combination of interface GUID and the node nickname id at LSB byte.
      - Fixed the default option of supported events.

  - Projects
    - PC
      - All projects adapted according to the possible usage of vscphelper library in debug or release variant.

  - VSCP framework
    - Bugfixes:
      - CLASS1.INFOMRATION Type=77 month/year was in wrong position in the event data.

    - Features:
      - CLASS1.INFOMRATION Type=78 and 79 added (rising and falling edge detection)
      - CLASS1.INFORMATION Heartbeat event supports now extra data, see vscp_information_sendNodeHeartbeatEventEx().
      - If your node shall send a custom heartbeat with user data and extended data,
        enable VSCP_CONFIG_ENABLE_CUSTOM_HEARTBEAT in your configuration. Find the function prototype in
        the vscp_portable.c module. Thanks troky!

## 0.7.0

  - Common
    - vscphelperlib updated to v13.0.0 build 13 (Aluminium release)

  - Examples
    - AVR
      - Upgraded to AtmelStudio 7

  - Projects
    - AVR
      - Upgraded to AtmelStudio 7

  - VSCP framework
    - Bugfixes:
      - If a segment controller heartbeat is received, the time since epoch will now be stored.
      - If a segment master don't assign a nickname id, a infinite segment master probe loop occurred.

    - Features:
      - CLASS1.INFORMATION Type=69 - 77 added.
      - CLASS1.CONTROL Type=42 - 43 added.
      - VSCP_TIMER_ID_INVALID introduced for invalid timer id. Please update your code accordingly, see template 'vscp_timer.c'.
      - The internal VSCP core time (time since epoch) is now updated once per second.
        The core process routine will handle it, independent of the current internal state.
        Please increase your max. number of timers to be able to use this feature.
      - Callout function for every received segment controller heartbeat with timestamp implemented.
      - MCU stored GUID is now supported additional to storing the GUID in code flash or in persistent memory.
      - Application is now able to handle protocol class events, if configured with VSCP_CONFIG_PROTOCOL_EVENT_NOTIFICATION.

## 0.6.1

  - VSCP framework
    - inttypes.h replaced with stdint.h, because inttypes.h was missing in the MCUXpresso toolchain for the Kinetis MCU.

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
