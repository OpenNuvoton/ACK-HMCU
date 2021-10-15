/**************************************************************************//**
 * @file     hal_dma.c
 * @brief    PDMA driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <hal_dma.h>

#define NU_PDMA_CH_MAX      PDMA_CH_MAX     /* Specify maximum channels of PDMA */
#define NU_PDMA_CH_Pos      0               /* Specify first channel number of PDMA */
#define NU_PDMA_CH_Msk      (((1 << NU_PDMA_CH_MAX) - 1) << NU_PDMA_CH_Pos)

struct nu_dma_chn_s
{
    void (*handler)(uint32_t, uint32_t);
    uint32_t    id;
    uint32_t    event;
    uint32_t        timeout_us;
};

static volatile int dma_inited = 0;
static volatile uint32_t dma_chn_mask = 0;
static struct nu_dma_chn_s dma_chn_arr[NU_PDMA_CH_MAX];
static const struct nu_modinit_s dma_modinit =
{
    DMA_0, PDMA_MODULE, 0, 0, PDMA_RST, PDMA_IRQn, (void *) &dma_chn_arr[0]
};

typedef struct
{
    uint32_t m_u32Peripheral;
    uint32_t m_u32SrcCtl;
    uint32_t m_u32DstCtl;
} S_PDMA_Peripheral_Ctl;

const S_PDMA_Peripheral_Ctl sPDMAPeripheralCtl[ ] =
{
    { PDMA_MEM, PDMA_SAR_INC, PDMA_DAR_INC },

    // M2P
    { PDMA_UART0_TX, PDMA_SAR_INC, PDMA_DAR_FIX },
    { PDMA_UART1_TX, PDMA_SAR_INC, PDMA_DAR_FIX },
    { PDMA_UART2_TX, PDMA_SAR_INC, PDMA_DAR_FIX },
    { PDMA_SPI0_TX, PDMA_SAR_INC, PDMA_DAR_FIX },
    { PDMA_SPI1_TX, PDMA_SAR_INC, PDMA_DAR_FIX },
    { PDMA_I2C0_TX, PDMA_SAR_INC, PDMA_DAR_FIX },
    { PDMA_I2C1_TX, PDMA_SAR_INC, PDMA_DAR_FIX },

    // P2M
    { PDMA_UART0_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_UART1_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_UART2_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_SPI0_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_SPI1_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_I2C0_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_I2C1_RX, PDMA_SAR_FIX, PDMA_DAR_INC },

    { PDMA_ADC_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_PWM0_P1_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_PWM0_P2_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_PWM0_P3_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_PWM1_P1_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_PWM1_P2_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { PDMA_PWM1_P3_RX, PDMA_SAR_FIX, PDMA_DAR_INC },
    { 0 }
};

static PDMA_T *HAL_DMA_GetModBase(void)
{
    return (PDMA_T *) NU_MODBASE(dma_modinit.modname);
}

static int HAL_DMA_GetPeripheralControl(uint32_t u32Peripheral, uint32_t *u32SrcCtl, uint32_t *u32DstCtl)
{
    int idx = 0;

    while (idx < (sizeof(sPDMAPeripheralCtl) / sizeof(sPDMAPeripheralCtl[0])))
    {
        if (sPDMAPeripheralCtl[idx].m_u32Peripheral == u32Peripheral)
        {
            *u32SrcCtl = sPDMAPeripheralCtl[idx].m_u32SrcCtl;
            *u32DstCtl = sPDMAPeripheralCtl[idx].m_u32DstCtl;
            return 1;
        }
        idx++;
    }

    return 0;
}

static void HAL_DMA_Init(void)
{
    PDMA_T *pdma = NULL;

    if (dma_inited)
        return;

    /* Unlock register lock protect */
    SYS_UnlockReg();

    pdma = HAL_DMA_GetModBase();

    dma_chn_mask = ~NU_PDMA_CH_Msk;
    memset(dma_chn_arr, 0x00, sizeof(dma_chn_arr));

    // Enable IP clock
    CLK_EnableModuleClock(dma_modinit.clkidx);

    // Reset this module
    SYS_ResetModule(dma_modinit.rsetidx);

    PDMA_Open(pdma, 0);

    //HAL_SYS_NVIC_SetVector(dma_modinit.irq_n, (uint32_t) dma_modinit.var);
    HAL_SYS_NVIC_EnableIRQ(dma_modinit.irq_n);

    dma_inited = 1;
}

static void HAL_DMA_EnableChannel(PDMA_T *pdma, int channelid)
{
    pdma->CHCTL |= (1 << channelid);
}

static void HAL_DMA_DisableChannel(PDMA_T *pdma, int channelid)
{
    pdma->CHCTL &= ~(1 << channelid);
}

int HAL_DMA_AllocateChannel(uint32_t capabilities)
{
    PDMA_T *pdma = NULL;
    HAL_DMA_Init();

    pdma = HAL_DMA_GetModBase();

    int i = nu_cto(dma_chn_mask);

    if (i != 32)
    {
        dma_chn_mask |= 1 << i;
        memset(dma_chn_arr + i - NU_PDMA_CH_Pos, 0x00, sizeof(struct nu_dma_chn_s));
        return i;
    }

    // No channel available
    return HAL_ERROR;
}

int HAL_DMA_FreeChannel(int channelid)
{
    if (! dma_inited)
    {
        return 0;
    }

    if (channelid != HAL_ERROR)
    {
        dma_chn_mask &= ~(1 << channelid);
    }

    dma_inited = 0;
    return 0;
}

int HAL_DMA_SetHandler(int channelid, uint32_t handler, uint32_t id, uint32_t event)
{
    PDMA_T *pdma;

    if (!(dma_chn_mask & (1 << channelid)))
        goto exit_hal_dma_sethandler;

    pdma = HAL_DMA_GetModBase();
    dma_chn_arr[channelid - NU_PDMA_CH_Pos].handler = (void (*)(uint32_t, uint32_t)) handler;
    dma_chn_arr[channelid - NU_PDMA_CH_Pos].id = id;
    dma_chn_arr[channelid - NU_PDMA_CH_Pos].event = event;

    return HAL_OK;

exit_hal_dma_sethandler:

    return HAL_ERROR;
}

int HAL_DMA_GetTransferredByteCount(int channelid, int trigger_bytelen)
{
    PDMA_T *pdma;
    int i32BitWidth = 0;
    int cur_txcnt = 0;

    if (!(dma_chn_mask & (1 << channelid)))
        return 0;

    pdma = HAL_DMA_GetModBase();

    i32BitWidth = pdma->DSCT[channelid].CTL & PDMA_DSCT_CTL_TXWIDTH_Msk;
    i32BitWidth = (i32BitWidth == PDMA_WIDTH_8) ? 1 : (i32BitWidth == PDMA_WIDTH_16) ? 2 : (i32BitWidth == PDMA_WIDTH_32) ? 4 : 0;

    cur_txcnt = ((pdma->DSCT[channelid].CTL & PDMA_DSCT_CTL_TXCNT_Msk) >> PDMA_DSCT_CTL_TXCNT_Pos);

    return (trigger_bytelen - (cur_txcnt + 1) * i32BitWidth);
}

static int HAL_DMA_FillTimeout(PDMA_T *pdma, int channelid, int timeout_us)
{
    if (!(dma_chn_mask & (1 << channelid)))
        goto exit_hal_dma_filltimeout;

    dma_chn_arr[channelid - NU_PDMA_CH_Pos].timeout_us = timeout_us;

    if (timeout_us && channelid <= 1)   // M480 limit
    {
        uint32_t u32ToClk_Max = 1000000 / (CLK_GetHCLKFreq() / (1 << 8));
        uint32_t u32Divider     = (timeout_us / u32ToClk_Max) / (1 << 16);
        uint32_t u32TOutCnt     = (timeout_us / u32ToClk_Max) % (1 << 16);

        PDMA_DisableTimeout(pdma,  1 << channelid);
        PDMA_EnableInt(pdma, channelid, PDMA_INT_TIMEOUT);    // Interrupt type

        if (u32Divider > 7)
        {
            u32Divider = 7;
            u32TOutCnt = (1 << 16);
        }

        pdma->TOUTPSC |= (u32Divider << (PDMA_TOUTPSC_TOUTPSC1_Pos * channelid));
        PDMA_SetTimeOut(pdma,  channelid, 1, u32TOutCnt);

        return 0;
    }
    else
    {
        PDMA_DisableInt(pdma, channelid, PDMA_INT_TIMEOUT);    // Interrupt type
        PDMA_DisableTimeout(pdma,  1 << channelid);
    }

exit_hal_dma_filltimeout:
    return -1;
}

int HAL_DMA_FillDescription(int channelid, uint32_t u32Peripheral, uint32_t data_width, uint32_t addr_src, uint32_t addr_dst, int32_t length, uint32_t timeout)
{
    PDMA_T *pdma;

    uint32_t u32SrcCtrl, u32DstCtrl;
    uint32_t u32PdmaIntMask = 0;

    if (!(dma_chn_mask & (1 << channelid)))
        goto exit_hal_dma_filldescription;

    if (!HAL_DMA_GetPeripheralControl(u32Peripheral, &u32SrcCtrl, &u32DstCtrl))
        goto exit_hal_dma_filldescription;

    pdma = HAL_DMA_GetModBase();

    HAL_DMA_DisableChannel(pdma, channelid);

    PDMA_DisableTimeout(pdma,  1 << channelid);

    PDMA_SetTransferMode(pdma,
                         channelid,
                         u32Peripheral,
                         0,
                         0);
    PDMA_SetTransferCnt(pdma,
                        channelid,
                        (data_width == 8) ? PDMA_WIDTH_8 : (data_width == 16) ? PDMA_WIDTH_16 : PDMA_WIDTH_32,
                        length);

    PDMA_SetTransferAddr(pdma,
                         channelid,
                         addr_src,
                         u32SrcCtrl,
                         addr_dst,
                         u32DstCtrl);

    PDMA_SetBurstType(pdma,
                      channelid,
                      PDMA_REQ_SINGLE,
                      0);

    PDMA_EnableInt(pdma,   channelid, PDMA_INT_TRANS_DONE);

    HAL_DMA_FillTimeout(pdma, channelid, timeout);

    HAL_DMA_EnableChannel(pdma, channelid);

    return HAL_OK;

exit_hal_dma_filldescription:

    return HAL_ERROR;
}

void PDMA_IRQHandler(void)
{
    PDMA_T *pdma   = HAL_DMA_GetModBase();
    uint32_t intsts = PDMA_GET_INT_STATUS(pdma);
    uint32_t abtsts = PDMA_GET_ABORT_STS(pdma);
    uint32_t tdsts  = PDMA_GET_TD_STS(pdma);
    uint32_t reqto  = intsts & (PDMA_INTSTS_REQTOF0_Msk | PDMA_INTSTS_REQTOF1_Msk);
    uint32_t reqto_ch = ((reqto & PDMA_INTSTS_REQTOF0_Msk) ? (1 << 0) : 0x0) | ((reqto & PDMA_INTSTS_REQTOF1_Msk) ? (1 << 1) : 0x0);
    int i;
    int allch_sts = (reqto_ch | tdsts | abtsts);

    // Abort
    if (intsts & PDMA_INTSTS_ABTIF_Msk)
    {
        // Clear all Abort flags
        PDMA_CLR_ABORT_FLAG(pdma, abtsts);
    }

    // Transfer done
    if (intsts & PDMA_INTSTS_TDIF_Msk)
    {
        // Clear all transfer done flags
        PDMA_CLR_TD_FLAG(pdma, tdsts);
    }

    // Timeout
    if (reqto)
    {
        // Clear all Timeout flags
        pdma->INTSTS = reqto;
    }

    while ((i = nu_ctz(allch_sts)) < NU_PDMA_CH_MAX)
    {
        int ch_mask     = (1 << i) ;

        if (allch_sts & ch_mask)
        {
            if (dma_chn_mask & ch_mask)
            {
                int ch_event = 0;
                struct nu_dma_chn_s *dma_chn = dma_chn_arr + i - NU_PDMA_CH_Pos;

                if (abtsts   & ch_mask) ch_event |= DMA_EVENT_ABORT;

                if (tdsts    & ch_mask) ch_event |= DMA_EVENT_TRANSFER_DONE;

                if (reqto_ch & ch_mask)
                {
                    PDMA_DisableTimeout(pdma,  ch_mask);
                    ch_event |= DMA_EVENT_TIMEOUT;
                }

                if (dma_chn->handler && (dma_chn->event & ch_event))
                    dma_chn->handler(dma_chn->id, ch_event);

                if (reqto_ch & ch_mask)
                    HAL_DMA_FillTimeout(pdma, i, dma_chn_arr[i - NU_PDMA_CH_Pos].timeout_us);
            }
        } //if ( allch_sts & ch_mask )

        // Clear the served bit.
        allch_sts &= ~ch_mask;

    } //for
}

