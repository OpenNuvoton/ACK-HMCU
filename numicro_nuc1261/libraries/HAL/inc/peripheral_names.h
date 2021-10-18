/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef SOC_PERIPHERALNAMES_H
#define SOC_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: Check all module base addresses (XXX_BASE in BSP) for free bit fields to define module name
//       which encodes module base address and module index/subindex.
#define NU_MODSUBINDEX_Pos              0
#define NU_MODSUBINDEX_Msk              (0x1Ful << NU_MODSUBINDEX_Pos)
#define NU_MODINDEX_Pos                 21
#define NU_MODINDEX_Msk                 (0x7ul << NU_MODINDEX_Pos)

#define NU_MODNAME(MODBASE, INDEX, SUBINDEX)    ((MODBASE) | ((INDEX) << NU_MODINDEX_Pos) | ((SUBINDEX) << NU_MODSUBINDEX_Pos))
#define NU_MODBASE(MODNAME)                     ((MODNAME) & ~(NU_MODINDEX_Msk | NU_MODSUBINDEX_Msk))
#define NU_MODINDEX(MODNAME)                    (((MODNAME) & NU_MODINDEX_Msk) >> NU_MODINDEX_Pos)
#define NU_MODSUBINDEX(MODNAME)                 (((MODNAME) & NU_MODSUBINDEX_Msk) >> NU_MODSUBINDEX_Pos)

typedef enum
{
    ADC_0_0 = (int) NU_MODNAME(ADC_BASE, 0, 0),
    ADC_0_1 = (int) NU_MODNAME(ADC_BASE, 0, 1),
    ADC_0_2 = (int) NU_MODNAME(ADC_BASE, 0, 2),
    ADC_0_3 = (int) NU_MODNAME(ADC_BASE, 0, 3),
    ADC_0_4 = (int) NU_MODNAME(ADC_BASE, 0, 4),
    ADC_0_5 = (int) NU_MODNAME(ADC_BASE, 0, 5),
    ADC_0_6 = (int) NU_MODNAME(ADC_BASE, 0, 6),
    ADC_0_7 = (int) NU_MODNAME(ADC_BASE, 0, 7),
    ADC_0_8 = (int) NU_MODNAME(ADC_BASE, 0, 8),
    ADC_0_9 = (int) NU_MODNAME(ADC_BASE, 0, 9),
    ADC_0_10 = (int) NU_MODNAME(ADC_BASE, 0, 10),
    ADC_0_11 = (int) NU_MODNAME(ADC_BASE, 0, 11),
    ADC_0_12 = (int) NU_MODNAME(ADC_BASE, 0, 12),
    ADC_0_13 = (int) NU_MODNAME(ADC_BASE, 0, 13),
    ADC_0_14 = (int) NU_MODNAME(ADC_BASE, 0, 14),
    ADC_0_15 = (int) NU_MODNAME(ADC_BASE, 0, 15),
} ADCName;
#define ADC_MAX     16

typedef enum
{
    UART_0 = (int) NU_MODNAME(UART0_BASE, 0, 0),
    UART_1 = (int) NU_MODNAME(UART1_BASE, 1, 0),
    UART_2 = (int) NU_MODNAME(UART2_BASE, 2, 0),
} UARTName;
#define UART_MAX    3

typedef enum
{
    PWM_0_0 = (int) NU_MODNAME(PWM0_BASE, 0, 0),
    PWM_0_1 = (int) NU_MODNAME(PWM0_BASE, 0, 1),
    PWM_0_2 = (int) NU_MODNAME(PWM0_BASE, 0, 2),
    PWM_0_3 = (int) NU_MODNAME(PWM0_BASE, 0, 3),
    PWM_0_4 = (int) NU_MODNAME(PWM0_BASE, 0, 4),
    PWM_0_5 = (int) NU_MODNAME(PWM0_BASE, 0, 5),

    PWM_1_0 = (int) NU_MODNAME(PWM1_BASE, 1, 0),
    PWM_1_1 = (int) NU_MODNAME(PWM1_BASE, 1, 1),
    PWM_1_2 = (int) NU_MODNAME(PWM1_BASE, 1, 2),
    PWM_1_3 = (int) NU_MODNAME(PWM1_BASE, 1, 3),
    PWM_1_4 = (int) NU_MODNAME(PWM1_BASE, 1, 4),
    PWM_1_5 = (int) NU_MODNAME(PWM1_BASE, 1, 5)
} PWMName;
#define PWM_MAX     12

typedef enum
{
    DMA_0 = (int) NU_MODNAME(PDMA_BASE, 0, 0)
} DMAName;
#define DMA_MAX     1

typedef enum
{
    RTC_0 = (int) NU_MODNAME(RTC_BASE, 0, 0)
} RTCName;
#define RTC_MAX     1

typedef enum
{
    CRC_0 = (int) NU_MODNAME(CRC_BASE, 0, 0),
} CRCName;
#define CRC_MAX     1

#ifdef __cplusplus
}
#endif

#endif //SOC_PERIPHERALNAMES_H
