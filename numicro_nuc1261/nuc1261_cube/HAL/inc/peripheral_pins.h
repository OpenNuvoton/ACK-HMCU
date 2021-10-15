/**************************************************************************//**
 * @file     peripheral_pins.c
 * @brief    Peripheral module pin declaration.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef SOC_PERIPHERAL_PINS_H
#define SOC_PERIPHERAL_PINS_H

#include "pinmap.h"
#include "peripheral_names.h"

#ifdef __cplusplus
extern "C" {
#endif


/* PWM */
extern const PinMap PinMap_PWM[];

/* UART_TX */
extern const PinMap PinMap_UART_TX[];

/* UART_RX */
extern const PinMap PinMap_UART_RX[];

/* UART_RTS */
extern const PinMap PinMap_UART_RTS[];

/* UART_CTS */
extern const PinMap PinMap_UART_CTS[];

#ifdef __cplusplus
}
#endif

#endif //SOC_PERIPHERAL_PINS_H

