/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef HAL_CRC_H
#define HAL_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "objects.h"
#include "hal_status.h"

typedef enum
{
    eCRCMode_CCITT    = (int)CRC_CCITT,
    eCRCMode_CRC8     = (int)CRC_8,
    eCRCMode_CRC16    = (int)CRC_16,
    eCRCMode_CRC32    = (int)CRC_32
} E_CRC_MODE;

typedef enum
{
    eCRCAttr_ChecksumCom = CRC_CHECKSUM_COM,
    eCRCAttr_ChecksumRVS = CRC_CHECKSUM_RVS,
    eCRCAttr_WDataCom = CRC_WDATA_COM,
    eCRCAttr_WDataRVS = CRC_WDATA_RVS
} E_CRC_ATTR;

typedef enum
{
    eCRCCpuWLen_8  = CRC_CPU_WDATA_8,
    eCRCCpuWLen_16 = CRC_CPU_WDATA_16,
    eCRCCpuWLen_32 = CRC_CPU_WDATA_32,
} E_CRC_CPUWLEN;

typedef struct
{
    CRCName         crc;
    E_CRC_MODE      eCRCMode;
    E_CRC_ATTR      eCRCAttr;
    E_CRC_CPUWLEN   eCRCCpuWLen;
    uint32_t        u32SeedValue;
} S_CRCDev;

int32_t HAL_CRC_Initialize(S_CRCDev *psCrcDev);
int32_t HAL_CRC_Open(S_CRCDev *psCrcDev);
uint32_t HAL_CRC_Calculate(S_CRCDev *psCrcDev, uint8_t *pu8Buf, uint32_t u32BufLen);
int32_t HAL_CRC_Close(S_CRCDev *psCrcDev);
int32_t HAL_CRC_Finalize(S_CRCDev *psCrcDev);

#ifdef __cplusplus
}
#endif

#endif
