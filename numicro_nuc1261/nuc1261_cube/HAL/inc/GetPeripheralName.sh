#!/bin/sh

PATH_SYS_FILE="../../StdDriver/inc/sys.h"


PrintHead () {
echo "/**************************************************************************//**
 * @file     peripheral_pins.c
 * @brief    Peripheral module pin declaration.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
"
}

PrintInclude () {
echo "#include \"peripheral_pins.h\""
}

# $1: "PinMap variable name" 
# $2: filter rule
PrintPinMapStructure () {
	echo "/* $1 */"
	echo "const PinMap PinMap_"$1"[] = { "

	grep -r $2 $PATH_SYS_FILE | awk -F' ' '{split($2,a,"_");  split(a[4],b,"MFP"); split(a[5],c,/[0-9]/); split($2,d,c[1]); split(d[2],e,"_"); split(a[6],f,"CH");  print "    { "substr(a[4],1,2)"_"substr(b[1],3)", \t",c[1]"_"e[1],", \t",$2," },"}'
	
	echo "    { NC,   NC,    0 }"
	echo "};"
	echo ""
}

OutCSource () {
	PrintHead
	PrintInclude

	PrintPinMapStructure "PWM" "MFP_PWM[0-9]*_CH"

    # SYS_GPA_MFPL_PA0MFP_UART1_TXD
	PrintPinMapStructure "UART_TX" "MFP_UART[0-9]*_TX"
	PrintPinMapStructure "UART_RX" "MFP_UART[0-9]*_RX"
	PrintPinMapStructure "UART_RTS" "MFP_UART[0-9]*_nRTS"
	PrintPinMapStructure "UART_CTS" "MFP_UART[0-9]*_nCTS"
}

PrintPinMapDef () {
	echo "/* $1 */"
	echo "extern const PinMap PinMap_"$1"[];"
	echo ""
}

OutCHeader () {

	PrintHead
	
echo "
#ifndef SOC_PERIPHERAL_PINS_H
#define SOC_PERIPHERAL_PINS_H

#include \"pinmap.h\"
#include \"peripheral_names.h\"

#ifdef __cplusplus
extern \"C\" {
#endif

"
	PrintPinMapDef "PWM"

	PrintPinMapDef "UART_TX"
	PrintPinMapDef "UART_RX"
	PrintPinMapDef "UART_RTS"
	PrintPinMapDef "UART_CTS"

echo "#ifdef __cplusplus
}
#endif

#endif //SOC_PERIPHERAL_PINS_H
"
	
}

OutCSource > __peripheral_pins.c
sed -i 's/EPWM_/PWM_/g' __peripheral_pins.c



OutCHeader > __peripheral_pins.h
