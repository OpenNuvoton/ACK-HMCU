/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef CMSIS_NVIC_H
#define CMSIS_NVIC_H

#include "cmsis.h"

#define NVIC_USER_IRQ_OFFSET 16
#define NVIC_USER_IRQ_NUMBER 32
#define NVIC_NUM_VECTORS     (NVIC_USER_IRQ_OFFSET + NVIC_USER_IRQ_NUMBER)

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    #define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) &Image$$ER_IRAMVEC$$ZI$$Base)
#elif defined(__ICCARM__)
    #pragma section = "IRAMVEC"
    #define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) __section_begin("IRAMVEC"))
#elif defined(__GNUC__)
    #define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) &__start_vector_table__)
#endif

#endif
