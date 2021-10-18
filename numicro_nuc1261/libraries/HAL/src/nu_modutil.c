/**************************************************************************//**
 * @file     nu_modutil.c
 * @brief    NuModule routine implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "pin_names.h"
#include "nu_modutil.h"

const struct nu_modinit_s *get_modinit(uint32_t modname, const struct nu_modinit_s *modprop_tab)
{
    const struct nu_modinit_s *modprop_ind = modprop_tab;
    while (modprop_ind->modname != NC)
    {
        if ((int) modname == modprop_ind->modname)
        {
            return modprop_ind;
        }
        else
        {
            modprop_ind ++;
        }
    }

    return NULL;
}
