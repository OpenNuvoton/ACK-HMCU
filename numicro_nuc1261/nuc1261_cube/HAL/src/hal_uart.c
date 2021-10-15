/**************************************************************************//**
 * @file     hal_uart.c
 * @brief    UART driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#include <hal_uart.h>
#include <nu_rbuf.h>

struct nu_uart_var
{
    S_UARTDev      *dev;
    uint32_t        ref_cnt;                // Reference count of the H/W module
    nu_rbuf_t       fifo_rbuf_rx;
#if DEVICE_UART_TX_DMA
    uint8_t         pdma_perp_tx;
    int8_t          pdma_chanid_tx;
    uint32_t        i32UartDmaTxDone;
#endif
#if DEVICE_UART_RX_DMA
    uint8_t         pdma_perp_rx;
    int8_t          pdma_chanid_rx;
#endif
};

static struct nu_uart_var uart0_var =
{
    .ref_cnt            =   0,
#if DEVICE_UART_TX_DMA
    .pdma_perp_tx       =   PDMA_UART0_TX,
    .pdma_chanid_tx     =   -1,
#endif
#if DEVICE_UART_RX_DMA
    .pdma_perp_rx       =   PDMA_UART0_RX,
    .pdma_chanid_rx     =   -1,
#endif
};
static struct nu_uart_var uart1_var =
{
    .ref_cnt            =   0,
#if DEVICE_UART_TX_DMA
    .pdma_perp_tx       =   PDMA_UART1_TX,
    .pdma_chanid_tx     =   -1,
#endif
#if DEVICE_UART_RX_DMA
    .pdma_perp_rx       =   PDMA_UART1_RX,
    .pdma_chanid_rx     =   -1,
#endif
};
static struct nu_uart_var uart2_var =
{
    .ref_cnt            =   0,
#if DEVICE_UART_TX_DMA
    .pdma_perp_tx       =   PDMA_UART2_TX,
    .pdma_chanid_tx     =   -1,
#endif
#if DEVICE_UART_RX_DMA
    .pdma_perp_rx   =   PDMA_UART2_RX,
    .pdma_chanid_rx     =   -1,
#endif
};


static const struct nu_modinit_s uart_modinit_tab[] =
{
    {UART_0, UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1), UART0_RST, UART02_IRQn, &uart0_var},
    {UART_1, UART1_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1), UART1_RST, UART1_IRQn,  &uart1_var},
    {UART_2, UART2_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1), UART2_RST, UART02_IRQn, &uart2_var},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

#if DEVICE_UART_TX_DMA
    void uart_dma_tx_done_cb(uint32_t idx, uint32_t dma_event);
#endif

#if DEVICE_UART_RX_DMA
    void uart_dma_rx_start(struct nu_uart_var *var);
    void uart_dma_rx_done_cb(uint32_t idx, uint32_t dma_event);
#endif

static uint32_t uart_modinit_mask = 0;

static int push_nu_rbuf(struct nu_uart_var *psNuUartVar, uint8_t *pu8Buf, int len)
{
    int wrote_byte = 0, ret;
    nu_rbuf_t *pnu_rbuf = &psNuUartVar->fifo_rbuf_rx;

    while (wrote_byte < len)
    {
        ret = nu_rbuf_write(pnu_rbuf, &pu8Buf[wrote_byte], (len - wrote_byte));
        if (ret > 0)
            wrote_byte += ret;
        else
            break;
    }

    return wrote_byte;
}

static int UART_GET_FIFO_LENGTH(UART_T *uart_base)
{
    return ((uart_base->FIFOSTS & UART_FIFOSTS_RXPTR_Msk) >> UART_FIFOSTS_RXPTR_Pos);
}

static void hal_uart_rxbuf_irq(struct nu_uart_var *psNuUartVar, uint32_t u32INTSTS)
{
    S_UARTDev *psUARTDev = psNuUartVar->dev;
    UART_T *uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);

    int readable_bytes = UART_GET_FIFO_LENGTH(uart_base);
    int aval_bytes = 0;

    while (readable_bytes > 0)
    {
        if (nu_rbuf_write(&psNuUartVar->fifo_rbuf_rx, (uint8_t *)&uart_base->DAT, 1) != 1)
        {
            // Drop if not empty
            while (!UART_GET_RX_EMPTY(uart_base))
            {
                uint32_t u32ch = (uint8_t)uart_base->DAT;
            }
            break;
        }
        aval_bytes++;
        readable_bytes--;
    }

    if (aval_bytes > 0)
    {
        if (psUARTDev && psUARTDev->irq_handler_rx)
            psUARTDev->irq_handler_rx(psUARTDev, eUARTEvent_Received);
    }
}

static void uart_irq(struct nu_uart_var *psNuUartVar)
{
    S_UARTDev *psUartDev = psNuUartVar->dev;

    if (!psNuUartVar || (psNuUartVar->dev == NULL))
    {
        return;
    }

    volatile UART_T *uart_base = (UART_T *) NU_MODBASE(psUartDev->uart);
    volatile uint32_t u32INTSTS = uart_base->INTSTS;
    volatile uint32_t u32FIFOSTS = uart_base->FIFOSTS;

#if DEVICE_UART_RX_DMA

    if (u32INTSTS & UART_INTSTS_HWRLSIF_Msk)
    {
        uart_base->FIFOSTS |= (UART_FIFOSTS_BIF_Msk | UART_FIFOSTS_FEF_Msk | UART_FIFOSTS_PEF_Msk);
        return;
    }

#endif

    if (u32INTSTS & (UART_INTSTS_RDAINT_Msk | UART_INTSTS_RXTOINT_Msk))
    {
        // Simulate clear of the interrupt flag. Temporarily disable the interrupt here and to be recovered on next read.
        hal_uart_rxbuf_irq(psNuUartVar, u32INTSTS);
    }

    // FIXME: Ignore all other interrupt flags. Clear them. Otherwise, program will get stuck in interrupt.
    uart_base->INTSTS = u32INTSTS;
    uart_base->FIFOSTS = u32FIFOSTS;

}

void UART02_IRQHandler(void)
{
    uart_irq(&uart0_var);
    uart_irq(&uart2_var);
}

void UART1_IRQHandler(void)
{
    uart_irq(&uart1_var);
}


/**
 * Configure UART flowcontrol
 *
 *
 * @param[in]  pUart    the UART register base of NuMicro
 * @param[in]  hufc_t   the flowcontrol setting.
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_SetFlowControl(S_UARTDev *psUartDev, E_UART_FLOW_CONTROL hufc_t)
{
    UART_T *pUart = NULL;

    if (!psUartDev)
        goto exit_hal_uart_setflowcontol;

    pUart = (UART_T *) NU_MODBASE(psUartDev->uart);

    // Disable to CTS/RTS first.
    UART_DISABLE_INT(pUart, UART_INTEN_ATORTSEN_Msk | UART_INTEN_ATOCTSEN_Msk);

    switch (hufc_t)
    {
    case eUART_FLOW_CONTROL_DISABLED:
        break;

    case eUART_FLOW_CONTROL_CTS:  //CTS only
        /* Set CTS pin input is low level active */
        pUart->MODEMSTS |= UART_MODEMSTS_CTSACTLV_Msk;

        /* Set CTS auto flow control enable */
        UART_ENABLE_INT(pUart, UART_INTEN_ATOCTSEN_Msk);
        break;

    case eUART_FLOW_CONTROL_RTS:  //RTS only
        /* Set RTS pin output is low level active */
        pUart->MODEM |= UART_MODEM_RTSACTLV_Msk;
        pUart->FIFO = (pUart->FIFO & ~ UART_FIFO_RTSTRGLV_Msk) | UART_FIFO_RTSTRGLV_14BYTES;

        /* Set RTS auto flow control enable */
        UART_ENABLE_INT(pUart, UART_INTEN_ATORTSEN_Msk);
        break;

    case eUART_FLOW_CONTROL_CTS_RTS:
        /* Set RTS pin output is low level active */
        pUart->MODEM |= UART_MODEM_RTSACTLV_Msk;

        /* Set CTS pin input is low level active */
        pUart->MODEMSTS |= UART_MODEMSTS_CTSACTLV_Msk;
        pUart->FIFO = (pUart->FIFO & ~ UART_FIFO_RTSTRGLV_Msk) | UART_FIFO_RTSTRGLV_14BYTES;

        /* Set RTS and CTS auto flow control enable */
        UART_ENABLE_INT(pUart, UART_INTEN_ATORTSEN_Msk | UART_INTEN_ATOCTSEN_Msk);
        break;

    default:
        goto exit_hal_uart_setflowcontol;
    }

    return HAL_OK;

exit_hal_uart_setflowcontol:

    return HAL_ERROR;
}

/**
 * Configure UART line configuration
 *
 *
 * @param[in]  pUart    the UART register base of NuMicro
 * @param[in]  hufc_t   the flowcontrol setting.
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_SetLineConf(S_UARTDev *psUartDev, E_UART_DATA_WIDTH udw, E_UART_STOP_BITS usb, E_UART_PARITY up)
{
    uint32_t u32data_width;
    uint32_t u32parity;
    uint32_t u32stop_bits;
    UART_T *pUart = NULL;

    if (!psUartDev)
        goto exit_hal_uart_setlineconf;

    pUart = (UART_T *) NU_MODBASE(psUartDev->uart);

    // Check data width parameters
    switch (udw)
    {
    case eUART_DATA_WIDTH_5BIT:
        u32data_width = UART_WORD_LEN_5;
        break;

    case eUART_DATA_WIDTH_6BIT:
        u32data_width = UART_WORD_LEN_6;
        break;

    case eUART_DATA_WIDTH_7BIT:
        u32data_width = UART_WORD_LEN_7;
        break;

    case eUART_DATA_WIDTH_8BIT:
        u32data_width = UART_WORD_LEN_8;
        break;

    default:
        goto exit_hal_uart_setlineconf;
    }

    // Check stop bit parameters
    switch (usb)
    {
    case eUART_STOP_BITS_1:
        u32stop_bits = UART_STOP_BIT_1;
        break;

    case eUART_STOP_BITS_2:
        u32stop_bits = UART_STOP_BIT_2;
        break;

    default:
        goto exit_hal_uart_setlineconf;
    }

    // Check parity parameters
    switch (up)
    {
    case eUART_NO_PARITY:
        u32parity = UART_PARITY_NONE;
        break;

    case eUART_ODD_PARITY:
        u32parity = UART_PARITY_ODD;
        break;

    case eUART_EVEN_PARITY:
        u32parity = UART_PARITY_EVEN;
        break;

    default:
        goto exit_hal_uart_setlineconf;
    }

    // Dont change baudrate setting.
    UART_SetLine_Config(pUart, 0, u32data_width, u32parity, u32stop_bits);

    return HAL_OK;

exit_hal_uart_setlineconf:
    return HAL_ERROR;
}

/**
 * Initialises a UART interface
 *
 *
 * @param[in]  uart  the interface which should be initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_Initialize(S_UARTDev *psUartDev)
{
    struct nu_modinit_s *modinit;
    struct nu_uart_var *var;
    UARTName s_UartName;
    UART_T *pUart;

    if (!psUartDev)
        goto exit_hal_uart_initialize;

    // found UARTNAME from the two PinName.
    uint32_t uart_tx = pinmap_peripheral(psUartDev->pin_tx, psUartDev->uart, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(psUartDev->pin_rx, psUartDev->uart, PinMap_UART_RX);
    uint32_t uart_data = NC;

    // Get final UARTName
    uart_data = (UARTName) pinmap_merge(uart_tx, uart_rx);

    if (uart_data == NC)
        goto exit_hal_uart_initialize;

    uint32_t uart_cts = NC;
    uint32_t uart_rts = NC;
    uint32_t uart_flowctl = NC;

    if (psUartDev->pin_cts != NC)
        uart_cts = pinmap_peripheral(psUartDev->pin_cts, psUartDev->uart, PinMap_UART_CTS);

    if (psUartDev->pin_rts != NC)
        uart_rts = pinmap_peripheral(psUartDev->pin_rts, psUartDev->uart, PinMap_UART_RTS);

    if ((psUartDev->pin_cts != NC) || (psUartDev->pin_rts != NC))
    {
        uart_flowctl = pinmap_merge(uart_cts, uart_rts);
        s_UartName = (UARTName)  pinmap_merge(uart_data, uart_flowctl);
    }
    else
        s_UartName = (UARTName) uart_data;

    if (s_UartName == NC)
        goto exit_hal_uart_initialize;

    // Find entry by UARTNAME
    if (!(modinit = (struct nu_modinit_s *) get_modinit(s_UartName, uart_modinit_tab)))
        goto exit_hal_uart_initialize;

    var = (struct nu_uart_var *) modinit->var;

    if (! var->ref_cnt)
    {

#if DEVICE_UART_TX_DMA

        if (psUartDev->irq_handler_tx)
        {
            var->pdma_chanid_tx         =       HAL_DMA_AllocateChannel(0);

            if (var->pdma_chanid_tx < 0)
                goto exit_hal_uart_initialize;

            HAL_DMA_SetHandler(var->pdma_chanid_tx,
                               (uint32_t)uart_dma_tx_done_cb,
                               (uint32_t)var,
                               DMA_EVENT_TRANSFER_DONE | DMA_EVENT_TIMEOUT);
        }

#endif
#if DEVICE_UART_RX_DMA

        if (psUartDev->irq_handler_rx)
        {
            var->pdma_chanid_rx         =       HAL_DMA_AllocateChannel(0);

            if (var->pdma_chanid_rx < 0)
                goto exit_hal_uart_initialize;

            HAL_DMA_SetHandler(var->pdma_chanid_rx,
                               (uint32_t)uart_dma_rx_done_cb,
                               (uint32_t)var,
                               DMA_EVENT_TRANSFER_DONE | DMA_EVENT_TIMEOUT);
        }

#endif

        do
        {
            SYS_UnlockReg();

            // Reset this module
            SYS_ResetModule(modinit->rsetidx);

            // Select IP clock source
            CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);

            // Enable IP clock
            CLK_EnableModuleClock(modinit->clkidx);

            pinmap_pinout(psUartDev->pin_tx, PinMap_UART_TX);
            pinmap_pinout(psUartDev->pin_rx, PinMap_UART_RX);

            if (psUartDev->pin_rts != NC)
                pinmap_pinout(psUartDev->pin_rts, PinMap_UART_RTS);

            if (psUartDev->pin_cts != NC)
                pinmap_pinout(psUartDev->pin_cts, PinMap_UART_CTS);

        }
        while (0);

        if (nu_rbuf_create(&var->fifo_rbuf_rx, psUartDev->pu8BufRx, psUartDev->u32BufSize) != 0)
            goto exit_hal_uart_initialize;

#if DEVICE_UART_TX_DMA

#endif
        /* Link parent and children. */
        var->dev = psUartDev;

        // Get Uart base address
        pUart = (UART_T *) NU_MODBASE(s_UartName);

        /* Set UART Baudrate */
        UART_Open(pUart, psUartDev->baudrate);

        // Set line configuration
        if (HAL_UART_SetLineConf(psUartDev, psUartDev->databits, psUartDev->stopbits, psUartDev->parity) < 0)
            goto exit_hal_uart_initialize;

        // Set flow-control parameters
        if (HAL_UART_SetFlowControl(psUartDev, psUartDev->flowctrl) < 0)
            goto exit_hal_uart_initialize;

#if DEVICE_UART_RX_DMA

        if (var->pdma_chanid_rx  != -1)
        {
            //                  HAL_SYS_NVIC_DisableIRQ(modinit->irq_n);
            HAL_SYS_NVIC_EnableIRQ(modinit->irq_n);
            pUart->FIFO &= (~UART_FIFO_RFITL_Msk);
            pUart->FIFO |= UART_FIFO_RFITL_1BYTE;
            UART_ENABLE_INT(pUart, (UART_INTEN_RLSIEN_Msk)); // Enable Receive Line interrupt
            uart_dma_rx_start(var);
        }
        else
#endif
        {
            HAL_SYS_NVIC_EnableIRQ(modinit->irq_n);
            pUart->FIFO &= (~UART_FIFO_RFITL_Msk);
            pUart->FIFO |= UART_FIFO_RFITL_1BYTE;
            UART_SetTimeoutCnt(pUart, 40);
            /* Enable Interrupt */
            UART_ENABLE_INT(pUart, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));
        }
    } //if (! var->ref_cnt)

    var->ref_cnt ++;

    if (var->ref_cnt)
    {
        // Mark this module to be inited.
        int i = modinit - uart_modinit_tab;
        uart_modinit_mask |= 1 << i;
    }

    return HAL_OK;

exit_hal_uart_initialize:

    if (pUart)
        UART_Close(pUart);

    return HAL_ERROR;
}

#if DEVICE_UART_TX_DMA
void uart_dma_tx_done_cb(uint32_t idx, uint32_t dma_event)
{
    struct nu_uart_var *var = (struct nu_uart_var *)idx;
    S_UARTDev *psUARTDev = var->dev;
    UART_T *uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);

    if (dma_event & DMA_EVENT_TRANSFER_DONE)
    {
        psUARTDev->irq_handler_tx(psUARTDev, eUARTEvent_SendCompleted);
        var->i32UartDmaTxDone = 1;
    }

    if (dma_event & (DMA_EVENT_ABORT | DMA_EVENT_TIMEOUT))
        while (1);

    UART_DISABLE_INT(uart_base, UART_INTEN_TXPDMAEN_Msk);
}
#endif

static int32_t platform_uart_write(struct nu_uart_var *var, uint8_t pu8TxBuf[], uint32_t u32WriteBytes, uint32_t timeout)
{
    uint32_t tx_count = 0;
    S_UARTDev *psUARTDev = var->dev;
    UART_T *uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);

#if DEVICE_UART_TX_DMA

    if ((u32WriteBytes <= 16) || ((uint32_t)pu8TxBuf % 4 != 0) || var->pdma_chanid_tx == -1)
#endif
    {
        //if( psUARTDev->irq_handler_tx )
        //  printf("CPU W->%d\r\n", u32WriteBytes);

        UART_DISABLE_INT(uart_base, UART_INTEN_TXPDMAEN_Msk);  // Stop DMA transfer

        if (UART_Write(uart_base, (uint8_t *)pu8TxBuf, u32WriteBytes) == u32WriteBytes)
            tx_count = u32WriteBytes;

#if DEVICE_UART_TX_DMA

        if (psUARTDev && psUARTDev->irq_handler_tx)
            psUARTDev->irq_handler_tx(psUARTDev, eUARTEvent_SendCompleted);

#endif
    }

#if DEVICE_UART_TX_DMA
    else
    {
        S_UARTDev *psUARTDev = var->dev;
        uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);

        // printf("DMA W->%d\r\n", u32WriteBytes);

        HAL_DMA_FillDescription(var->pdma_chanid_tx,
                                (uint32_t)var->pdma_perp_tx,
                                8,
                                (uint32_t)&pu8TxBuf[0],
                                (uint32_t)uart_base,
                                u32WriteBytes,
                                0);

        //UART_DISABLE_INT(uart_base, UART_INTEN_THREIEN_Msk);
        var->i32UartDmaTxDone = 0;
        UART_ENABLE_INT(uart_base, UART_INTEN_TXPDMAEN_Msk);  // Start DMA transfer
        long leave_ms = HAL_SYS_TICK_Get() + timeout;
        while (!var->i32UartDmaTxDone)
            if ((HAL_SYS_TICK_Get() >= leave_ms)) break;

        if (var->i32UartDmaTxDone)
            tx_count = u32WriteBytes;
    }


#endif
    while ((uart_base->FIFOSTS & UART_FIFOSTS_TXEMPTY_Msk) == 0);

    //platform_dump_buffer(pu8TxBuf, tx_count, 0);

    return tx_count;
}

static void uart_dma_rx_debug(struct nu_uart_var *var);

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
int32_t HAL_UART_Send(S_UARTDev *psUartDev, const void *data, uint32_t size, uint32_t timeout)
{
    struct nu_modinit_s *modinit;
    struct nu_uart_var *var;
    int32_t ret = -1;

    if (!psUartDev)
        goto exit_hal_uart_send;

    modinit = (struct nu_modinit_s *)get_modinit(psUartDev->uart, uart_modinit_tab);

    /* Valid? */
    if (!modinit) goto exit_hal_uart_send;

    var = modinit->var;

    /* Initialized? */
    if (!var->ref_cnt) goto exit_hal_uart_send;

    if (platform_uart_write(var, (uint8_t *)data, size, timeout) == size)
        ret = 0;

exit_hal_uart_send:

    return ret;
}

#if DEVICE_UART_RX_DMA
void uart_dma_rx_start(struct nu_uart_var *var)
{
    S_UARTDev *psUARTDev = var->dev;
    UART_T *uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);
    int trigger_len = psUARTDev->u32BufSize / DEF_DMA_RXBUF_CHUNKNUM;
    int toggle_ctl = var->fifo_rbuf_rx.chunk_idx;
    uint8_t *pu8Buf = &var->fifo_rbuf_rx.buffer[trigger_len * toggle_ctl];

    HAL_DMA_FillDescription(var->pdma_chanid_rx,
                            (uint32_t)var->pdma_perp_rx,
                            8,
                            (uint32_t)uart_base,
                            (uint32_t)pu8Buf,
                            trigger_len, //half buffer
                            5000);
    UART_ENABLE_INT(uart_base, UART_INTEN_RXPDMAEN_Msk); // Start DMA transfer
}

void uart_dma_rx_stop(struct nu_uart_var *var)
{
    S_UARTDev *psUARTDev = var->dev;
    UART_T *uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);
    UART_DISABLE_INT(uart_base, UART_INTEN_RXPDMAEN_Msk); // Stop DMA transfer
}

void uart_dma_rx_done_cb(uint32_t idx, uint32_t dma_event)
{
    struct nu_uart_var *var = (struct nu_uart_var *)idx;
    S_UARTDev *psUARTDev = var->dev;
    UART_T *uart_base = (UART_T *) NU_MODBASE(psUARTDev->uart);
    int cur = var->fifo_rbuf_rx.chunk_idx;
    int trigger_len = psUARTDev->u32BufSize / DEF_DMA_RXBUF_CHUNKNUM;
    int transferred_byte_count = HAL_DMA_GetTransferredByteCount(var->pdma_chanid_rx, trigger_len);
    int i32RBufWOff = 0;

    if (dma_event & (DMA_EVENT_TIMEOUT | DMA_EVENT_TRANSFER_DONE))
    {
        if (dma_event & DMA_EVENT_TRANSFER_DONE)   //Transfer done
        {
            transferred_byte_count = trigger_len;
            uart_dma_rx_stop(var);
            var->fifo_rbuf_rx.chunk_idx = (var->fifo_rbuf_rx.chunk_idx + 1) % DEF_DMA_RXBUF_CHUNKNUM;
            uart_dma_rx_start(var);    // trigger next
        }
        else if ((dma_event & DMA_EVENT_TIMEOUT) && !UART_GET_RX_EMPTY(uart_base))
        {
            return;
        }

        i32RBufWOff = var->fifo_rbuf_rx.tail - (trigger_len * cur);

        if (transferred_byte_count > i32RBufWOff)
        {
            nu_rbuf_dummy_write(&var->fifo_rbuf_rx, transferred_byte_count - i32RBufWOff);

            if (psUARTDev && psUARTDev->irq_handler_rx)
                psUARTDev->irq_handler_rx(psUARTDev, eUARTEvent_Received);
        }
    }

    if (dma_event & DMA_EVENT_ABORT)
        while (1);

}


void UART_DisableRxTimeout(UART_T *uart)
{
    /* Disable time-out counter enable */
    uart->INTEN &= ~UART_INTEN_TOCNTEN_Msk;
}

#endif

static int32_t platform_uart_read(struct nu_uart_var *var, uint8_t pu8RxBuf[], uint32_t u32ReadBytes, uint32_t timeout)
{
    S_UARTDev *psUartDev = var->dev;
    nu_rbuf_t *prbuf = &var->fifo_rbuf_rx;
    uint32_t rx_count = 0;
    size_t rev_size = 0;
    int32_t ret;
    long long leave_ms = HAL_SYS_TICK_Get() + timeout;

    //printf("[%06d]%s %x: %d %d, to=%d. in\r\n", HAL_SYS_TICK_Get(), __func__, var, rx_count, u32ReadBytes, timeout);
    while (rx_count < u32ReadBytes)
    {
        rev_size = u32ReadBytes - rx_count;
        rx_count += nu_rbuf_read(prbuf, &pu8RxBuf[rx_count], rev_size);

        if ((rx_count == u32ReadBytes) || (HAL_SYS_TICK_Get() >= leave_ms)) break;

        //printf("[%06d]%x: %d %d Wait.\r\n", (unsigned)HAL_SYS_TICK_Get(), var, rx_count, u32ReadBytes );
        //uart_dma_rx_debug(var);

        if (1 == 1)   //Timeout?
        {
            //printf("[%06d]Wait %d.\r\n", (unsigned)HAL_SYS_TICK_Get(), ret );
            //printf("[%06d]Got %d.\r\n", (unsigned)HAL_SYS_TICK_Get(), ret );
            //nu_rbuf_dump ( &var->fifo_rbuf_rx );
            continue;
        }
        else
            break;
    }

    return rx_count;
}

/**
 * Receive data on a UART interface
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
int32_t HAL_UART_Recv(S_UARTDev *psUartDev, void *data, uint32_t expect_size, uint32_t *recv_size, uint32_t timeout)
{
    struct nu_modinit_s *modinit;
    struct nu_uart_var *var;
    int count = 0;

    if (!psUartDev || !recv_size)
        goto exit_hal_uart_recv;

    modinit = (struct nu_modinit_s *)get_modinit(psUartDev->uart, uart_modinit_tab);

    /* Valid? */
    if (!modinit) goto exit_hal_uart_recv;

    var = (struct nu_uart_var *)modinit->var;

    /* Initialized? */
    if (!var->ref_cnt) goto exit_hal_uart_recv;

    //printf("[%06d]before-%d.\r\n", (unsigned)HAL_SYS_TICK_Get(), timeout );
    *recv_size = platform_uart_read(var, (uint8_t *)data, expect_size, timeout);
    //printf("[%06d]after-%d. %d\r\n", (unsigned)HAL_SYS_TICK_Get(), timeout, *recv_size );

    return HAL_OK;

exit_hal_uart_recv:

    *recv_size = 0;
    return HAL_ERROR;
}

int32_t HAL_UART_DrainRxBuf(S_UARTDev *psUartDev)
{
    struct nu_modinit_s *modinit;
    struct nu_uart_var *var;
    int i32Dummy;
    nu_rbuf_t *prbuf ;

    if (!psUartDev)
        goto exit_hal_uart_drainrxbuf;

    modinit = (struct nu_modinit_s *)get_modinit(psUartDev->uart, uart_modinit_tab);

    /* Valid? */
    if (!modinit) goto exit_hal_uart_drainrxbuf;

    var = (struct nu_uart_var *)modinit->var;

    /* Initialized? */
    if (!var->ref_cnt) goto exit_hal_uart_drainrxbuf;

    prbuf = &var->fifo_rbuf_rx;
    i32Dummy = nu_rbuf_avail_read_space(prbuf);

    if (i32Dummy > 0)
    {
        printf("Drained %d bytes\n", i32Dummy);
        nu_rbuf_dummy_read(prbuf, i32Dummy);
    }
    return HAL_OK;

exit_hal_uart_drainrxbuf:

    return HAL_ERROR;
}

/**
 * Deinitialises a UART interface
 *
 * @param[in]  uart  the interface which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t HAL_UART_Finalize(S_UARTDev *psUartDev)
{
    struct nu_modinit_s *modinit;
    struct nu_uart_var *var;
    UART_T *pUart;

    if (!psUartDev)
        goto exit_hal_uart_finalize;

    modinit = (struct nu_modinit_s *)get_modinit(psUartDev->uart, uart_modinit_tab);

    /* Valid? */
    if (!modinit) goto exit_hal_uart_finalize;

    var = (struct nu_uart_var *)modinit->var;

    /* Initialized? */
    if (!var->ref_cnt) goto exit_hal_uart_finalize;

    pUart = (UART_T *) NU_MODBASE(psUartDev->uart);

    var->ref_cnt --;

    if (! var->ref_cnt)
    {

        do
        {
            UART_Close(pUart);

            //UART_DISABLE_INT(pUart, (UART_INTEN_RDAIEN_Msk | UART_INTEN_THREIEN_Msk | UART_INTEN_RXTOIEN_Msk));
            HAL_SYS_NVIC_DisableIRQ(modinit->irq_n);

            // Disable IP clock
            CLK_DisableModuleClock(modinit->clkidx);
        }
        while (0);

        nu_rbuf_destroy(&var->fifo_rbuf_rx);

        /* Unlink parent and children. */
        var->dev = NULL;
    }

    if (! var->ref_cnt)
    {
        // Mark this module to be deinited.
        int i = modinit - uart_modinit_tab;
        uart_modinit_mask &= ~(1 << i);
    }

    return HAL_OK;

exit_hal_uart_finalize:

    return HAL_ERROR;
}
