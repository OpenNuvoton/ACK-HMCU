/**************************************************************************//**
 * @file     hal_flash.c
 * @brief    FMC driver implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <hal_flash.h>
#include <string.h>

void HAL_FLASH_Lock(void)
{
    SYS_UnlockReg();            /* Unlock register lock protect */
    FMC_DISABLE_AP_UPDATE();    /* Disable FMC ISP function */
    FMC_Close();                /* Disable APROM erase/program */
    SYS_LockReg();              /* Unlock register lock protect */
}

void HAL_FLASH_Unlock(void)
{
    SYS_UnlockReg();            /* Unlock register lock protect */
    FMC_Open();                 /* Enable FMC ISP function */
    FMC_ENABLE_AP_UPDATE();     /* Enable APROM erase/program */
}

HAL_Status HAL_FLASH_Erase(uint32_t u32SrcAddr, uint32_t u32LengthInBytes)
{
    int i = 0;
    int page_num;

    if ((u32SrcAddr % FLASH_PAGE_SIZE) != 0) //must be a 4k-aligned address
        goto exit_hal_flash_erase;

    if ((u32SrcAddr + u32LengthInBytes) > FLASH_END)
        goto exit_hal_flash_erase;

    page_num = ((u32LengthInBytes + FLASH_PAGE_SIZE) - 1) / FLASH_PAGE_SIZE;

    for (i = 0; i < page_num; i++)
    {
        if (FMC_Erase(u32SrcAddr + i * FLASH_PAGE_SIZE) < 0)
            break;
    }

    if (i != page_num)
        goto exit_hal_flash_erase;

    return HAL_OK;

exit_hal_flash_erase:

    return HAL_ERROR;
}

int HAL_FLASH_Read(const void *pvDstBuf, uint32_t *pu32SrcAddr, uint32_t u32InBufferLen)
{
    if (!pu32SrcAddr || !pvDstBuf || !u32InBufferLen)
        goto exit_hal_flash_read;

    memcpy((void *)pvDstBuf, pu32SrcAddr, u32InBufferLen);

    return u32InBufferLen;

exit_hal_flash_read:

    return HAL_ERROR;
}

typedef enum
{
    eUpdateLen_Word = sizeof(uint32_t),
    eUpdateLen_2Word = 2 * eUpdateLen_Word,
    eUpdateLen_Page = FLASH_PAGE_SIZE
} E_UPDATE_LEN;

// u32StartAddr must be a aligned-word address
int HAL_FLASH_Write(uint32_t u32StartAddr, uint32_t *pu32Data, uint32_t u32LenBytes)
{
    int ret = 0;
    uint32_t *pu32SrcAddr;
    uint32_t u32DstAddr;

    int remaining, len;

    if ((u32StartAddr % sizeof(uint32_t)) > 0)
        goto exit_hal_flash_write;

    // Write word or 2word to flash.
    remaining = u32LenBytes;
    pu32SrcAddr = pu32Data;
    u32DstAddr = u32StartAddr;

    do
    {
#if (FLASH_PAGE_SIZE == 2048u)
        len = (remaining >= eUpdateLen_2Word) ? eUpdateLen_2Word : eUpdateLen_Word;
#else
        len = eUpdateLen_Word;
#endif

        if (len == eUpdateLen_2Word)
            FMC_Write8Bytes(u32DstAddr, pu32SrcAddr[0], pu32SrcAddr[1]);
        else
            FMC_Write(u32DstAddr, pu32SrcAddr[0]);

        remaining -= len;
        pu32SrcAddr += (len / eUpdateLen_Word);
        u32DstAddr += len;

    }
    while (remaining > 0);

    ret = u32LenBytes;

exit_hal_flash_write:

    return ret;
}

static int IsCheckAllFF(uint8_t *pu8Buf, int i32Len)
{
    int i = 0;
    while (i < i32Len)
    {
        if (pu8Buf[i] != 0xffu)
            return 0;
        i++;
    }
    return 1;
}

static uint8_t g_au8PageCache[FLASH_PAGE_SIZE];

static int FLASH_Update(uint32_t u32StartAddr, uint32_t *pu32Data, uint32_t u32LenBytes)
{
    int ret = 0;
    int remaining = u32LenBytes;
    uint8_t *src_addr = (uint8_t *) pu32Data;
    uint32_t fl_addr = 0;
    int fl_offset = 0;
    int len = 0;
    int i32UpdateLen;
    uint32_t *pu32PageCache = (uint32_t *)&g_au8PageCache[0];
    int i32DoErase = 0;

    do
    {
        // Select optimize writing
        if (remaining / eUpdateLen_Page)
            i32UpdateLen = eUpdateLen_Page;
        else if (remaining / eUpdateLen_2Word)
            i32UpdateLen = eUpdateLen_2Word;
        else
            i32UpdateLen = eUpdateLen_Word;

        fl_addr = ROUND_DOWN(u32StartAddr, i32UpdateLen);
        fl_offset = u32StartAddr - fl_addr;
        len = MIN(i32UpdateLen - fl_offset, remaining);

        i32DoErase = 0;
        /* Wrote buffer is all-0xff? if not, erase page and update page. */
        if (!IsCheckAllFF((uint8_t *)(u32StartAddr + fl_offset), len))
        {
            i32UpdateLen = eUpdateLen_Page;
            fl_addr = ROUND_DOWN(u32StartAddr, i32UpdateLen);
            fl_offset = u32StartAddr - fl_addr;
            len = MIN(i32UpdateLen - fl_offset, remaining);
            i32DoErase = 1;
        }

        /* Not-align and remaining is less than eUpdateLen bytes. */
        if ((fl_offset != 0) || (len < i32UpdateLen))
        {
            /* Load from the flash into the cache if necessary */
            memcpy((void *)pu32PageCache, (void *) fl_addr, i32UpdateLen);
        }
        /* Update the cache from the source */
        memcpy((void *)((uint32_t)pu32PageCache + fl_offset), src_addr, len);

        if (i32DoErase)
        {
            /* Erase the page */
            if ((ret = HAL_FLASH_Erase(fl_addr, FLASH_PAGE_SIZE)) != HAL_OK)
            {
                goto exit_flash_update;
            }
        }

        if ((ret = HAL_FLASH_Write(fl_addr, pu32PageCache, i32UpdateLen)) < 0)
        {
            goto exit_flash_update;
        }

        u32StartAddr += len;
        src_addr += len;
        remaining -= len;

    }
    while (remaining > 0);

    return u32LenBytes;

exit_flash_update:

    return 0;
}

HAL_Status HAL_FLASH_CheckCRCValue(uint32_t u32SrcData, uint32_t u32LenBytes, uint32_t u32CRCValue)
{
    uint32_t u32FmcChecksum = FMC_GetChkSum(u32SrcData, u32LenBytes);

    if (u32FmcChecksum != u32CRCValue)
        return HAL_ERROR;

    return HAL_OK;
}

int HAL_FLASH_Update(uint32_t *pu32DstAddr, uint32_t *pSrcData, uint32_t u32LenBytes)
{
    int ret;

    if (!pSrcData || !u32LenBytes)
        goto exit_hal_flash_update;

    ret = FLASH_Update((uint32_t)pu32DstAddr, pSrcData, u32LenBytes);

    return ret;

exit_hal_flash_update:

    return HAL_ERROR;
}
