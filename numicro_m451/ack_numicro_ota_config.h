/**************************************************************************//**
 * @brief    ACK HMCU platform porting.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __ACK_NUMICRO_OTA_CONFIG_H__
#define __ACK_NUMICRO_OTA_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "numicro_hal.h"

// The Loader and Status Partition sizes must each be a multiple of FLASH_PAGE_SIZE. (The size of the
// Loader Partition is always set to the flash page size. See ack_numicro_ota.h.)
// The sum of the Loader and Status Partition sizes must be an even multiple of FLASH_PAGE_SIZE
// so that the remainder of flash can be equally divided among the Staging and Primary Partitions.

#define ACK_NUMICRO_OTA_FLASH_BASE         FLASH_BASE
#define ACK_NUMICRO_OTA_FLASH_SIZE         FLASH_SIZE
#define ACK_NUMICRO_OTA_FLASH_PAGE_SIZE    FLASH_PAGE_SIZE

#ifndef PAGE_SIZE_2048
#define ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE  (11*ACK_NUMICRO_OTA_FLASH_PAGE_SIZE)    // 5.5KB
#else
#define ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE  (3*ACK_NUMICRO_OTA_FLASH_PAGE_SIZE)    // 6 KB
#endif

#ifdef __cplusplus
}
#endif

#endif // ndef __ACK_NUMICRO_OTA_CONFIG_H__
