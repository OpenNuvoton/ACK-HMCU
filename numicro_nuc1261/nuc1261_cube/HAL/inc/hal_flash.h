/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef HAL_FLASH_H
#define HAL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <objects.h>
#include <stdint.h>
#include <hal_status.h>


#define FLASH_PAGE_SIZE     FMC_FLASH_PAGE_SIZE

#undef FLASH_BASE
#define FLASH_BASE          FMC_APROM_BASE

#ifndef FLASH_SIZE
// For backward-compatible
#define FLASH_SIZE          (128*1024)
#endif

#define FLASH_END           (FLASH_BASE+FLASH_SIZE)

#define FLASHPARTDEV_READABLE_Pos       (0)
#define FLASHPARTDEV_READABLE_Msk       (0x1u << FLASHPARTDEV_READABLE_Pos)

#define FLASHPARTDEV_WRITABLE_Pos       (1)
#define FLASHPARTDEV_WRITABLE_Msk       (0x1u << FLASHPARTDEV_WRITABLE_Pos)

typedef struct
{
    const char     *partition_description;
    uint32_t        partition_start_addr;
    uint32_t        partition_length;
    uint32_t        partition_options;
} S_FlashPartitionDev;

void HAL_FLASH_Lock(void);
void HAL_FLASH_Unlock(void);
HAL_Status HAL_FLASH_CheckCRCValue(uint32_t u32SrcData, uint32_t u32LenBytes, uint32_t u32CRCValue);
HAL_Status HAL_FLASH_Erase(uint32_t u32SrcAddr, uint32_t u32LengthInBytes);
int HAL_FLASH_Read(const void *pvDstBuf, uint32_t *pu32SrcAddr, uint32_t u32InBufferLen);
int HAL_FLASH_Write(uint32_t u32StartAddr, uint32_t *pu32Data, uint32_t u32LenBytes);
int HAL_FLASH_Update(uint32_t *pu32DstAddr, uint32_t *pSrcData, uint32_t u32LenBytes);

#ifdef __cplusplus
}
#endif

#endif
