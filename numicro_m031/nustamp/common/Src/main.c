/**************************************************************************//**
 * @file     maic.c
 * @brief    Smartlight implementation.
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

#define DEF_PWM_TEST    0
#if DEF_PWM_TEST
void Hardware_TurnLightOn();
extern uint32_t g_colortemperature;
void cw_pwm_test()
{
    int idx = 0;
    int coltemp[5] = { 2200, 2700, 4000, 5500, 7000 };

    for (idx = 0; idx < 5; idx++)
    {
        g_colortemperature = coltemp[idx];
        Hardware_TurnLightOn();
        ACKPlatform_Delay(1000);
    }
}
#endif

int main(void)
{
    mcu_init();
#if DEF_PWM_TEST
    while (1)
        cw_pwm_test();
#else
    setup();
    while (1)
        loop();
#endif

}