/**************************************************************************//**
 * @file     nu_miscutil.c
 * @brief    Some misc. routine implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "pin_names.h"
#include "nu_miscutil.h"

void nu_nop(uint32_t n)
{
    uint32_t times = n / 10;
    uint32_t rmn = n % 10;

    while (times --)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }

    switch (rmn)
    {
    case 9:
        __NOP();
    case 8:
        __NOP();
    case 7:
        __NOP();
    case 6:
        __NOP();
    case 5:
        __NOP();
    case 4:
        __NOP();
    case 3:
        __NOP();
    case 2:
        __NOP();
    case 1:
        __NOP();
    default:
        break;
    }
}
