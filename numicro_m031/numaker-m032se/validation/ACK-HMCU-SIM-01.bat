@echo off
SETLOCAL

CALL config.cmd

py -3 %PATH_VALIDATION%\validate-directive.py -pd %ACK_DEBUG_PORT% -pm %ACK_COMM_PORT% directive -c 1 V3_ALEXA REPORT_STATE

pause