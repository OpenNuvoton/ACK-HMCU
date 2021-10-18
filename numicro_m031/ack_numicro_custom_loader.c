/**************************************************************************//**
 * @file     ack_numicro_custom_loader.c
 * @brief    ACK HMCU bootloader implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "main.h"
#include "ack_numicro_ota.h"
#include <stdbool.h>
#include <stdint.h>
#include "numicro_hal.h"
#include <string.h>

// Forward references.
static bool CheckStatusPartitionForUnappliedImage(void);
static bool ApplyOtaImage(void);
static bool EraseStatusPartitionToIndicateNoUnappliedImage(void);
static void UninitializeHardware(void);
static void JumpToApplication(void);

void Loader(void)
{
    // Check status partition to see whether there's an unapplied OTA image.
    if (CheckStatusPartitionForUnappliedImage())
    {
        //printf("Will update.\n");
        if (ApplyOtaImage())
        {
            printf("Updated to PRIMARY from STAGING.\n");
            EraseStatusPartitionToIndicateNoUnappliedImage();
        }
        else
        {
            return; //Some troubles.
        }
    }

    // Now that we've applied an OTA image (or not) above, prepare to execute the application
    // on the Primary Partition.
    UninitializeHardware();

    // Does not return.
    JumpToApplication();
}

// Determines whether there's an unapplied OTA image in the Staging Partition.
bool CheckStatusPartitionForUnappliedImage(void)
{
    uint32_t u32Ret = HAL_ERROR;
    bool bCRCVerifiedResult = false;
    volatile ACKNuMicroOtaStatusPartition_t *pStatusPartition
        = (ACKNuMicroOtaStatusPartition_t *)ACK_NUMICRO_OTA_STATUS_PARTITION_START;

    if ((pStatusPartition->Signature1 != ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE1)
            || (pStatusPartition->Signature2 != ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE2)
            || pStatusPartition->Pad[0]
            || pStatusPartition->Pad[1]
            || pStatusPartition->Pad[2]
            || (pStatusPartition->State != ACK_NUMICRO_OTA_STATUS_PARTITION_STATE_UNAPPLIED_IMAGE)
            || (pStatusPartition->ImageSize == 0)
            || (pStatusPartition->ImageSize > ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE)
            || (pStatusPartition->ImageSize > ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE)
            || (pStatusPartition->ImageStartAddress != ACK_NUMICRO_OTA_PRIMARY_PARTITION_START))
    {
        // Signature or pad bytes not valid, or they are valid and the state doesn't indicate
        // the presence of an unapplied OTA image. Or the image size is 0 or too large, or the
        // image is intended for a different address than where the Primary Partition is.
        return false;
    }

    HAL_CRC_Open(&g_sCrcDev);
    u32Ret = HAL_CRC_Calculate(&g_sCrcDev, (uint8_t *)ACK_NUMICRO_OTA_STAGING_PARTITION_START, pStatusPartition->ImageSize);
    HAL_CRC_Close(&g_sCrcDev);
    bCRCVerifiedResult = (pStatusPartition->Crc32 ==  u32Ret);

    return bCRCVerifiedResult;
}

// Programs flash representing the Primary Partition with the contents of the Staging Partition.
bool ApplyOtaImage(void)
{
    volatile ACKNuMicroOtaStatusPartition_t *pStatusPartition
        = (ACKNuMicroOtaStatusPartition_t *)ACK_NUMICRO_OTA_STATUS_PARTITION_START;
    bool result = false;

    uint32_t _16BitWordCount = pStatusPartition->ImageSize / 2;
    if (pStatusPartition->ImageSize & 1)
    {
        ++_16BitWordCount;
    }

    ACKNuMicroOta_FlashBegin();

#if (ACK_NUMICRO_OTA_PRIMARY_PARTITION_START % ACK_NUMICRO_OTA_FLASH_PAGE_SIZE)
#error Staging Partition must be an even multiple of the flash page size.
#endif
    if (ACKNuMicroOta_FlashErase(
                ACK_NUMICRO_OTA_PRIMARY_PARTITION_START / ACK_NUMICRO_OTA_FLASH_PAGE_SIZE,
                _16BitWordCount * 2))
    {
        result = ACKNuMicroOta_FlashWrite(
                     ACK_NUMICRO_OTA_PRIMARY_PARTITION_START,
                     (const void *)ACK_NUMICRO_OTA_STAGING_PARTITION_START,
                     _16BitWordCount);
    }

    ACKNuMicroOta_FlashEnd();

    return result;
}

bool EraseStatusPartitionToIndicateNoUnappliedImage(void)
{
    bool result;

    ACKNuMicroOta_FlashBegin();

#if (ACK_NUMICRO_OTA_STAGING_PARTITION_START % ACK_NUMICRO_OTA_FLASH_PAGE_SIZE)
#error Staging Partition must be an even multiple of the flash page size.
#endif
    result = ACKNuMicroOta_FlashErase(
                 ACK_NUMICRO_OTA_STATUS_PARTITION_START / ACK_NUMICRO_OTA_FLASH_PAGE_SIZE,
                 ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE);

    ACKNuMicroOta_FlashEnd();

    return result;
}

void UninitializeHardware(void)
{

}

void JumpToApplication(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    FMC_Open();

    //printf("VECMAP = 0x%x -> 0x%x\r\n", FMC_GetVECMAP(), ACK_NUMICRO_OTA_PRIMARY_PARTITION_START);

    while ((UART0->FIFOSTS & UART_FIFOSTS_TXEMPTY_Msk) == 0);

    FMC_SetVectorPageAddr(ACK_NUMICRO_OTA_PRIMARY_PARTITION_START);
    SYS_LockReg();

    SYS_ResetCPU();
    NVIC_SystemReset();

    // Never get here.
}


void HAL_SYS_TICK_InvokeCallback(void)
{

}
