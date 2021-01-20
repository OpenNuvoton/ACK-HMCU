@echo off
SETLOCAL

CALL :OFFLINE_OTA SmartLight
pause
EXIT /b 0


:OFFLINE_OTA
echo Path of output file is %~1

set PATH_TOOL=..\..\..\..\test\validation\
set PATH_OTA=%~1.ota

py -3 %PATH_TOOL%validate-ota.py --debug-port COM9 --module-port COM21 --devicetype A3D6VH9ZBWCJX2 %PATH_OTA%

