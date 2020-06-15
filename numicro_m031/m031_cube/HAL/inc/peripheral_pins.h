/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef SOC_PERIPHERALPINS_H
#define SOC_PERIPHERALPINS_H

#include "pinmap.h"
#include "peripheral_names.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const PinMap PinMap_ADC[];

extern const PinMap PinMap_I2C_SDA[];

extern const PinMap PinMap_I2C_SCL[];

extern const PinMap PinMap_PWM[];

extern const PinMap PinMap_UART_TX[];

extern const PinMap PinMap_UART_RX[];

extern const PinMap PinMap_UART_RTS[];

extern const PinMap PinMap_UART_CTS[];

extern const PinMap PinMap_SPI_MOSI[];

extern const PinMap PinMap_SPI_MISO[];

extern const PinMap PinMap_SPI_SCLK[];

extern const PinMap PinMap_SPI_SSEL[];

#ifdef __cplusplus
}
#endif

#endif //SOC_PERIPHERALPINS_H

