/**************************************************************************//**
 * @file     maic.c
 * @brief    ACK HMCU porting.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
 
#include "main.h"

void mcu_init(void);
void peripheral_init(void);

void Loader(void);
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    mcu_init();

    Loader();

    while (1);
}
