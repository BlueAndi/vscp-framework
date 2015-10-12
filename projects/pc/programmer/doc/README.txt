README
======

1. Linux
--------

Following has to be installed:
- gcc

Steps:
1. Change the folder to the pc example folder.
2. Call "make debug" to build a debug version or "make release" to build a release version.
3. Execute the binary "./bin/vscpProgrammerPc"
4. Finished

2. Windows
----------

2.1 Tools to install

Download and install MingGW and MSys (http://www.mingw.org/).

2.2 How to build

The makefile needs a unix environment and therefore the msys shell is used.
Start the shell via <mingw-root>\msys\1.0\msys.bat, where <mingw-root> represents
the MinGW installation directory.

In the MSYS shell:
1. Change the folder to the pc example folder.
2. Call "make debug" to build a debug version or "make release" to build a release version.
3. Execute the binary "./bin/vscpProgrammerPc.exe"
4. Finished

In general
----------

Call the binary with -h or --help to find out the further options.

