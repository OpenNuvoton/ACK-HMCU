/**************************************************************************//**
 * @brief    ACK HMCU platform porting.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __ACK_NUMICRO_OTA_SHARED_H__
#define __ACK_NUMICRO_OTA_SHARED_H__

#include "ack_numicro_ota_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "numicro_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE ACK_NUMICRO_OTA_FLASH_PAGE_SIZE

// Partition start and end addresses.
#define ACK_NUMICRO_OTA_LOADER_PARTITION_START ACK_NUMICRO_OTA_FLASH_BASE
#define ACK_NUMICRO_OTA_LOADER_PARTITION_END ((ACK_NUMICRO_OTA_LOADER_PARTITION_START + \
    ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE) - 1)

#define ACK_NUMICRO_OTA_STATUS_PARTITION_START (ACK_NUMICRO_OTA_LOADER_PARTITION_END + 1)
#define ACK_NUMICRO_OTA_STATUS_PARTITION_END ((ACK_NUMICRO_OTA_STATUS_PARTITION_START + \
    ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE) - 1)

#define ACK_NUMICRO_OTA_PRIMARY_PARTITION_START (ACK_NUMICRO_OTA_STATUS_PARTITION_END + 1)
#define ACK_NUMICRO_OTA_PRIMARY_PARTITION_END ((ACK_NUMICRO_OTA_PRIMARY_PARTITION_START + \
    ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE) - 1)

#define ACK_NUMICRO_OTA_STAGING_PARTITION_START (ACK_NUMICRO_OTA_PRIMARY_PARTITION_END + 1)
#define ACK_NUMICRO_OTA_STAGING_PARTITION_END ((ACK_NUMICRO_OTA_STAGING_PARTITION_START + \
    ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE) - 1)

// Flash partition sizes.
#define ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE ((ACK_NUMICRO_OTA_FLASH_SIZE - \
    (ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE + ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE)) / 2)
#define ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE

// Sanity checks on partition sizes.
#if (ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE % ACK_NUMICRO_OTA_FLASH_PAGE_SIZE) \
    || (ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE % ACK_NUMICRO_OTA_FLASH_PAGE_SIZE) \
    || (ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE % ACK_NUMICRO_OTA_FLASH_PAGE_SIZE) \
    || (ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE % ACK_NUMICRO_OTA_FLASH_PAGE_SIZE) \
    || (ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE != ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE) \
    || ((ACK_NUMICRO_OTA_LOADER_PARTITION_SIZE + ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE \
        + ACK_NUMICRO_OTA_PRIMARY_PARTITION_SIZE + ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE) \
            != ACK_NUMICRO_OTA_FLASH_SIZE)
#error \
Partition sizes must be a multiple of the page size;
\
Primary and Staging Partitions must be the same size,;
\
and the sum of the sizes of the Loader, Status, Primary, \
and Staging Partitions must equal the total flash size.
#endif

typedef struct _ACKNuMicroOtaStatusPartition_t
{
    // Must be ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE1.
    uint32_t Signature1;

    // Note that this is at the end of the structure so that it's the very final thing flashed
    // to make the Status Partition valid.
    // ACK_NUMICRO_OTA_STATUS_PARTITION_STATE_UNAPPLIED_IMAGE: Unapplied image present in Staging Partition.
    // Anything else: No unapplied OTA image present in Staging Partition.
    uint8_t State;

    // Reserved. Must be 0.
    uint8_t Pad[3];

    // Address in the Primary partition where the program starts. Must match the base address of
    // the Primary Partition.
    uint32_t ImageStartAddress;

    // Size in bytes of the OTA image on the Staging Partition.
    uint32_t ImageSize;

    // CRC32 of image data, for image validation.
    uint32_t Crc32;

    // Must be ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE2, or else State is ignored and considered 0.
    // This is deliberately placed at the end of the partition to avoid seeing the Status Partition
    // as valid unless the *entire* contents of this struct can be successfully flashed to it.
    uint32_t Signature2;
}
ACKNuMicroOtaStatusPartition_t;

// Value for ACK_NuMicroOtaStatusPartition_t Signature1 and Signature2.
#define ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE1 0x314b4341
#define ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE2 0x324b4341

// Value for ACK_HostFirmwareUpdateStatusPartition_t State.
#define ACK_NUMICRO_OTA_STATUS_PARTITION_STATE_UNAPPLIED_IMAGE 0xa5

// Routines shared with custom loader program.
void ACKNuMicroOta_FlashBegin(void);
void ACKNuMicroOta_FlashEnd(void);
bool ACKNuMicroOta_FlashErase(uint32_t startPageOrdinal, uint32_t sizeBytes);
bool ACKNuMicroOta_FlashWrite(uintptr_t address, const void *pData, uint32_t dataSize16BitValues);

#ifdef __cplusplus
}
#endif

#endif // ndef __ACK_NUMICRO_OTA_SHARED_H__
