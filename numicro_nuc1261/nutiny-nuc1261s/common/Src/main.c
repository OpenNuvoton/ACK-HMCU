/**************************************************************************//**
 * @brief    ACK HMCU platform porting.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "main.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
void setup(void);
void loop(void);
void mcu_init(void);

int main(void)
{
    mcu_init();

    setup();

    while (1)
        loop();

}