@echo off

rem ********************************************************************************
rem Author: Andreas Merkle, http://www.blue-andi.de
rem ********************************************************************************

rem GNU32 tools
set SED=..\..\..\..\tools\gnu32\sed.exe
set EXPAND=..\..\..\..\tools\gnu32\expand.exe

echo Generate application register access ...
java -jar ../../../../tools/xslt/saxon/saxon9he.jar -s:app_register.xml -xsl:xml2appReg.xsl
if not %ERRORLEVEL% == 0 (
pause
)

echo Generate user persistency access ...
java -jar ../../../../tools/xslt/saxon/saxon9he.jar -s:app_register.xml -xsl:xml2ps.xsl
if not %ERRORLEVEL% == 0 (
pause
)

rem Convert unix to windows line endings
echo Convert line endings ...
%SED% -i "s/$//" *.h
%SED% -i "s/$//" *.c

rem Convert tabs to spaces
echo Convert tabs to spaces ...
for /f "delims=" %%a in ('dir /b /a-d "*.c" "*.h"') do (
    %EXPAND% -t 4 %%a > temp.tmp
    type temp.tmp > %%a
)

rem Remove temporary files
del temp.tmp
del /Q sed*

rem Copy generated files to project
copy /Y vscp_app_reg.c ..\vscp_user
copy /Y vscp_ps_user.* ..\vscp_user

del vscp_app_reg.c
del /Q vscp_ps_user.*
