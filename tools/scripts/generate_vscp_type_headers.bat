@echo off

rem ********************************************************************************
rem Author: Andreas Merkle, http://www.blue-andi.de
rem ********************************************************************************

echo Generate VSCP type headers ...
java -jar ../xslt/saxon/saxon9he.jar -s:../vscp-protocol/vscp-protocol.xml -xsl:vscp-protocol2vscp-type-headers.xsl
if not %ERRORLEVEL% == 0 (
pause
)
