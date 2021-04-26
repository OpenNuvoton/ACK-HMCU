/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "objects.h"
#include "hal_status.h"

/*
 * Pin configuration
 */
typedef enum
{
    eGPIO_ANALOG,
    eGPIO_IRQ,
    eGPIO_INPUT_PULL_UP,
    eGPIO_INPUT_PULL_DOWN,
    eGPIO_INPUT_HIGH_IMPEDANCE,
    eGPIO_OUTPUT_PUSH_PULL,
    eGPIO_OUTPUT_OPEN_DRAIN_NO_PULL,
    eGPIO_OUTPUT_OPEN_DRAIN_PULL_UP
} E_GPIO_TYPE;

/*
 * GPIO interrupt trigger
 */
typedef enum
{
    eGPIO_IRQ_TRIGGER_RISING_EDGE  = 0x1, /* Interrupt triggered at input signal's rising edge  */
    eGPIO_IRQ_TRIGGER_FALLING_EDGE = 0x2, /* Interrupt triggered at input signal's falling edge */
    eGPIO_IRQ_TRIGGER_BOTH_EDGES   = eGPIO_IRQ_TRIGGER_RISING_EDGE | eGPIO_IRQ_TRIGGER_FALLING_EDGE,
} E_GPIO_IRQ_TRIGGER_TYPE;

typedef struct
{
    PinName     pin;
    uint32_t    mask;
    E_GPIO_TYPE gpio_type;
    uint32_t    irq_handler;
    void       *arg;
} S_GPIODev;

#define NU_MAX_PIN_PER_PORT     16

#ifndef OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE
#define OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE 0
#endif

#ifndef OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST
#define OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST NC
#endif

#ifndef OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE
#define OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE GPIO_DBCTL_DBCLKSRC_LIRC
#endif

#ifndef OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE
#define OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE GPIO_DBCTL_DBCLKSEL_16
#endif

typedef void (*gpio_irq_handler_t)(void *arg);
int32_t HAL_GPIO_Initialize(S_GPIODev *dev);
int32_t HAL_GPIO_OutputHigh(S_GPIODev *dev);
int32_t HAL_GPIO_OutputLow(S_GPIODev *dev);
int32_t HAL_GPIO_OutputToggle(S_GPIODev *dev);
int32_t HAL_GPIO_InputGetValue(S_GPIODev *dev, uint32_t *value);
int32_t HAL_GPIO_EnableIRQ(S_GPIODev *dev, E_GPIO_IRQ_TRIGGER_TYPE trigger, gpio_irq_handler_t handler, void *arg);
int32_t HAL_GPIO_DisableIRQ(S_GPIODev *dev);
int32_t HAL_GPIO_ClearIRQ(S_GPIODev *dev);
int32_t HAL_GPIO_Finalize(S_GPIODev *dev);

#ifdef __cplusplus
}
#endif

#endif
