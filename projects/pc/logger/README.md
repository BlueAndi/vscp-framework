# VSCP logger

## Manual

The VSCP logger shows events on the command line, which are received via a connection to a VSCP daemon.

Example of logs:
```
2015-10-08 21:42:03 Rx: L1    class=0x0014 type=0x09 prio= 0 oAddr=0x00 - num=3 data=00 00 00
2015-10-08 21:42:04 Rx: L1    class=0x0000 type=0x01 prio= 0 oAddr=0x00 - num=5 data=AC 56 16 63 02
```

Call the logger with -h or --help to see the command line arguments.

Have fun!

## Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
