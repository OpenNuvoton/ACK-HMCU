/**************************************************************************//**
 * @file     board.c
 * @brief    On-board peripheral initialzation.
 * @note
 *
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "numicro_hal.h"

S_CRCDev    g_sCrcDev =
{
    .crc          =  CRC_0,
    .eCRCMode     =  eCRCMode_CRC32,
    .eCRCAttr     = (eCRCAttr_WDataRVS | eCRCAttr_ChecksumCom | eCRCAttr_ChecksumRVS),
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

    /* Select UART module clock source as HIRC and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1));

    /* Set GPA multi-function pins for UART0 RXD and TXD */
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA2MFP_Msk | SYS_GPA_MFPL_PA3MFP_Msk)) | (SYS_GPA_MFPL_PA2MFP_UART0_TXD | SYS_GPA_MFPL_PA3MFP_UART0_RXD);

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
    printf("\n\nCPU @ %dHz\r\n", SystemCoreClock);
}