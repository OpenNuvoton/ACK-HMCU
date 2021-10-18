/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef NU_MODULE_UTIL_H
#define NU_MODULE_UTIL_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nu_modinit_s
{
    int modname;
    uint32_t clkidx;
    uint32_t clksrc;
    uint32_t clkdiv;
    uint32_t rsetidx;

    IRQn_Type irq_n;
    //int irq_n;

    void *var;
};

const struct nu_modinit_s *get_modinit(uint32_t modname, const struct nu_modinit_s *modprop_tab);

#ifdef __cplusplus
}
#endif

#endif
