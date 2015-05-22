@echo off

rem ********************************************************************************
rem Author: Andreas Merkle, http://www.blue-andi.de
rem ********************************************************************************

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

copy /Y vscp_app_reg.c ..\vscp_user
copy /Y vscp_ps_user.* ..\vscp_user

del vscp_app_reg.c
del /Q vscp_ps_user.*
