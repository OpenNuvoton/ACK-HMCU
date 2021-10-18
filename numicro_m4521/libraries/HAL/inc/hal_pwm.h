/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef HAL_PWM_H
#define HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "objects.h"
#include "hal_status.h"

typedef struct
{
    PWMName     pwm;
    PinName     pin;
    uint32_t        frequency;
    uint32_t        dutycycle;  // In percent, 0~100%
    uint32_t        pinpolar_inverse;
} S_PWMDev;

/**
 * Initialises a PWM pin
 */
HAL_Status HAL_PWM_Initialize(S_PWMDev *psPwmDev);

/**
 * Starts Pulse-Width Modulation signal output on a PWM pin
 */
HAL_Status HAL_PWM_Start(S_PWMDev *psPwmDev);

/**
 * Stops output on a PWM pin
 */
HAL_Status HAL_PWM_Stop(S_PWMDev *psPwmDev);

/**
 * Setup the Para of PWM
 */
HAL_Status HAL_PWM_Setup(S_PWMDev *psPwmDev);

/**
 * De-initialises an PWM interface, Turns off an PWM hardware interface
 */
HAL_Status HAL_PWM_Finalize(S_PWMDev *psPwmDev);

#ifdef __cplusplus
}
#endif

#endif
