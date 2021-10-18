/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "objects.h"
#include "hal_status.h"

#define DEVICE_UART_TX_DMA 1
#define DEVICE_UART_RX_DMA 1
#define DEF_DMA_RXBUF_CHUNKNUM  8

#if DEVICE_UART_TX_DMA || DEVICE_UART_RX_DMA
#include <hal_dma.h>
#endif

/*
 * UART data width
 */
typedef enum
{
    eUART_DATA_WIDTH_5BIT,
    eUART_DATA_WIDTH_6BIT,
    eUART_DATA_WIDTH_7BIT,
    eUART_DATA_WIDTH_8BIT
} E_UART_DATA_WIDTH;

/*
 * UART stop bits
 */
typedef enum
{
    eUART_STOP_BITS_1,
    eUART_STOP_BITS_2
} E_UART_STOP_BITS;

/*
 * UART flow control
 */
typedef enum
{
    eUART_FLOW_CONTROL_DISABLED,
    eUART_FLOW_CONTROL_CTS,
    eUART_FLOW_CONTROL_RTS,
    eUART_FLOW_CONTROL_CTS_RTS
} E_UART_FLOW_CONTROL;

/*
 * UART parity
 */
typedef enum
{
    eUART_NO_PARITY,
    eUART_ODD_PARITY,
    eUART_EVEN_PARITY
}  E_UART_PARITY;

/*
 * UART mode
 */
typedef enum
{
    eUART_MODE_TX,
    eUART_MODE_RX,
    eUART_MODE_TX_RX
}  E_UART_MODE;

/*
 * UART mode
 */
typedef enum
{
    eUARTEvent_SendCompleted,
    eUARTEvent_Received
} E_UART_EVENT;


typedef struct
{
    UARTName    uart;
    PinName     pin_tx;
    PinName     pin_rx;
    PinName     pin_rts;
    PinName     pin_cts;

    uint32_t baudrate;
    E_UART_DATA_WIDTH databits;
    E_UART_PARITY parity;
    E_UART_STOP_BITS stopbits;
    E_UART_FLOW_CONTROL flowctrl;

    uint8_t    *pu8BufRx;
    uint32_t    u32BufSize;

    void (*irq_handler_tx)(void *psUARTDev, E_UART_EVENT eHalUartEvent);
    void (*irq_handler_rx)(void *psUARTDev, E_UART_EVENT eHalUartEvent);
} S_UARTDev;

/**
 * Configure UART flowcontrol
 *
 *
 * @param[in]  pUart    the UART register base of NuMicro
 * @param[in]  hufc_t   the flowcontrol setting.
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_SetFlowControl(S_UARTDev *psUartDev, E_UART_FLOW_CONTROL hufc_t);

/**
 * Configure UART line configuration
 *
 *
 * @param[in]  pUart    the UART register base of NuMicro
 * @param[in]  hufc_t   the flowcontrol setting.
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_SetLineConf(S_UARTDev *psUartDev, E_UART_DATA_WIDTH udw, E_UART_STOP_BITS usb, E_UART_PARITY up);

/**
 * Initialises a UART interface
 *
 *
 * @param[in]  uart  the interface which should be initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_Initialize(S_UARTDev *psUartDev);

/**
 * Transmit data on a UART interface
 *
 * @param[in]  uart     the UART interface
 * @param[in]  data     pointer to the start of data
 * @param[in]  size     number of bytes to transmit
 * @param[in]  timeout  timeout in milisecond, set this value to HAL_WAIT_FOREVER
 *                      if you want to wait forever
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_Send(S_UARTDev *psUartDev, const void *data, uint32_t size, uint32_t timeout);

/**
 * Receive data on an UART interface
 *
 * @param[in]   uart         the UART interface
 * @param[out]  data         pointer to the buffer which will store incoming data
 * @param[in]   expect_size  number of bytes to receive
 * @param[out]  recv_size    number of bytes received
 * @param[in]   timeout      timeout in milisecond, set this value to HAL_WAIT_FOREVER
 *                           if you want to wait forever
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_Recv(S_UARTDev *psUartDev, void *data, uint32_t expect_size, uint32_t *recv_size, uint32_t timeout);


int32_t HAL_UART_DrainRxBuf(S_UARTDev *psUartDev);

/**
 * Deinitialises a UART interface
 *
 * @param[in]  uart  the interface which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_Finalize(S_UARTDev *psUartDev);

#ifdef __cplusplus
}
#endif

#endif
