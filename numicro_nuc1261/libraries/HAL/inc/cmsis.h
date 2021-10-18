/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef OS_CMSIS_H
#define OS_CMSIS_H

#include "NUC1261.h"
#include "cmsis_nvic.h"

// Support linker-generated symbol as start of relocated vector table.
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    extern uint32_t Image$$ER_IRAMVEC$$ZI$$Base;
#elif defined(__ICCARM__)

#elif defined(__GNUC__)
    extern uint32_t __start_vector_table__;
#endif


#endif
