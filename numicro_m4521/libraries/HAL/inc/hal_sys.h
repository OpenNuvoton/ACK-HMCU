/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __HAL_SYS
#define __HAL_SYS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "NuMicro.h"
#include "hal_status.h"

#define DEF_SYSTICK_INT_PRIORITY    0

static uint64_t u64Tick;

HAL_Status HAL_SYS_TICK_Init(uint32_t TickPriority, int32_t u32MiliSeconds);
HAL_Status HAL_SYS_TICK_Deinit(void);


void HAL_SYS_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_SYS_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_SYS_NVIC_SystemReset(void);
uint32_t HAL_SYS_NVIC_GetPriority(IRQn_Type IRQn);
void HAL_SYS_NVIC_SetPendingIRQ(IRQn_Type IRQn);
uint32_t HAL_SYS_NVIC_GetPendingIRQ(IRQn_Type IRQn);

void HAL_SYS_NVIC_ClearPendingIRQ(IRQn_Type IRQn);
void HAL_SYS_NVIC_DisableIRQ(IRQn_Type IRQn);

uint32_t HAL_SYS_TICK_Config(uint32_t TicksNumber);
uint32_t HAL_SYS_TICK_Get(void);

void HAL_SYS_Delay(__IO uint32_t Delay);

void HAL_SYS_TICK_Suspend(void);
void HAL_SYS_TICK_Resume(void);

HAL_Status HAL_SYS_Init(void);
HAL_Status HAL_SYS_DeInit(void);
void HAL_SYS_TICK_InvokeCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SYS */
