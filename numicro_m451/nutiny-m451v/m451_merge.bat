@echo off
SETLOCAL

CALL :MERGEHEX DashReplenishment
CALL :MERGEHEX HelloWorld
CALL :MERGEHEX Microwave
CALL :MERGEHEX SmartLight

pause

EXIT /B %ERRORLEVEL%

:MERGEHEX
echo Path of output file is %~1

set PATH_TOOL=..\..\..\..\ota\utility\
set PATH_MERGED=Merged_%~1.hex
set PATH_LOADER=.\customloader\MDK-ARM\Build\CustomLoader.hex
set PATH_HMCU=.\applications\%~1\MDK-ARM\Build\%~1.hex
set PATH_OTA=%~1.ota

py -3 %PATH_TOOL%hexmerge.py -o %PATH_MERGED% %PATH_LOADER% %PATH_HMCU%
py -3 %PATH_TOOL%hexdump.py %PATH_MERGED%
py -3 %PATH_TOOL%hex2ota.py -t A3D6VH9ZBWCJX2 %PATH_HMCU% %PATH_OTA%
py -3 %PATH_TOOL%otadump.py %PATH_OTA%

EXIT /B 0