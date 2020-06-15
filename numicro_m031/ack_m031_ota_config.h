/*
 * Copyright (c) 2018-Present Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * AMAZON PROPRIETARY/CONFIDENTIAL
 *
 * This file is being provided to you and is licensed as "Software" under the Master Equipment Agreement
 * signed by you and Amazon.com, Inc. or its affiliates (the "Agreement"). See the Agreement for the
 * specific terms and conditions of the Agreement. If you did not receive this file directly from
 * Amazon.com. Inc. or its affiliates subject to an Agreement that governs its use, you have no license
 * to use this file.
 *
 * This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 */

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

// For M032SEAAE: 128KB flash and 16K RAM, Flash page size: 512B.
#define ACK_NUMICRO_OTA_FLASH_BASE         FLASH_BASE
#define ACK_NUMICRO_OTA_FLASH_SIZE         FLASH_SIZE
#define ACK_NUMICRO_OTA_FLASH_PAGE_SIZE        FLASH_PAGE_SIZE
#define ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE      (11*ACK_NUMICRO_OTA_FLASH_PAGE_SIZE)    // 5.5KB

#ifdef __cplusplus
}
#endif

#endif // ndef __ACK_NUMICRO_OTA_CONFIG_H__
