/**************************************************************************//**
 * @file     maic.c
 * @brief    ACK HMCU porting.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "numicro_hal.h"

S_CRCDev    g_sCrcDev =
{
    .crc                    =   CRC_0,
    .eCRCMode       =   eCRCMode_CRC32,
    .eCRCAttr       = (eCRCAttr_WDataRVS | eCRCAttr_ChecksumCom | eCRCAttr_ChecksumRVS),
    .eCRCCpuWLen  = eCRCCpuWLen_32,
    .u32SeedValue = 0xffffffff
};

void crc32dev_init(void)
{
    HAL_CRC_Initialize(&g_sCrcDev);
}

void crc32dev_deinit(void)
{
    HAL_CRC_Finalize(&g_sCrcDev);
}

void peripheral_init(void)
{
    //uartdev_init();
    SYS_UnlockReg();

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) | (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    UART_Open(UART0, 115200);

    crc32dev_init();

    SYS_LockReg();
}

void peripheral_deinit(void)
{
    crc32dev_deinit();
    UART_Close(UART0);
    SYS_UnlockReg();
    CLK_DisableModuleClock(UART0_MODULE);
    HAL_SYS_DeInit();
    SYS_LockReg();
}

void mcu_init(void)
{
    HAL_SYS_Init();
    peripheral_init();
    //printf("\n\nCPU @ %dHz\n", SystemCoreClock);
}