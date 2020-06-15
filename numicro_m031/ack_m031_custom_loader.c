/**************************************************************************//**
 * @file     ack_m031_custom_loader.c
 * @brief    ACK HMCU bootloader implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "main.h"
#include "ack_m031_ota.h"
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

#if 0
static int numicro_userconf_check_rewrite(void)
{
    uint32_t au32Config[2];
    uint32_t    i, u32Data;            /* variables */
    int ret = 0;

    SYS_UnlockReg();

    FMC_Open();

    if (FMC_GetBootSource() == 0)
        printf("[APROM]\n");           /* debug message */
    else
        printf("[LDROM]\n");           /* debug message */

    u32Data = FMC_ReadCID();           /* Get company ID, should be 0xDA. */
    printf("  Company ID ............................ [0x%08x]\n", u32Data);   /* information message */

    u32Data = FMC_ReadPID();           /* Get product ID. */
    printf("  Product ID ............................ [0x%08x]\n", u32Data);   /* information message */

    for (i = 0; i < 3; i++)            /* Get 96-bits UID. */
    {
        u32Data = FMC_ReadUID(i);
        printf("  Unique ID %d ........................... [0x%08x]\n", i, u32Data);  /* information message */
    }

    for (i = 0; i < 4; i++)            /* Get 4 words UCID. */
    {
        u32Data = FMC_ReadUCID(i);
        printf("  Unique Customer ID %d .................. [0x%08x]\n", i, u32Data);  /* information message */
    }

    if (FMC_ReadConfig(au32Config, 2) < 0)
        goto exit_numicro_userconf_check_rewrite;


#define DEF_CONFIG0_CBS_APROM_IAP   0x2
#define DEF_CONFIG0_CBS_Pos             (6)
#define DEF_CONFIG0_CBS_Msk             (0x3ul << DEF_CONFIG0_CBS_Pos)

    printf("FMC User config: 0:%08x, 1:%08x, CBS:%d\n", au32Config[0], au32Config[1], ((au32Config[0]&DEF_CONFIG0_CBS_Msk) >> DEF_CONFIG0_CBS_Pos));
    if ((au32Config[0]&DEF_CONFIG0_CBS_Msk) != (DEF_CONFIG0_CBS_APROM_IAP << DEF_CONFIG0_CBS_Pos)) //Boot from AP-IAP mode?
    {
        FMC_ENABLE_CFG_UPDATE();

        au32Config[0] &= ~DEF_CONFIG0_CBS_Msk ;
        au32Config[0] |= (DEF_CONFIG0_CBS_APROM_IAP << DEF_CONFIG0_CBS_Pos) ;
        printf("Will write FMC User config: 0:%08x, 1:%08x\n", au32Config[0], au32Config[1]);

        if (FMC_WriteConfig(au32Config, 2) < 0)
        {
            printf("Error: FMC_WriteConfig!\n");
            ret = -1;
            goto exit_numicro_userconf_check_rewrite;
        }

        memset(au32Config, 0xff, sizeof(au32Config));

        FMC_ReadConfig(au32Config, 2);
        printf("Read back FMC User config: 0:%08x, 1:%08x\n", au32Config[0], au32Config[1]);

        if ((au32Config[0]&DEF_CONFIG0_CBS_Msk) != (DEF_CONFIG0_CBS_APROM_IAP << DEF_CONFIG0_CBS_Pos))
        {
            printf("Error: Program Config Failed - %08x != %08x\n", (au32Config[0]&DEF_CONFIG0_CBS_Msk), (DEF_CONFIG0_CBS_APROM_IAP << DEF_CONFIG0_CBS_Pos));
            ret = -1;
            goto exit_numicro_userconf_check_rewrite;
        }

        printf("do chip reset\n");
        /* Reset Chip to reload new CONFIG value */
        SYS->IPRST0 = SYS_IPRST0_CHIPRST_Msk;
    }

exit_numicro_userconf_check_rewrite:

    /* Disable FMC ISP function */
    FMC_Close();
    /* Lock protected registers */
    SYS_LockReg();

    return ret;
}
#endif

void Loader(void)
{
    /* Avoidence */
    //if ( numicro_userconf_check_rewrite () < 0 )
    //  return;

    // Check status partition to see whether there's an unapplied OTA image.
    if (CheckStatusPartitionForUnappliedImage())
    {
        //printf("Found an OTA image.\n");
        printf("Will update.\n");
        if (ApplyOtaImage())
        {
            printf("Updated image to PRIMARY partition from STAGING.\n");
            EraseStatusPartitionToIndicateNoUnappliedImage();
            //printf("Erased OTA status page.\n");
        }
        else
        {
            //printf("Fail to apply OTA image.\n");
            return; //Some troubles.
        }
    }

    //printf("Normal booting\n");

    // Now that we've applied an OTA image (or not) above, prepare to execute the application
    // on the Primary Partition.
    UninitializeHardware();

    // Does not return.
    JumpToApplication();
}

// Determines whether there's an unapplied OTA image in the Staging Partition.
bool CheckStatusPartitionForUnappliedImage(void)
{
    bool bCRCVerifiedResult = false;
    volatile ACKNuMicroOtaStatusPartition_t *pStatusPartition
        = (ACKNuMicroOtaStatusPartition_t *)ACK_NUMICRO_OTA_STATUS_PARTITION_START;

#if 0
    printf("ACK_NUMICRO_OTA_LOADER_PARTITION_START: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_LOADER_PARTITION_START);
    printf("ACK_NUMICRO_OTA_STATUS_PARTITION_START: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_STATUS_PARTITION_START);
    printf("ACK_NUMICRO_OTA_PRIMARY_PARTITION_START: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_PRIMARY_PARTITION_START);
    printf("ACK_NUMICRO_OTA_STAGING_PARTITION_START: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_STAGING_PARTITION_START);


    printf("pStatusPartition->Signature1: 0x%08x.\n", (unsigned int)pStatusPartition->Signature1);
    printf("ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE1: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE1);

    printf("pStatusPartition->Signature2: 0x%08x.\n", (unsigned int)pStatusPartition->Signature2);
    printf("ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE2: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE2);

    printf("pStatusPartition->Pad[0]: 0x%08x.\n", (unsigned int)pStatusPartition->Pad[0]);
    printf("pStatusPartition->Pad[1]: 0x%08x.\n", (unsigned int)pStatusPartition->Pad[1]);
    printf("pStatusPartition->Pad[2]: 0x%08x.\n", (unsigned int)pStatusPartition->Pad[2]);

    printf("pStatusPartition->State: 0x%08x.\n", (unsigned int)pStatusPartition->State);
    printf("ACK_NUMICRO_OTA_STATUS_PARTITION_STATE_UNAPPLIED_IMAGE: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_STATUS_PARTITION_STATE_UNAPPLIED_IMAGE);

    printf("pStatusPartition->ImageSize: 0x%08x.\n", (unsigned int)pStatusPartition->ImageSize);
    printf("ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE);
    printf("ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE);

    printf("pStatusPartition->ImageStartAddress: 0x%08x.\n", (unsigned int)pStatusPartition->ImageStartAddress);
    printf("ACK_NUMICRO_OTA_PRIMARY_PARTITION_START: 0x%08x.\n", (unsigned int)ACK_NUMICRO_OTA_PRIMARY_PARTITION_START);
#endif

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
    //printf("pStatusPartition->Crc32: 0x%08x.\n", pStatusPartition->Crc32 );


    uint32_t u32Ret = HAL_ERROR;
    HAL_CRC_Open(&g_sCrcDev);
    u32Ret = HAL_CRC_Calculate(&g_sCrcDev, (uint8_t *)ACK_NUMICRO_OTA_STAGING_PARTITION_START, pStatusPartition->ImageSize);
    HAL_CRC_Close(&g_sCrcDev);
    bCRCVerifiedResult = (pStatusPartition->Crc32 ==  u32Ret);
    //printf("CRC verified result: %d. (%08x, %08x)\n",  bCRCVerifiedResult, pStatusPartition->Crc32, u32Ret );

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

#if defined(__ICCARM__) || defined(__GNUC__)
    //printf("VECMAP = 0x%x\n", FMC_GetVECMAP());
#else
#ifndef BootLD
    printf("Current RO Base = 0x%x, VECMAP = 0x%x\n", (uint32_t)&Image$$RO$$Base, FMC_GetVECMAP());
#else
    printf("VECMAP = 0x%x\n", FMC_GetVECMAP());
#endif
#endif
    while ((UART0->FIFOSTS & UART_FIFOSTS_TXEMPTY_Msk) == 0);

    FMC_SetVectorPageAddr(ACK_NUMICRO_OTA_PRIMARY_PARTITION_START);
    SYS_LockReg();

    NVIC_SystemReset();

    // Never get here.
}


void HAL_SYS_TICK_InvokeCallback(void)
{
}
