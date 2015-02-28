#VSCP L1 Bootloader of relay board 01

##Description

1. Determine whether
    a) the user force bootloader mode,
    b) an valid application is present and shall be started,
    c) the application jumped to the bootloader,
    d) no application is present.

2. Conditions for case a):
    - If the segment initialization button is pressed during power-up, stay in the bootloader.
    Go to step 6.

3. Conditions for case b):
    - The segment initialization button is released.
    - The boot flag in the persistent memory shall contain 0xAA for destination application.
    If all conditions met, jump to the application otherwise go to step 6.

4. Conditions for case c):
    - The hardware jumper is not set.
    - The boot flag in the persistent memory shall contain 0xBB for destination bootloader.
    If all conditions met, go to step 9.

5. Conditions for case d):
    - The segment initialization button is released.
    - The boot flag in the persistent memory shall contain anything else than 0xAA and 0xBB.
    If all conditions met, go to step 6.

6. Send a "new node online/probe" event, so that the programming tool knows the node is available now.
    Node id 0xFE is used!

7. If any "ACK probe" event is received, the node shall go to sleep mode, because the
    nickname is already used.

8. Wait (infinite) for the "enter bootloader" event. Only if the received nickname
    and GUID are equal to the internal ones of the node, validate the proposed programming
    algorithm.
    If the algorithm is supported, continue with next step, otherwise a
    "NACK enter bootloader" event will be sent.

9. Because the application already received the "enter bootloader" event and validated its
    parameters, a "ACK enter bootloader" event shall be sent back now in the bootloader.
    Send the flash block size (= flash page size) and the number of blocks as parameters.

10. Wait (infinite) for the "start block data transfer" event. If the proposed memory type
    is correct acknowledge it via "ACK start block data transfer" event otherwise via
    "NACK start block data transfer" event.

11. Now several "block data" events will be received, until the complete block is transfered.
    Every received block data shall be written to RAM buffer. After the last data is transfered,
    a CRC16-CCITT shall be calculated over the whole block. And an "ACK block data" event shall
    be sent with the calculated CRC as parameter.

12. If a complete block is received, wait for a "program block" event. This is the
    trigger to delete a flash page and write the block RAM buffer to the flash page.
    If the block is successful written, send an "ACK program block" event to confirm it.
    Otherwise send an "NAK program block" event.
    Note that before writing the first block to flash, write 0xFF to boot flag in the persistent memory. 
    This shall be done here, because after this, there is no way back.

13. The next block shall be handled (go to step 10), except an "drop nickname/reset device" event
    is received. In this case the block programming operation shall be stopped.

14. Validate the application in the flash.
    If the application is valid, a "ACK activate new image" event shall be sent, the boot flag
    in the persistent memory shall be set to 0xAA and reboot the node.
    Otherwise "NAK activate new image" event shall be sent and the node shall go to sleep.

Notes:
* Unused bytes in a flash block shall be 0xFF.
* The host shall send always an image in the size of the whole application section.
* Because the AVR has flash pages, but the VSCP has blocks, both are considered as equal.
* If the programming procedure is aborted by "drop nickname/reset device" event
  the node reboots.

Error codes:
* 0 - Programming algorithm not supported
* 1 - Memory type not supported
* 2 - Bad block number
* 3 - Invalid message

##Facts

* The bootloader supports the following AVR MCUs:
 * ATmega328P
* The CAN controller MCP2515 is used and connected to the SPI.
* The BOOTSZ is set to 0 which results in a 4096 bytes bootloader section.

##How to change the MCU?
* Change the MCU in the AtmelStudio.
* Change the BOOTSZ configuration in vscp\_platform.

##How to change the GUID?
* You will find it in the vscp\_bl\_adapter.c file, in the local variable section.

##Mailing list

Are you interesting? Join us on the mailing list: https://groups.google.com/forum/#!forum/vscp

##Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

##License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
