/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef MBED_PINMAP_H
#define MBED_PINMAP_H

#include "pin_names.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    PinName pin;
    int peripheral;
    int function;
} PinMap;

void pin_function(PinName pin, int function);
void pin_mode(PinName pin, PinMode mode);
void pin_pullctrl(PinName pin, PinPullCtrl pullctl);

uint32_t pinmap_peripheral(PinName pin, uint32_t wanted_peripheral, const PinMap *map);
uint32_t pinmap_merge(uint32_t a, uint32_t b);
void     pinmap_pinout(PinName pin, const PinMap *map);

void halt_on_error_found(const char *funcname, const char *str, uint32_t para);

#ifdef __cplusplus
}
#endif

#endif
