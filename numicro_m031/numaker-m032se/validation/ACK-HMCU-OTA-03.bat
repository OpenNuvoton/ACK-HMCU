@echo off
SETLOCAL

CALL config.cmd

py -3 %PATH_VALIDATION%\validate-ota.py -pd %ACK_DEBUG_PORT% -pm %ACK_COMM_PORT% -t %ACKDEVICETYPE% -n 3 %OTA3_FILE_PATH%
pause
