/**************************************************************************//**
 * @file     pinmap.c
 * @brief    Some peripheral pin function implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "pinmap.h"
#include "port_names.h"

void halt_on_error_found(const char *funcname, const char *str, uint32_t para)
{
    printf("[%s] %s %x\r\n", funcname, str, para);
    while (1);
}

void pinmap_pinout(PinName pin, const PinMap *map)
{
    if (pin == NC)
        return;

    while (map->pin != NC)
    {
        if (map->pin == pin)
        {
            pin_function(pin, map->function);

            pin_pullctrl(pin, PullNone);
            return;
        }
        map++;
    }
    halt_on_error_found(__func__, "could not pinout", (uint32_t)pin);
}

uint32_t pinmap_merge(uint32_t a, uint32_t b)
{
    // both are the same (inc both NC)
    if (a == b)
        return a;

    // one (or both) is not connected
    if (a == (uint32_t)NC)
        return b;
    if (b == (uint32_t)NC)
        return a;

    // mis-match error case
    halt_on_error_found(__func__, "pinmap mis-match", (uint32_t)a);
    return (uint32_t)NC;
}

static uint32_t pinmap_find_peripheral(PinName pin, uint32_t wanted_peripheral, const PinMap *map)
{
    while (map->pin != NC)
    {
        if ((map->pin == pin) &&
                (map->peripheral == wanted_peripheral))
            return map->peripheral;
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_peripheral(PinName pin, uint32_t wanted_peripheral, const PinMap *map)
{
    uint32_t peripheral = (uint32_t)NC;

    if (pin == (PinName)NC)
        return (uint32_t)NC;

    peripheral = pinmap_find_peripheral(pin, wanted_peripheral, map);
    if ((uint32_t)NC == peripheral) // no mapping available
    {
        halt_on_error_found(__func__, "pinmap not found for peripheral", (uint32_t)peripheral);
    }
    return peripheral;
}

static uint32_t pinmap_find_function(PinName pin, const PinMap *map)
{
    while (map->pin != NC)
    {
        if (map->pin == pin)
            return map->function;
        map++;
    }
    return (uint32_t)NC;
}

static uint32_t pinmap_function(PinName pin, const PinMap *map)
{
    uint32_t function = (uint32_t)NC;

    if (pin == (PinName)NC)
        return (uint32_t)NC;

    function = pinmap_find_function(pin, map);
    if ((uint32_t)NC == function) // no mapping available
    {
        halt_on_error_found(__func__, "pinmap not found for function", (uint32_t)function);
    }
    return function;
}

/**
 * Configure pin multi-function
 */
void pin_function(PinName pin, int data)
{
    uint32_t pin_index;
    uint32_t port_index;
    __IO uint32_t *GPx_MFPx;
    uint32_t MFP_Msk;

    if (pin == (PinName)NC)
        halt_on_error_found(__func__, "pin is NC!!", 0);

    pin_index = NU_PINNAME_TO_PIN(pin);
    port_index = NU_PINNAME_TO_PORT(pin);
    GPx_MFPx = ((__IO uint32_t *) &SYS->GPA_MFPL) + port_index * 2 + (pin_index / 8);
    MFP_Msk = NU_MFP_MSK(pin_index);

    // E.g.: SYS->GPA_MFPL  = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA0MFP_Msk) ) | SYS_GPA_MFPL_PA0MFP_SC0_CD  ;
    *GPx_MFPx  = (*GPx_MFPx & (~MFP_Msk)) | data;
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode)
{
    uint32_t pin_index;
    uint32_t port_index;
    GPIO_T *gpio_base;
    uint32_t mode_intern;

    if (pin == (PinName)NC)
        halt_on_error_found(__func__, "pin is NC!!", 0);

    pin_index = NU_PINNAME_TO_PIN(pin);
    port_index = NU_PINNAME_TO_PORT(pin);
    gpio_base = NU_PORT_BASE(port_index);
    mode_intern = GPIO_MODE_INPUT;

    switch (mode)
    {

    case PushPull:
        mode_intern = GPIO_MODE_OUTPUT;
        break;

    case OpenDrain:
        mode_intern = GPIO_MODE_OPEN_DRAIN;
        break;

    case Quasi:
        mode_intern = GPIO_MODE_QUASI;
        break;

    default:
        mode_intern = GPIO_MODE_INPUT;
        break;
    }

    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);
}

void pin_pullctrl(PinName pin, PinPullCtrl pullctl)
{
#if 1
    // Sorry, Not support in this series.
    return;
#else
    uint32_t pin_index;
    uint32_t port_index;
    GPIO_T *gpio_base;
    uint32_t pullctl_intern;

    if (pin == (PinName)NC)
        halt_on_error_found(__func__, "pin is NC!!", 0);

    pin_index = NU_PINNAME_TO_PIN(pin);
    port_index = NU_PINNAME_TO_PORT(pin);
    gpio_base = NU_PORT_BASE(port_index);

    pullctl_intern = GPIO_PUSEL_PULL_UP;

    switch (pullctl)
    {
    case PullUp:
        pullctl_intern = GPIO_PUSEL_PULL_UP;
        break;

    case PullDown:
        pullctl_intern = GPIO_PUSEL_PULL_DOWN;
        break;

    case PullNone:
        pullctl_intern = GPIO_PUSEL_DISABLE;
        break;
    default:
        return;
    }

    GPIO_SetPullCtl(gpio_base, 1 << pin_index, pullctl_intern);
#endif
}
