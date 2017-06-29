# VSCP L1 Programmer

## Manual

The VSCP L1 Programmer programs a intel hex file image to a node via VSCP daemon connection.
It supports the VSCP standard boot loader algorithm.

For programming a node you need
* the node id (nickname),
* its GUID (the global unique id),
* a intel hex file with the new image,
* the ip address of your running VSCP daemon
* and at least its user/password.

Please note that the programmer only supports intel hex record types DATA and EOF. All others are NOT supported yet!

Example for programming a node on windows:
```
vscpProgrammerPc.exe -u admin -p secret -a demo.vscp.org:9598 -fillBlock -f new_image.hex -n 1 -g 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:01
```

Example for programming a node on linux:
```
./vscpProgrammerPc -u admin -p secret -a demo.vscp.org:9598 -fillBlock -f new_image.hex -n 1 -g 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:01
```

For more information about the program arguments, please call the executable with -h or --help.

The VSCP L1 Programmer can be quit at any time with 'q'.

Have fun!

## Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
