## 0.2.0 (unreleased)

Features:

  - VSCP framework
    - More events can now be sent on a higher abstraction. See ./vscp/events folder.
    - L1 over L2 event classes added.
    
  - PC example
    - The pc example loads the standard and extended decision matrix from a xml file.
    - L1 over L2 events are supported now.
    - The node sends now periodically simulated temperature values.
    - The node simulates 8 lamps, which can be controlled via decision matrix.
  
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
