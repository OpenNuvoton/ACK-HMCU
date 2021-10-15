/**************************************************************************//**
 * @file     hal_flash.c
 * @brief    GPIO driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <hal_sys.h>
#include <hal_gpio.h>

struct nu_gpio_irq_var
{
    S_GPIODev         *obj_arr[NU_MAX_PIN_PER_PORT];
    IRQn_Type       irq_n;
    uint32_t        port_index;
};

static void numicro_gpio_irq_enable(S_GPIODev *obj);
static void numicro_gpio_irq(struct nu_gpio_irq_var *var);

//EINT0_IRQn
static struct nu_gpio_irq_var gpio_irq_var_arr[] =
{
    {{NULL}, GPAB_IRQn, 0},
    {{NULL}, GPAB_IRQn, 1},
    {{NULL}, GPCDEF_IRQn, 2},
    {{NULL}, GPCDEF_IRQn, 3},
    {{NULL}, GPCDEF_IRQn, 4},
    {{NULL}, GPCDEF_IRQn, 5},
};
#define NU_MAX_PORT  (sizeof (gpio_irq_var_arr) / sizeof (gpio_irq_var_arr[0]))

void GPA_IRQHandler(void)
{
    numicro_gpio_irq(&gpio_irq_var_arr[0]);    // 16: GPIO Port A
}
void GPB_IRQHandler(void)
{
    numicro_gpio_irq(&gpio_irq_var_arr[1]);    // 17: GPIO Port B
}
void GPC_IRQHandler(void)
{
    numicro_gpio_irq(&gpio_irq_var_arr[2]);    // 18: GPIO Port C
}
void GPD_IRQHandler(void)
{
    numicro_gpio_irq(&gpio_irq_var_arr[3]);    // 19: GPIO Port D
}
void GPE_IRQHandler(void)
{
    numicro_gpio_irq(&gpio_irq_var_arr[4]);    // 20: GPIO Port E
}
void GPF_IRQHandler(void)
{
    numicro_gpio_irq(&gpio_irq_var_arr[5]);    // 21: GPIO Port F
}

void GPAB_IRQHandler(void)
{
    GPA_IRQHandler();
    GPB_IRQHandler();
}

void GPCDEF_IRQHandler(void)
{
    GPC_IRQHandler();
    GPD_IRQHandler();
    GPE_IRQHandler();
    GPF_IRQHandler();
}

static PinName gpio_irq_debounce_arr[] =
{
    OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST
};

static void numicro_gpio_irq(struct nu_gpio_irq_var *var)
{
    // NOTE: GPA_IRQn, GPB_IRQn, ... are not arranged sequentially, so we cannot calculate out port_index through offset from GPA_IRQn.
    //       Instead, we add port_index into gpio_irq_var_arr table.
    uint32_t port_index = var->port_index;
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    uint32_t intsrc = gpio_base->INTSRC;
    uint32_t inten = gpio_base->INTEN;
    while (intsrc)
    {
        int pin_index = nu_ctz(intsrc);
        S_GPIODev *dev = var->obj_arr[pin_index];
        if (dev != NULL)
        {
            if (inten & (GPIO_INT_RISING << pin_index))
            {
                if (GPIO_PIN_DATA(port_index, pin_index))
                {
                    if (dev->irq_handler)
                    {
                        ((gpio_irq_handler_t) dev->irq_handler)(dev->arg);
                    }
                }
            }

            if (inten & (GPIO_INT_FALLING << pin_index))
            {
                if (! GPIO_PIN_DATA(port_index, pin_index))
                {
                    if (dev->irq_handler)
                    {
                        ((gpio_irq_handler_t) dev->irq_handler)(dev->arg);
                    }
                }
            }
        }

        intsrc &= ~(1 << pin_index);
    }
    // Clear all interrupt flags
    gpio_base->INTSRC = gpio_base->INTSRC;
}

static int numicro_gpio_irq_init(S_GPIODev *dev, PinName pin, gpio_irq_handler_t handler, void *arg)
{
    if (pin == NC)
    {
        return -1;
    }

    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    if (pin_index >= NU_MAX_PIN_PER_PORT || port_index >= NU_MAX_PORT)
    {
        return -1;
    }

    dev->pin = pin;
    dev->irq_handler = (uint32_t) handler;
    dev->arg = arg;

    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    // NOTE: In InterruptIn constructor, gpio_irq_init() is called with gpio_init_in() which is responsible for multi-function pin setting.
    //       There is no need to call gpio_set() redundantly.

    {
#if OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE
        // Suppress compiler warning
        (void) gpio_irq_debounce_arr;

        // Configure de-bounce clock source and sampling cycle time
        GPIO_SET_DEBOUNCE_TIME(gpio_base, OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE, OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE);
        GPIO_ENABLE_DEBOUNCE(gpio_base, 1 << pin_index);
#else
        // Enable de-bounce if the pin is in the de-bounce enable list

        // De-bounce defaults to disabled.
        GPIO_DISABLE_DEBOUNCE(gpio_base, 1 << pin_index);

        PinName *debounce_pos = gpio_irq_debounce_arr;
        PinName *debounce_end = gpio_irq_debounce_arr + sizeof(gpio_irq_debounce_arr) / sizeof(gpio_irq_debounce_arr[0]);
        for (; debounce_pos != debounce_end && *debounce_pos != NC; debounce_pos ++)
        {
            uint32_t pin_index_debunce = NU_PINNAME_TO_PIN(*debounce_pos);
            uint32_t port_index_debounce = NU_PINNAME_TO_PORT(*debounce_pos);

            if (pin_index == pin_index_debunce &&
                    port_index == port_index_debounce)
            {
                // Configure de-bounce clock source and sampling cycle time
                GPIO_SET_DEBOUNCE_TIME(OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE, OS_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE);
                GPIO_ENABLE_DEBOUNCE(gpio_base, 1 << pin_index);
                break;
            }
        }
#endif
    }

    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    var->obj_arr[pin_index] = dev;

    // NOTE: InterruptIn requires IRQ enabled by default.
    numicro_gpio_irq_enable(dev);

    return 0;
}

static void numicro_gpio_irq_free(S_GPIODev *dev)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(dev->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    NVIC_DisableIRQ(var->irq_n);
    NU_PORT_BASE(port_index)->INTEN = 0;

    var->obj_arr[pin_index] = NULL;
}

static void numicro_gpio_set(S_GPIODev *dev, E_GPIO_IRQ_TRIGGER_TYPE event, uint32_t enable)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(dev->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    switch (event)
    {
    case eGPIO_IRQ_TRIGGER_FALLING_EDGE:
        if (enable)
        {
            GPIO_EnableInt(gpio_base, pin_index, GPIO_INT_FALLING);
        }
        else
        {
            GPIO_DisableInt(gpio_base, pin_index);
            //gpio_base->INTEN &= ~(GPIO_INT_RISING << pin_index);
        }
        break;

    case eGPIO_IRQ_TRIGGER_RISING_EDGE:
        if (enable)
        {
            GPIO_EnableInt(gpio_base, pin_index, GPIO_INT_RISING);
        }
        else
        {
            GPIO_DisableInt(gpio_base, pin_index);
            //gpio_base->INTEN &= ~(GPIO_INT_FALLING << pin_index);
        }
        break;

    case eGPIO_IRQ_TRIGGER_BOTH_EDGES:
        if (enable)
        {
            GPIO_EnableInt(gpio_base, pin_index, GPIO_INT_BOTH_EDGE);
        }
        else
        {
            GPIO_DisableInt(gpio_base, pin_index);
        }
        break;

    default:
        break;
    }
}

static void numicro_gpio_irq_enable(S_GPIODev *dev)
{
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    HAL_SYS_NVIC_EnableIRQ(var->irq_n);
}

static void numicro_gpio_irq_disable(S_GPIODev *dev)
{
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    HAL_SYS_NVIC_DisableIRQ(var->irq_n);
}

static uint32_t gpio_set(PinName pin)
{
    if (pin == (PinName) NC)
        return 0;

    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);

    pin_function(pin, 0 << NU_MFP_POS(pin_index));

    return (uint32_t)(1 << pin_index);    // Return the pin mask
}

static int gpio_is_connected(const S_GPIODev *dev)
{
    return (dev->pin != (PinName) NC);
}

static void gpio_init(S_GPIODev *dev, PinName pin)
{
    dev->pin = pin;

    if (dev->pin == (PinName) NC)
    {
        return;
    }

    dev->mask = gpio_set(pin);
}

static void gpio_mode(S_GPIODev *dev, PinMode mode)
{
    if (dev->pin == (PinName) NC)
    {
        return;
    }

    pin_mode(dev->pin, mode);
}

static void gpio_pullctrl(S_GPIODev *dev, PinPullCtrl mode)
{
    if (dev->pin == (PinName) NC)
    {
        return;
    }

    pin_pullctrl(dev->pin, mode);
    //pin_mode(dev->pin, mode);
}

static void gpio_dir(S_GPIODev *dev, PinDirection direction)
{
    if (dev->pin == (PinName) NC)
    {
        return;
    }

    uint32_t pin_index = NU_PINNAME_TO_PIN(dev->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    uint32_t mode_intern = GPIO_MODE_INPUT;

    switch (direction)
    {
    case PIN_INPUT:
        mode_intern = GPIO_MODE_INPUT;
        break;

    case PIN_OUTPUT:
        mode_intern = GPIO_MODE_OUTPUT;
        break;

    default:
        return;
    }

    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);
}

void gpio_write(S_GPIODev *dev, int value)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(dev->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);

    GPIO_PIN_DATA(port_index, pin_index) = value ? 1 : 0;
}

int gpio_read(S_GPIODev *dev)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(dev->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(dev->pin);

    return (GPIO_PIN_DATA(port_index, pin_index) ? 1 : 0);
}

/**
 * Initialises a GPIO pin
 *
 * @note  Prepares a GPIO pin for use.
 *
 * @param[in]  gpio           the gpio pin which should be initialised
 * @param[in]  configuration  A structure containing the required gpio configuration
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_Initialize(S_GPIODev *dev)
{
    if (!dev)
        goto exit_hal_gpio_initialize;

    dev->mask = gpio_set(dev->pin);

    switch (dev->gpio_type)
    {

    case eGPIO_ANALOG:
        break;

    case eGPIO_IRQ:
        gpio_dir(dev, PIN_INPUT);
        break;

    case eGPIO_INPUT_PULL_UP:
        gpio_dir(dev, PIN_INPUT);
        gpio_pullctrl(dev, PullUp);
        break;

    case eGPIO_INPUT_PULL_DOWN:
        gpio_dir(dev, PIN_INPUT);
        gpio_pullctrl(dev, PullDown);
        break;

    case eGPIO_INPUT_HIGH_IMPEDANCE:
        gpio_dir(dev, PIN_INPUT);
        gpio_pullctrl(dev, PullNone);
        break;

    case eGPIO_OUTPUT_PUSH_PULL:
//              gpio_dir ( obj, PIN_OUTPUT );
        gpio_mode(dev, PushPull);
        break;

    case eGPIO_OUTPUT_OPEN_DRAIN_NO_PULL:
//              gpio_dir ( obj, PIN_OUTPUT );
        gpio_mode(dev, OpenDrain);
        gpio_pullctrl(dev, PullNone);
        break;

    case eGPIO_OUTPUT_OPEN_DRAIN_PULL_UP:
//              gpio_dir ( obj, PIN_OUTPUT );
        gpio_mode(dev, OpenDrain);
        gpio_pullctrl(dev, PullUp);
        break;

    default:
        goto exit_hal_gpio_initialize;
    } //switch

    return HAL_OK;

exit_hal_gpio_initialize:

    return HAL_ERROR;
}

/**
 * Sets an output GPIO pin high
 *
 * @note  Using this function on a gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set high
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_OutputHigh(S_GPIODev *dev)
{
    gpio_write(dev, 1);
    return HAL_OK;
}

/**
 * Sets an output GPIO pin low
 *
 * @note  Using this function on a gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set low
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_OutputLow(S_GPIODev *dev)
{
    gpio_write(dev, 0);
    return HAL_OK;
}

/**
 * Trigger an output GPIO pin's output. Using this function on a
 * gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set low
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_OutputToggle(S_GPIODev *dev)
{
    (gpio_read(dev) == 1) ? gpio_write(dev, 0) : gpio_write(dev, 1) ;
    return HAL_OK;
}

/**
 * Get the state of an input GPIO pin. Using this function on a
 * gpio pin which is set to output mode will return an undefined value.
 *
 * @param[in]  gpio   the gpio pin which should be read
 * @param[in]  value  gpio value
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_InputGetValue(S_GPIODev *dev, uint32_t *value)
{
    if (value)
    {
        *value = gpio_read(dev);
        return HAL_OK;
    }
    return HAL_ERROR;
}

/**
 * Enables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which is set to
 * output mode is undefined.
 *
 * @param[in]  gpio     the gpio pin which will provide the interrupt trigger
 * @param[in]  trigger  the type of trigger (rising/falling edge)
 * @param[in]  handler  a function pointer to the interrupt handler
 * @param[in]  arg      an argument that will be passed to the interrupt handler
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_EnableIRQ(S_GPIODev *dev, E_GPIO_IRQ_TRIGGER_TYPE trigger,
                           gpio_irq_handler_t handler, void *arg)
{
    if (numicro_gpio_irq_init(dev, dev->pin, handler, arg) < 0)
        goto exit_hal_gpio_enableirq;

    numicro_gpio_set(dev, trigger, 1);

    numicro_gpio_irq_enable(dev);

    return HAL_OK;

exit_hal_gpio_enableirq:

    return HAL_ERROR;
}

/**
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref hal_gpio_input_irq_enable is undefined.
 *
 * @param[in]  gpio  the gpio pin which provided the interrupt trigger
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_DisableIRQ(S_GPIODev *dev)
{
    numicro_gpio_set(dev, eGPIO_IRQ_TRIGGER_BOTH_EDGES, 0);

    numicro_gpio_irq_disable(dev);

    return HAL_OK;
}

/**
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref hal_gpio_input_irq_enable is undefined.
 *
 * @param[in]  gpio  the gpio pin which provided the interrupt trigger
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_ClearIRQ(S_GPIODev *dev)
{
    numicro_gpio_irq_free(dev);
    return HAL_OK;
}

/**
 * Set a GPIO pin in default state.
 *
 * @param[in]  gpio  the gpio pin which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_GPIO_Finalize(S_GPIODev *dev)
{
    return HAL_OK;
}
