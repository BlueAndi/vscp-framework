#Relay Board 01

##Description

...

##Fuses

* Brown-out detection set to 4,3V
* Serial program downloading enabled
* Boot sector size set to 4096 bytes (2048 words)
* 16 MHz crystal oscillator

This will result in
* Without bootloader: Extended 0xFC, High 0xD9, Low 0xFF
* With bootloader: Extended 0xFC, High 0xD8, Low 0xFF

##Mailing list

Are you interesting? Join us on the mailing list: https://groups.google.com/forum/#!forum/vscp

##Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

##License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
