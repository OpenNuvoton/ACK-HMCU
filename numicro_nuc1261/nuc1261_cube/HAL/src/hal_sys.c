/**************************************************************************//**
 * @file     hal_sys.c
 * @brief    Cpu core driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <hal_sys.h>

static volatile uint32_t u32Tick = 0;

void HAL_SYS_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
    /* Check the parameters */
    assert_param(IS_NVIC_PREEMPTION_PRIORITY(PreemptPriority));
    NVIC_SetPriority(IRQn, PreemptPriority);
}

void HAL_SYS_NVIC_EnableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Enable interrupt */
    NVIC_EnableIRQ(IRQn);
}

void HAL_SYS_NVIC_SystemReset(void)
{
    /* System Reset */
    NVIC_SystemReset();
}

uint32_t HAL_SYS_NVIC_GetPriority(IRQn_Type IRQn)
{
    /* Get priority for Cortex-M system or device specific interrupts */
    return NVIC_GetPriority(IRQn);
}

void HAL_SYS_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Set interrupt pending */
    NVIC_SetPendingIRQ(IRQn);
}

uint32_t HAL_SYS_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Return 1 if pending else 0 */
    return NVIC_GetPendingIRQ(IRQn);
}

void HAL_SYS_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Clear pending interrupt */
    NVIC_ClearPendingIRQ(IRQn);
}



void HAL_SYS_NVIC_DisableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Disable interrupt */
    NVIC_DisableIRQ(IRQn);
}

uint32_t HAL_SYS_TICK_Config(uint32_t TicksNumber)
{
    return SysTick_Config(TicksNumber);
}

HAL_Status HAL_SYS_TICK_Init(uint32_t TickPriority, int32_t i32MiliSeconds)
{
    // Source: HCLK, Reload:  CLK_GetHCLKFreq()/1000U = ~1ms
    if (i32MiliSeconds < 0 || !i32MiliSeconds)
        return HAL_ERROR;
    CLK_EnableSysTick(CLK_CLKSEL0_STCLKSEL_HCLK, CLK_GetHCLKFreq() / i32MiliSeconds);
    HAL_SYS_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
    return HAL_OK;
}

HAL_Status HAL_SYS_TICK_Deinit(void)
{
    HAL_SYS_NVIC_DisableIRQ(SysTick_IRQn);
    CLK_DisableSysTick();
    return HAL_OK;
}

static void HAL_SYS_TICK_Inc(void)
{
    u32Tick++;
}

uint32_t HAL_SYS_TICK_Get(void)
{
    return u32Tick;
}

void HAL_SYS_Delay(__IO uint32_t Delay)
{
    volatile uint32_t tickstart = HAL_SYS_TICK_Get();
    volatile uint32_t tickstop = tickstart + Delay;

    if (tickstop < HAL_MAX_DELAY)
        tickstop++;

    while (HAL_SYS_TICK_Get() < tickstop)
    {

    }
}

void HAL_SYS_TICK_Suspend(void)
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

void HAL_SYS_TICK_Resume(void)
{
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

HAL_Status HAL_SYS_Init(void)
{
    // NOTE: Support singleton semantics to be called from other init functions
    static int inited = 0;
    if (inited)
    {
        return HAL_OK;
    }
    inited = 1;

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

#if DEF_USE_HXT
    /* Enable HXT clock (external XTAL 32MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
#endif

#if DEF_USE_LXT
    /* Enable LXT */
    CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);
    /* Wait for LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);
#endif

    /* Enable Internal RC 22.1184 MHz clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Enable LIRC */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);

    /* Wait for HIRC 22.1184 MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set core clock as 72M from PLL */
    CLK_SetCoreClock(FREQ_72MHZ);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

    return HAL_OK;
}

HAL_Status HAL_SYS_DeInit(void)
{
    HAL_SYS_TICK_Deinit();
    return HAL_OK;
}

void SysTick_Handler()
{
    HAL_SYS_TICK_Inc();
    HAL_SYS_TICK_InvokeCallback();
}
