@echo off
SETLOCAL

CALL config.cmd

py -3 %PATH_VALIDATION%\validate-directive.py -pd %ACK_DEBUG_PORT% -pm %ACK_COMM_PORT% directive -c 1 V3_ALEXA_POWER_CONTROLLER TURN_ON event -c 1 V3_ALEXA RESPONSE property V3_ALEXA_POWER_CONTROLLER POWER_STATE_PROP --power-state 0

pause