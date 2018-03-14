# VSCP L1 Bootloader for Relay Board 01

## Description

This bootloader follows the VSCP standard bootloader algorithm. It uses the VSCP
framework base bootloader, which is described in
![detail here](https://github.com/BlueAndi/vscp-framework/tree/master/vscp/bootloader).
And provides additional the user specific adaptions, which are in the vscp\_user folder.

## Facts

* This bootloader project supports currently the following AVR MCUs:
 * ATmega328P
* The CAN controller MCP2515 is used and connected to the SPI.
* The BOOTSZ is set to 0 which results in a 4096 bytes bootloader section.

### Persistent memory layout

This bootloader uses the following persistent memory layout:

| Address  | Size in byte  | Description |
| :------: | :-----------: | :---------: |
| 0 | 1 | Boot flag (jump to application or stay in bootloader) |
| 1 | 1 | VSCP nickname id |

## How to change the MCU?
* Change the MCU in the AtmelStudio.
* Change the BOOTSZ configuration in vscp\_platform.

## How to change the GUID?
* You will find it in the vscp\_bl\_adapter.c file, in the local variable section.

## Mailing list

Are you interesting? Join us on the mailing list: https://groups.google.com/forum/#!forum/vscp

## Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
