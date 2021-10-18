/**************************************************************************//**
 * @file     hal_pwm.c
 * @brief    PWM driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <hal_pwm.h>

struct nu_pwm_var
{
    uint32_t    en_msk;
};

static struct nu_pwm_var pwm0_var =
{
    .en_msk = 0
};

static struct nu_pwm_var pwm1_var =
{
    .en_msk = 0
};

static uint32_t pwm_modinit_mask = 0;

static const struct nu_modinit_s pwm_modinit_tab[] =
{
    {PWM_0_0, PWM0_MODULE, CLK_CLKSEL1_PWM0SEL_PCLK0, 0, PWM0_RST, PWM0_IRQn, &pwm0_var},
    {PWM_0_1, PWM0_MODULE, CLK_CLKSEL1_PWM0SEL_PCLK0, 0, PWM0_RST, PWM0_IRQn, &pwm0_var},
    {PWM_0_2, PWM0_MODULE, CLK_CLKSEL1_PWM0SEL_PCLK0, 0, PWM0_RST, PWM0_IRQn, &pwm0_var},
    {PWM_0_3, PWM0_MODULE, CLK_CLKSEL1_PWM0SEL_PCLK0, 0, PWM0_RST, PWM0_IRQn, &pwm0_var},
    {PWM_0_4, PWM0_MODULE, CLK_CLKSEL1_PWM0SEL_PCLK0, 0, PWM0_RST, PWM0_IRQn, &pwm0_var},
    {PWM_0_5, PWM0_MODULE, CLK_CLKSEL1_PWM0SEL_PCLK0, 0, PWM0_RST, PWM0_IRQn, &pwm0_var},

    {PWM_1_0, PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0, PWM1_RST, PWM1_IRQn, &pwm1_var},
    {PWM_1_1, PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0, PWM1_RST, PWM1_IRQn, &pwm1_var},
    {PWM_1_2, PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0, PWM1_RST, PWM1_IRQn, &pwm1_var},
    {PWM_1_3, PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0, PWM1_RST, PWM1_IRQn, &pwm1_var},
    {PWM_1_4, PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0, PWM1_RST, PWM1_IRQn, &pwm1_var},
    {PWM_1_5, PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0, PWM1_RST, PWM1_IRQn, &pwm1_var},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

static void platform_pwmout_config(S_PWMDev *psPwmDev, int start)
{
    PWM_T *pwm_base = (PWM_T *) NU_MODBASE(psPwmDev->pwm);
    uint32_t chn = NU_MODSUBINDEX(psPwmDev->pwm);

    //printf("[%s] %08x.%d frequency=%d, dutycycle=%d, start=%d\n", __func__, pwm_base, chn, psPwmDev->frequency, psPwmDev->dutycycle, start);

    // To avoid abnormal pulse on (re-)configuration, follow the sequence: stop/configure(/re-start).
    // Stop
    PWM_ForceStop(pwm_base, 1 << chn);

    // Inverse output signal
    if (psPwmDev->pinpolar_inverse)
        pwm_base->POLCTL |= 1 << (PWM_POLCTL_PINV0_Pos + chn);

    // Enable output of the specified PWM channel
    PWM_EnableOutput(pwm_base, 1 << chn);

    // Configure
    PWM_ConfigOutputChannel(pwm_base, chn, psPwmDev->frequency, psPwmDev->dutycycle);

    // Re-start
    PWM_Start(pwm_base, 1 << chn);
}

static int32_t platform_pwmout_init(S_PWMDev *psPwmDev)
{
    struct nu_modinit_s *modinit = NULL;
    if (psPwmDev->pwm != (PWMName) pinmap_peripheral(psPwmDev->pin, psPwmDev->pwm, PinMap_PWM))
        goto exit_platform_pwmout_init;

    modinit = (struct nu_modinit_s *) get_modinit(psPwmDev->pwm, pwm_modinit_tab);
    if (!modinit || modinit->modname != (int) psPwmDev->pwm)
        goto exit_platform_pwmout_init;

    // NOTE: All channels (identified by PWMName) share a PWM module. This reset will also affect other channels of the same PWM module.
    if (!((struct nu_pwm_var *) modinit->var)->en_msk)
    {
        // Reset this module if no channel enabled
        SYS_ResetModule(modinit->rsetidx);
    }

    uint32_t chn =  NU_MODSUBINDEX(psPwmDev->pwm);

    // NOTE: Channels 0/1/2/3/4/5 share a clock source.
    if ((((struct nu_pwm_var *) modinit->var)->en_msk & 0x3F) == 0)
    {
        // Select clock source of paired channels
        CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);
        // Enable clock of paired channels
        CLK_EnableModuleClock(modinit->clkidx);
    }

    // Wire pinout
    pinmap_pinout(psPwmDev->pin, PinMap_PWM);

    platform_pwmout_config(psPwmDev, 0);

    ((struct nu_pwm_var *) modinit->var)->en_msk |= 1 << chn;

    // Mark this module to be inited.
    int i = modinit - pwm_modinit_tab;
    pwm_modinit_mask |= 1 << i;

    return HAL_OK;

exit_platform_pwmout_init:

    return HAL_ERROR;
}

static int platform_pwmout_free(S_PWMDev *psPwmDev)
{
    struct nu_modinit_s *modinit = NULL;
    PWM_T *pwm_base = (PWM_T *) NU_MODBASE(psPwmDev->pwm);
    uint32_t chn =  NU_MODSUBINDEX(psPwmDev->pwm);
    PWM_ForceStop(pwm_base, 1 << chn);
    PWM_DisableOutput(pwm_base, 1 << chn);

    modinit = (struct nu_modinit_s *)get_modinit(psPwmDev->pwm, pwm_modinit_tab);
    if (!modinit || (modinit->modname != (int) psPwmDev->pwm))
        goto exit_platform_pwmout_free;

    ((struct nu_pwm_var *) modinit->var)->en_msk &= ~(1 << chn);

    if ((((struct nu_pwm_var *) modinit->var)->en_msk & 0x3F) == 0)
    {
        CLK_DisableModuleClock(modinit->clkidx);
    }

    // Mark this module to be deinited.
    int i = modinit - pwm_modinit_tab;
    pwm_modinit_mask &= ~(1 << i);

    return HAL_OK;

exit_platform_pwmout_free:
    return HAL_ERROR;
}

/**
 * Initialises a PWM pin
 */
HAL_Status HAL_PWM_Initialize(S_PWMDev *psPwmDev)
{
    if (!psPwmDev)
        goto exit_hal_pwm_initialize;

    return platform_pwmout_init(psPwmDev);

exit_hal_pwm_initialize:

    return HAL_ERROR;
}

/**
 * Starts Pulse-Width Modulation signal output on a PWM pin
 */
HAL_Status HAL_PWM_Start(S_PWMDev *psPwmDev)
{
    if (!psPwmDev)
        goto exit_hal_pwm_start;

    if (psPwmDev->dutycycle)
        platform_pwmout_config(psPwmDev, 1);
    else
        platform_pwmout_config(psPwmDev, 0);

    return HAL_OK;

exit_hal_pwm_start:

    return HAL_ERROR;
}

/**
 * Stops output on a PWM pin
 */
HAL_Status HAL_PWM_Stop(S_PWMDev *psPwmDev)
{
    if (!psPwmDev)
        goto exit_hal_pwm_stop;

    platform_pwmout_config(psPwmDev, 0);

    return HAL_OK;

exit_hal_pwm_stop:

    return HAL_ERROR;
}

/**
 * De-initialises an PWM interface, Turns off an PWM hardware interface
 */
HAL_Status HAL_PWM_Finalize(S_PWMDev *psPwmDev)
{
    if (!psPwmDev)
        goto exit_hal_pwm_finalize;

    return platform_pwmout_free(psPwmDev);

exit_hal_pwm_finalize:

    return HAL_ERROR;
}
