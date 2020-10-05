/**************************************************************************//**
 * @file     board.c
 * @brief    Smartlight implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
 
#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "numicro_hal.h"
#include "ack_user_config.h"

/* GPIO */
S_GPIODev g_asBoardGpioDev [] =
{
    { .pin = (PinName)HOST_INT, .gpio_type = eGPIO_INPUT_PULL_UP },     //ACK_HW_PIN_HOST_INTERRUPT
    { .pin = (PinName)RESET_B, .gpio_type = eGPIO_OUTPUT_PUSH_PULL },   //ACK_HW_PIN_MODULE_RESET
};
const int i32BoardMaxGPIONum  = sizeof(g_asBoardGpioDev) / sizeof(g_asBoardGpioDev[0]);

S_GPIODev g_asBoardUserGpioDev [] =
{
    // eUserGPIODev_HeartBeat,
    { .pin = (PinName)GPIO_HB,  .gpio_type = eGPIO_OUTPUT_PUSH_PULL }, //Alexa-connected indictor

    // eUserGPIODev_RestoreFactorySetting,
    { .pin = (PinName)GPIO_FS,  .gpio_type = eGPIO_INPUT_PULL_UP },     //Restore factory setting

    // eUserGPIODev_ACK_PowerEnable
    { .pin = (PinName)PWR_EN,   .gpio_type = eGPIO_OUTPUT_PUSH_PULL },  //ACK module power controlling
};
const int i32BoardMaxUserGPIONum  = sizeof(g_asBoardUserGpioDev) / sizeof(g_asBoardUserGpioDev[0]);

/* User-defined */
/* UART */
#define DEF_UART_BUFLEN_DBG 8
static uint8_t au8UartBuf_DBG[DEF_UART_BUFLEN_DBG];
#define DEF_UART_BUFLEN_ACK 512
static uint8_t au8UartBuf_ACK[DEF_UART_BUFLEN_ACK];

void platform_ack_irq_handler_rx(void *psUARTDev, E_UART_EVENT eHalUartEvent);
void platform_ack_irq_handler_tx(void *psUARTDev, E_UART_EVENT eHalUartEvent);

S_UARTDev g_asBoardUartDev[] =
{
    { /* eUartDev_DBG, UART PORT 0, STDIO UART */
        .uart           = UART_0,
        .pin_tx         = (PinName)STDIO_UART_TX,
        .pin_rx         = (PinName)STDIO_UART_RX,
        .pin_rts        = NC,
        .pin_cts        = NC,
        .pu8BufRx       = &au8UartBuf_DBG[0],
        .u32BufSize     = DEF_UART_BUFLEN_DBG,
        .baudrate       = 115200,
        .databits       = eUART_DATA_WIDTH_8BIT,
        .flowctrl       = eUART_FLOW_CONTROL_DISABLED,
        .parity         = eUART_NO_PARITY,
        .stopbits       = eUART_STOP_BITS_1
    },
    { /* eUartDev_ACK, UART PORT 1 */
        .uart           = UART_1,
        .pin_tx         = (PinName)ACK_TX,
        .pin_rx         = (PinName)ACK_RX,
        .pin_rts        = NC,
        .pin_cts        = NC,
        .pu8BufRx       = &au8UartBuf_ACK[0],
        .u32BufSize     = DEF_UART_BUFLEN_ACK,
        .baudrate       = 115200,
        .databits       = eUART_DATA_WIDTH_8BIT,
        .flowctrl       = eUART_FLOW_CONTROL_DISABLED,
        .parity         = eUART_NO_PARITY,
        .stopbits       = eUART_STOP_BITS_1,
        .irq_handler_tx = platform_ack_irq_handler_tx,
        .irq_handler_rx = platform_ack_irq_handler_rx,
    },
};
const int i32BoardMaxUartNum = sizeof(g_asBoardUartDev) / sizeof(g_asBoardUartDev[0]);

S_CRCDev    g_sCrcDev =
{
    .crc          = CRC_0,
    .eCRCMode     = eCRCMode_CRC32,
    .eCRCAttr     = (eCRCAttr_WDataRVS | eCRCAttr_ChecksumCom | eCRCAttr_ChecksumRVS),
    .eCRCCpuWLen  = eCRCCpuWLen_32,
    .u32SeedValue = 0xffffffff
};

S_PWMDev g_asBoardPwmDev[] =
{
    {
        .pwm                = PWM_0_1,
        .pin                = (PinName)PWM0_CH1,
        .frequency          = 10000ul,
        .dutycycle          = 0ul,
        .pinpolar_inverse = 0
    },
    {
        .pwm                = PWM_0_0,
        .pin                = (PinName)PWM0_CH0,
        .frequency          = 10000ul,
        .dutycycle          = 0ul,
        .pinpolar_inverse = 0
    },
};
const int i32BoardMaxPwmNum = sizeof(g_asBoardPwmDev) / sizeof(g_asBoardPwmDev[0]);

void pwmdev_init(void)
{
    int i = 0;
    for (i = 0; i < i32BoardMaxPwmNum; i++)
        HAL_PWM_Initialize(&g_asBoardPwmDev[i]);
}

void pwmdev_deinit(void)
{
    int i = 0;
    for (i = 0; i < i32BoardMaxPwmNum; i++)
        HAL_PWM_Initialize(&g_asBoardPwmDev[i]);
}


void gpiodev_init(void)
{
    int i = 0;
    for (i = 0; i < i32BoardMaxGPIONum; i++)
        HAL_GPIO_Initialize(&g_asBoardGpioDev[i]);

    for (i = 0; i < i32BoardMaxUserGPIONum; i++)
        HAL_GPIO_Initialize(&g_asBoardUserGpioDev[i]);

}

void gpiodev_deinit(void)
{
    int i = 0;
    for (i = 0; i < i32BoardMaxGPIONum; i++)
        HAL_GPIO_Finalize(&g_asBoardGpioDev[i]);

    for (i = 0; i < i32BoardMaxUserGPIONum; i++)
        HAL_GPIO_Finalize(&g_asBoardUserGpioDev[i]);
}

void uartdev_init(void)
{
    int i = 0;
    for (i = 0; i < i32BoardMaxUartNum; i++)
        HAL_UART_Initialize(&g_asBoardUartDev[i]);
}

void uartdev_deinit(void)
{
    int i = 0;
    for (i = 0; i < i32BoardMaxUartNum; i++)
        HAL_UART_Finalize(&g_asBoardUartDev[i]);
}

void crc32dev_init(void)
{
    HAL_CRC_Initialize(&g_sCrcDev);
}

void crc32dev_deinit(void)
{
    HAL_CRC_Finalize(&g_sCrcDev);
}

void peripheral_init(void)
{
    uartdev_init();
    crc32dev_init();
    gpiodev_init();
    pwmdev_init();
}

void peripheral_deinit(void)
{
    gpiodev_deinit();
    crc32dev_deinit();
    uartdev_deinit();
    pwmdev_deinit();
}

void mcu_init(void)
{
    HAL_SYS_Init();
    peripheral_init();

    // Pull-high PWR_EN pin to power-on ACK module.
    HAL_GPIO_OutputHigh(&g_asBoardUserGpioDev[eUserGPIODev_ACK_PowerEnable]);

    printf("\n\nCPU @ %dHz\n", SystemCoreClock);
}