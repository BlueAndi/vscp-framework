README
======

How to execute the tests of the VSCP framework?
-----------------------------------------------

The tests itself are designed to be executed on a PC.

1. Linux
--------

- Note: Not tested under linux yet -

Steps:
1. Change to the folder ./vscp/test
2. Call "make debug"
3. Execute the binary "./bin/vscp_test" and check the output
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
1. Change the folder to ./vscp/test
2. Call "make debug"
3. Execute the binary "./bin/vscp_test.exe" and check the output
4. Finished
