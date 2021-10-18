/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef HAL_DMA_H
#define HAL_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <objects.h>
#include <stdint.h>
#include <string.h>
#include <hal_status.h>
#include <hal_sys.h>

#define DMA_CAP_NONE    (0 << 0)

#define DMA_EVENT_ABORT             (1 << 0)
#define DMA_EVENT_TRANSFER_DONE     (1 << 1)
#define DMA_EVENT_TIMEOUT           (1 << 2)
#define DMA_EVENT_ALL               (DMA_EVENT_ABORT | DMA_EVENT_TRANSFER_DONE | DMA_EVENT_TIMEOUT)
#define DMA_EVENT_MASK              DMA_EVENT_ALL

int HAL_DMA_AllocateChannel(uint32_t capabilities);
int HAL_DMA_FreeChannel(int channelid);
int HAL_DMA_SetHandler(int channelid, uint32_t handler, uint32_t id, uint32_t event);
int HAL_DMA_FillDescription(int channelid, uint32_t u32Peripheral, uint32_t data_width, uint32_t addr_src, uint32_t addr_dst, int32_t length, uint32_t timeout);
int HAL_DMA_GetTransferredByteCount(int channelid, int trigger_bytelen);

#ifdef __cplusplus
}
#endif

#endif
