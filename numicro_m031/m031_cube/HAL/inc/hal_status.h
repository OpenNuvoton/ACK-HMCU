/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __HAL_STATUS
#define __HAL_STATUS

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_common.h>
#include <stdio.h>

typedef enum
{
    HAL_I2C_ERROR_NO_SLAVE = -4,
    HAL_BUSY     = -3,
    HAL_TIMEOUT  = -2,
    HAL_ERROR    = -1,
    HAL_OK       = 0
} HAL_Status;

#define HAL_MAX_DELAY    0xFFFFFFFFU
#define HAL_WAIT_FOREVER 0xFFFFFFFFU

//#define ROUND_DOWN(a,b)   (((a) / (b)) * (b))
//#define ROUND_UP(a,b)         (((a+b) / (b)) * (b))
#define ROUND_DOWN(a, b)    ((a) & ~(b - 1))
#define ROUND_UP(a, b)      (((a) + (b - 1)) & ~(b - 1))
#define ALIGNED(a, b)       (((a) & (b - 1)) == 0)

#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

#ifdef __cplusplus
}
#endif

#endif /* __HAL_STATUS */
