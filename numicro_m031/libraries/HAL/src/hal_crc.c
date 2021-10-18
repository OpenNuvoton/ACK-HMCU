/**************************************************************************//**
 * @file     hal_crc.c
 * @brief    CRC driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <hal_crc.h>

#define CRC_SET_WDATA_LEN(u32WDataLen) (CRC->CTL = (CRC->CTL & ~CRC_CTL_DATLEN_Msk) | u32WDataLen)

static const struct nu_modinit_s crc_modinit =
{
    CRC_0, CRC_MODULE, 0, 0, CRC_RST, 0, NULL
};

static volatile int crc_inited = 0;
static volatile uint32_t crc_ref_cnt = 0;
int32_t HAL_CRC_Initialize(S_CRCDev *psCrcDev)
{
    if (crc_inited || !psCrcDev)
        goto exit_hal_crc_initialize;

    /* Unlock register lock protect */
    SYS_UnlockReg();

    // Enable IP clock
    CLK_EnableModuleClock(crc_modinit.clkidx);

    // Reset this module
    SYS_ResetModule(crc_modinit.rsetidx);

    crc_inited = 1;

    return HAL_OK;

exit_hal_crc_initialize:

    return HAL_ERROR;
}

int32_t HAL_CRC_Open(S_CRCDev *psCrcDev)
{
    HAL_CRC_Initialize(psCrcDev);

    if (!crc_inited || crc_ref_cnt || !psCrcDev)
        goto exit_hal_crc_open;

    switch (psCrcDev->eCRCMode)
    {
    case eCRCMode_CCITT:
    case eCRCMode_CRC8:
    case eCRCMode_CRC16:
    case eCRCMode_CRC32:
        break;
    default:
        goto exit_hal_crc_open;
    }

    switch (psCrcDev->eCRCCpuWLen)
    {
    case eCRCCpuWLen_8:
    case eCRCCpuWLen_16:
    case eCRCCpuWLen_32:
        break;
    default:
        goto exit_hal_crc_open;
    }

    CRC_Open(psCrcDev->eCRCMode, psCrcDev->eCRCAttr, psCrcDev->u32SeedValue, psCrcDev->eCRCCpuWLen);

    crc_ref_cnt++;

    return HAL_OK;

exit_hal_crc_open:

    return HAL_ERROR;
}

static uint32_t CRC_WLen8(uint8_t *pu8Buf, uint32_t u32BufLen)
{
    int i;

    CRC_SET_WDATA_LEN(eCRCCpuWLen_8);
    for (i = 0; i < u32BufLen; i++)
        CRC_WRITE_DATA(pu8Buf[i]);

    return CRC_GetChecksum();
}

static uint32_t CRC_WLen16(uint16_t *pu16Buf, uint32_t u32BufLen)
{
    int i;

    CRC_SET_WDATA_LEN(eCRCCpuWLen_16);
    for (i = 0; i < u32BufLen / 2; i++)
        CRC_WRITE_DATA(pu16Buf[i]);

    if ((u32BufLen % 2U) != 0U)
        return CRC_WLen8((uint8_t *)&pu16Buf[i], u32BufLen % 2U);

    return CRC_GetChecksum();
}

static uint32_t CRC_WLen32(uint32_t *pu32Buf, uint32_t u32BufLen)
{
    int i;

    CRC_SET_WDATA_LEN(eCRCCpuWLen_32);
    for (i = 0; i < u32BufLen / 4; i++)
        CRC_WRITE_DATA(pu32Buf[i]);

    if ((u32BufLen % 4U) != 0U)
        return CRC_WLen8((uint8_t *)&pu32Buf[i], u32BufLen % 4U);

    return CRC_GetChecksum();
}


uint32_t HAL_CRC_Calculate(S_CRCDev *psCrcDev, uint8_t *pu8Buf, uint32_t u32BufLen)
{
    uint32_t u32ChkSumValue = 0;
    uint32_t fl_addr = 0;
    int fl_offset, len, remaining;
    uint8_t *puCur8Buf = pu8Buf;

    if (!crc_ref_cnt || !psCrcDev || !pu8Buf || !u32BufLen)
        goto exit_hal_crc_calcualte;

    remaining = u32BufLen;
    do
    {
        switch (psCrcDev->eCRCCpuWLen)
        {
        case eCRCCpuWLen_32:
            fl_addr = ROUND_UP((uint32_t)puCur8Buf, 4);
            fl_offset = fl_addr - (uint32_t)puCur8Buf;
            len = (fl_offset > 0) ? fl_offset : remaining;
            if (ALIGNED((uint32_t)puCur8Buf, 4U))
            {
                CRC_WLen32((uint32_t *) puCur8Buf, len);
                break;
            }

        case eCRCCpuWLen_16:
            fl_addr = ROUND_UP((uint32_t)puCur8Buf, 2);
            fl_offset = fl_addr - (uint32_t)puCur8Buf;
            len = (fl_offset > 0) ? fl_offset : remaining;
            if (ALIGNED((uint32_t)puCur8Buf, 2U))
            {
                CRC_WLen16((uint16_t *) puCur8Buf, len);
                break;
            }

        case eCRCCpuWLen_8:
            len = remaining;
            CRC_WLen8(puCur8Buf, len);
            break;

        default:
            goto exit_hal_crc_calcualte;
        } //switch

        remaining -= len;
        puCur8Buf += len;

    }
    while (remaining > 0);

    return CRC_GetChecksum();

exit_hal_crc_calcualte:

    return HAL_ERROR;
}

int32_t HAL_CRC_Close(S_CRCDev *psCrcDev)
{
    if (!crc_ref_cnt)
        goto exit_hal_crc_close;

    // Reset this module
    SYS_ResetModule(crc_modinit.rsetidx);

    crc_ref_cnt--;

    return HAL_OK;

exit_hal_crc_close:

    return HAL_ERROR;
}

int32_t HAL_CRC_Finalize(S_CRCDev *psCrcDev)
{
    if (!crc_inited)
        goto exit_hal_crc_finalize;

    /* Unlock register lock protect */
    SYS_UnlockReg();

    // Disable IP clock
    CLK_DisableModuleClock(crc_modinit.clkidx);

    crc_inited = 0;

    return HAL_OK;

exit_hal_crc_finalize:

    return HAL_ERROR;
}
