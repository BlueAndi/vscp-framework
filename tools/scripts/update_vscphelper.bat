@echo off

if "%1" == "" (
    echo VSCP path parameter is missing.
    exit 1
)

if "%1" == "" (
    echo Variant is missing. Please specify release or debug.
    exit 1
)

set VSCPHELPER_PATH=..\..\common\pc\vscphelper

echo Updating the vscphelper ...

copy /Y %1\src\common\crc.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\common\canal.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\common\canal_macro.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\common\canaldlldef.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\common\vscp.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\common\vscp_class.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\common\vscp_type.h %VSCPHELPER_PATH%\
copy /Y %1\src\vscp\helperlib\vscphelperlib.h %VSCPHELPER_PATH%\

copy /Y %1\src\vscp\helperlib\windows\vscphelper.dll %VSCPHELPER_PATH%\lib\windows\%2\
copy /Y %1\src\vscp\helperlib\windows\vscphelper.lib %VSCPHELPER_PATH%\lib\windows\%2\

echo Finished.
