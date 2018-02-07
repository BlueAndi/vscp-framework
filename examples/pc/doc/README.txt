README
======

1. Linux
--------

Following has to be installed:
- gcc
- libwxbase3.0-dev (dependency of vscphelper lib)
- libexpat1-dev (XML parser)

Steps:
1. Change the folder to the pc example folder.
2. Call "make debug" to build a debug version or "make release" to build a release version.
3. Execute the binary "./bin/vscpExamplePc"
4. Finished

2. Windows
----------

2.1 Tools to install

Download and install MingGW and MSys (http://www.mingw.org/).
Download and install the following packages with the installation manager:
- mingw32-expat
- mingw32-libexpat-dev

https://sourceforge.net/projects/mingw/files/MinGW/Base/pthreads-w32/pthreads-w32-2.10-pre-20160821-1/

2.2 How to build

The makefile needs a unix environment and therefore the msys shell is used.
Start the shell via <mingw-root>\msys\1.0\msys.bat, where <mingw-root> represents
the MinGW installation directory.

In the MSYS shell:
1. Change the folder to the pc example folder.
2. Call "make debug" to build a debug version or "make release" to build a release version.
3. Execute the binary "./bin/vscpExamplePc.exe"
4. Finished

In general
----------

Call the binary with -h or --help to find out the further options.
