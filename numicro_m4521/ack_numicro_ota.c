/**************************************************************************//**
 * @brief    ACK HMCU OTA callback functions implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "ack_numicro_ota.h"
#include "numicro_hal.h"

void ACKNuMicroOta_FlashBegin(void)
{
    HAL_FLASH_Unlock();
}

void ACKNuMicroOta_FlashEnd(void)
{
    HAL_FLASH_Lock();
}

bool ACKNuMicroOta_FlashErase(uint32_t startPageOrdinal, uint32_t sizeBytes)
{
    // Erase the given range.
    return (HAL_OK == HAL_FLASH_Erase(FLASH_BASE + (FLASH_PAGE_SIZE * startPageOrdinal), sizeBytes));
}

bool ACKNuMicroOta_FlashWrite(uintptr_t address, const void *pData, uint32_t dataSize16BitValues)
{
    uint32_t u32DataSize = 2 * dataSize16BitValues;
    // no data to program is an error condition.
    if (((uintptr_t)pData & 1) || (0 == dataSize16BitValues))
        return false;

    return (u32DataSize == HAL_FLASH_Write(address, (uint32_t *)pData, u32DataSize));
}
