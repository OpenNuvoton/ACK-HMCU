/**************************************************************************//**
 * @brief    ACK HMCU platform porting.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "ack.h"
#include <stdarg.h>
#include <numicro_hal.h>
#include <inttypes.h>
#include "ack_user_platform.h"
#include "main.h"
#include "board.h"
#include "ack_numicro_ota.h"

#define ACK_DEBUG_PRINT_UART    &g_asBoardUartDev[eUartDev_DBG]
#define ACK_MODULE_UART         &g_asBoardUartDev[eUartDev_ACK]



#ifdef ACK_HOST_FIRMWARE_UPDATE
    //#define DEF_OFFLINEFWUPDATE
    static uint32_t sg_otaSize;
    static uint32_t sg_otaCrc32;
#endif

/* Initializes your Platform-Specific routines.
 * Your implementation should set up UARTs, GPIO pins,
 * and similar resources needed by the implementations of rest of the ACKPlatform_* functions. */
void ACKPlatform_Initialize(void)
{
#if defined(DEF_OFFLINEFWUPDATE)
    uint32_t crc32;
    /* Do an offline OTA firmware upgrading flow testing. */
    /* You should write another firmware into OTA_STAGING parition before doing the testing. */
    ACK_DEBUG_PRINT_C("Do an offline OTA firmware upgrading flow testing\r\n");

    /* Prepare sg_otaSize and sg_otaSize variables. */
    sg_otaSize = ACK_NUMICRO_OTA_STAGING_PARTITION_SIZE;
    crc32 = ACKPlatform_CalculateCrc32((const void *)ACK_NUMICRO_OTA_STAGING_PARTITION_START, sg_otaSize);
    sg_otaCrc32 = crc32;

    ACK_DEBUG_PRINT_C("calculated sg_otaCrc32=%08x\r\n", sg_otaCrc32);

    /* Calculate CRC32 checksum */
    if (ACKPlatform_HostFirmwareUpdateSuccessfullyRetrieved())
    {
        ACK_DEBUG_PRINT_C("Applying\r\n");

        /* Write OTA otagging in OTA_STATUS partition if checksum is valid. */
        ACKPlatform_ApplyHostFirmwareUpdate();
    }
#endif
}

/* Provide elapsed milliseconds (can be used for timing)
 * and the ability to delay operation for a period of milliseconds, respectively. */
uint32_t ACKPlatform_TickCount(void)
{
    return HAL_SYS_TICK_Get();
}

void ACKPlatform_Delay(uint32_t milliseconds)
{
    HAL_SYS_Delay(milliseconds);
}

/* Provide communications with the ACK Connectivity Module (over a UART). */
bool ACKPlatform_Send(const void *pBuffer, size_t length, uint32_t timeoutMilliseconds)
{
    if (HAL_UART_Send(ACK_MODULE_UART, pBuffer, length, timeoutMilliseconds) == HAL_OK)
        return true;

    return false;
}

bool ACKPlatform_Receive(void *pBuffer, size_t length, uint32_t timeoutMilliseconds)
{
    uint32_t recv_size = 0;
    HAL_Status ret = HAL_UART_Recv(ACK_MODULE_UART, pBuffer, length, &recv_size, timeoutMilliseconds);
    if ((ret != HAL_OK) || (recv_size != length))
    {
        ACK_DEBUG_PRINT_E("[%d]expect size:%d != real returned size:%d timeout=%d \r\n", HAL_SYS_TICK_Get(), length, recv_size, timeoutMilliseconds);
        return false;
    }

    return true;
}

void ACKPlatform_DrainRead(void)
{
    HAL_UART_DrainRxBuf(ACK_MODULE_UART);
}

/* Calculates a 32-bit CRC value.
 * This is a platform-specific function because some MCUs have hardware assist
 * for CRC calculation, but note that your implementation must use the same
 * CRC algorithm as is used by the Alexa Connect Kit Connectivity Module.
 * See the implementation in ack_arduino_platform.cpp for more information.
 * */
uint32_t ACKPlatform_CalculateCrc32(const void *pInput, size_t length)
{
    uint32_t u32Ret = HAL_ERROR;
    HAL_CRC_Open(&g_sCrcDev);
    u32Ret = HAL_CRC_Calculate(&g_sCrcDev, (uint8_t *)pInput, length);
    HAL_CRC_Close(&g_sCrcDev);
    return u32Ret;
}

/* Writes messages to a platform-specific debug output,
 * typically (but not mandatorily) a UART to which a serial monitor is attached.
 * Your implementation should format the message and send it to the platform-specific output.
 * */
void ACKPlatform_DebugPrint(const char *pMessage)
{
    size_t count;

    count = strlen(pMessage);

    if (count >= 0)
    {
        HAL_UART_Send(ACK_DEBUG_PRINT_UART, pMessage, count, 1000);
    }
}

void ACKPlatform_WriteDigitalPin(ACKHardwarePin_t pin, bool state)
{
    if (state)
        HAL_GPIO_OutputHigh(&g_asBoardGpioDev[pin]);
    else
        HAL_GPIO_OutputLow(&g_asBoardGpioDev[pin]);
}

bool ACKPlatform_ReadDigitalPin(ACKHardwarePin_t pin)
{
    uint32_t value = 1;
    HAL_GPIO_InputGetValue(&g_asBoardGpioDev[pin], &value);
    return (value > 0);
}

void ACKPlatform_SetDigitalPinPWMLevel(ACKHardwarePin_t pin, uint8_t val)
{
    S_PWMDev *psPwmDev = &g_asBoardPwmDev[pin];

    if (val)
        psPwmDev->dutycycle = (float)(val + 1) * 100 / 256;
    else
        psPwmDev->dutycycle = 0;

    ACK_DEBUG_PRINT_I("pin=%d, value=%d -> freq=%d(Hz), dutycycle=%d(%%)", pin, val, psPwmDev->frequency, psPwmDev->dutycycle);
    HAL_PWM_Start(psPwmDev);
}

#ifdef ACK_HOST_FIRMWARE_UPDATE

bool ACKPlatform_StartHostFirmwareUpdate(uint32_t size, uint32_t targetAddress, uint32_t crc32)
{
    bool ret = true;
    sg_otaSize = size;
    sg_otaCrc32 = crc32;

    ACKNuMicroOta_FlashBegin();
    if (ACK_NUMICRO_OTA_PRIMARY_PARTITION_START != targetAddress)
    {
        ACK_DEBUG_PRINT_E(
            "Host firmware update targets address %"PRIx32" but primary partition is at %"PRIx32".",
            targetAddress,
            ACK_NUMICRO_OTA_PRIMARY_PARTITION_START);
        ret = false;
        goto exit_ACKPlatform_StartHostFirmwareUpdate;
    }
#ifndef DEF_OFFLINEFWUPDATE
    else if (!ACKNuMicroOta_FlashErase(ACK_NUMICRO_OTA_STAGING_PARTITION_START / FLASH_PAGE_SIZE, size))
    {
        ACK_DEBUG_PRINT_E(
            "Erasing all OTA Staging partition %"PRIx32" %"PRIx32" is failure.",
            ACK_NUMICRO_OTA_STAGING_PARTITION_START,
            size);
        ret = false;
        goto exit_ACKPlatform_StartHostFirmwareUpdate;
    }
#endif
exit_ACKPlatform_StartHostFirmwareUpdate:
    ACKNuMicroOta_FlashEnd();
    return ret;
}

bool ACKPlatform_SaveHostFirmwareUpdateChunk(uint32_t startOffset, const uint8_t *pData, uint32_t size)
{
    bool result;
    uint32_t partitionOffset;
    uint32_t unitCount;

    ACKNuMicroOta_FlashBegin();

    partitionOffset = ACK_NUMICRO_OTA_STAGING_PARTITION_START + startOffset;

    // The routine to write to flash takes a count of 16-bit words, not a byte count.
    unitCount = (size % 1) ? ((size + 1) / 2) : (size / 2);

    result = ACKNuMicroOta_FlashWrite(partitionOffset, pData, unitCount);

    ACKNuMicroOta_FlashEnd();

    if (!result)
    {
        ACK_DEBUG_PRINT_E(
            "Unable to write OTA chunk of 0x%"PRIx32" bytes to offset 0x%"PRIx32" in Staging Partition.",
            unitCount * 2,
            partitionOffset);
        return false;
    }

    return true;
}

bool ACKPlatform_HostFirmwareUpdateSuccessfullyRetrieved(void)
{
    uint32_t crc32;
    ACKNuMicroOtaStatusPartition_t statusPartition;
    bool success;

    // Check CRC.
    crc32 = ACKPlatform_CalculateCrc32((const void *)ACK_NUMICRO_OTA_STAGING_PARTITION_START, sg_otaSize);
    if (crc32 != sg_otaCrc32)
    {
        ACK_DEBUG_PRINT_E(
            "Calculated OTA image CRC 0x%"PRIx32" does not match expected CRC 0x%"PRIx32".",
            crc32,
            sg_otaCrc32);

        return false;
    }

    // CRC is OK. Update the status partition to indicate that an update image is present.
    memset(&statusPartition, 0, sizeof(statusPartition));
    statusPartition.Signature1 = ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE1;
    statusPartition.Signature2 = ACK_NUMICRO_OTA_STATUS_PARTITION_SIGNATURE2;
    statusPartition.State = ACK_NUMICRO_OTA_STATUS_PARTITION_STATE_UNAPPLIED_IMAGE;
    statusPartition.Crc32 = sg_otaCrc32;
    statusPartition.ImageStartAddress = ACK_NUMICRO_OTA_PRIMARY_PARTITION_START;
    statusPartition.ImageSize = sg_otaSize;

    ACKNuMicroOta_FlashBegin();

    success = false;

    if (ACKNuMicroOta_FlashErase(
                ACK_NUMICRO_OTA_STATUS_PARTITION_START / FLASH_PAGE_SIZE,
                ACK_NUMICRO_OTA_STATUS_PARTITION_SIZE))
    {
        ACK_STATIC_ASSERT(0 == (sizeof(ACKNuMicroOtaStatusPartition_t) % 1));
        if (ACKNuMicroOta_FlashWrite(
                    ACK_NUMICRO_OTA_STATUS_PARTITION_START,
                    &statusPartition,
                    sizeof(ACKNuMicroOtaStatusPartition_t) / 2))
        {
            success = true;
        }
        else
        {
            ACK_DEBUG_PRINT_E("Writing Status Partition failed.");
        }
    }
    else
    {
        ACK_DEBUG_PRINT_E("Erasing Status Partition failed.");
    }

    ACKNuMicroOta_FlashEnd();

    return success;
}

void ACKPlatform_HostFirmwareUpdateFailed(void)
{
    // No-op on this platform.
}

void ACKPlatform_ApplyHostFirmwareUpdate(void)
{
    // Does not return.

    /* Unlock protected registers */
    SYS_UnlockReg();

    FMC_Open();

    ACK_DEBUG_PRINT_C("VECMAP = 0x%x -> 0x%08x\n", FMC_GetVECMAP(), ACK_NUMICRO_OTA_LOADER_PARTITION_START);
    while ((UART0->FIFOSTS & UART_FIFOSTS_TXEMPTY_Msk) == 0);

    FMC_SetVectorPageAddr(ACK_NUMICRO_OTA_LOADER_PARTITION_START);

    /* Reset Chip to reload new CONFIG value */
    SYS_ResetChip();

    // Never get here.
    /* Lock protected registers */
    SYS_LockReg();
}

#endif // def ACK_HOST_FIRMWARE_UPDATE


#if DEF_ENABLE_HMCU_INDICTOR_BLINK
#define DEF_HEARTBEAT_PERIOD    1000
#define DEF_HEARTBEAT_DELAY     70
static uint32_t g_u32HeartBeatPhase = 0;
static uint32_t g_u32HeartBeatPeriod = DEF_HEARTBEAT_PERIOD;
static uint32_t g_u32HeartBeatNextPhase = 0;
extern S_GPIODev g_asBoardUserGpioDev[];
#define DEF_LED_HEARTBEAT       &g_asBoardUserGpioDev[eUserGPIODev_HeartBeat]

static void Led_Heartbeat(void)
{
    unsigned long brightness = 0;
    unsigned long delay = 0;
    uint32_t u32CurMs = HAL_SYS_TICK_Get();

    switch (ACK_LifecycleState)
    {
    case ACK_LIFECYCLE_NOT_CONNECTED_TO_ALEXA:
        HAL_GPIO_OutputHigh(DEF_LED_HEARTBEAT);
        break;

    case ACK_LIFECYCLE_CONNECTED_TO_ALEXA:
        if (u32CurMs < g_u32HeartBeatNextPhase)
            return;

        switch (g_u32HeartBeatPhase)
        {
        case 0:
            g_u32HeartBeatPeriod =  DEF_HEARTBEAT_PERIOD;
            delay = DEF_HEARTBEAT_DELAY * 4;
            g_u32HeartBeatPhase++;
            brightness = 1;
            break;
        case 1:
            delay = g_u32HeartBeatPeriod / 4 - DEF_HEARTBEAT_DELAY;
            g_u32HeartBeatPhase++;
            break;
        case 2:
            delay = DEF_HEARTBEAT_DELAY * 2;
            g_u32HeartBeatPhase++;
            brightness = 1;
            break;
        default:
            delay = g_u32HeartBeatPeriod - g_u32HeartBeatPeriod / 4 -   DEF_HEARTBEAT_DELAY;
            g_u32HeartBeatPhase = 0;
            break;
        } //switch (g_u32HeartBeatPhase)

        if (brightness)
            HAL_GPIO_OutputLow(DEF_LED_HEARTBEAT);
        else
            HAL_GPIO_OutputHigh(DEF_LED_HEARTBEAT);

        g_u32HeartBeatNextPhase = u32CurMs + delay ;

        break;
    default:
        break;
    } //switch (ACK_LifecycleState)
}
#endif //#DEF_ENABLE_HMCU_INDICTOR_BLINK


int g_bDoRestoreFactorySetting = 0;
#if DEF_ENABLE_RESTORE_FACTORY_SETTING
#define DEF_DO_FACTORY_SETTING_MS   5000
#define DEF_BUTTON_DORESTOREFACTORYSETTING      &g_asBoardUserGpioDev[eUserGPIODev_RestoreFactorySetting]

static uint32_t g_u32ButtonFactorySettingNextPhase = 0;
static void Button_DoRestoreFactorySetting(void)
{
    uint32_t value = 1;
    uint32_t u32CurMs = HAL_SYS_TICK_Get();

    HAL_GPIO_InputGetValue(DEF_BUTTON_DORESTOREFACTORYSETTING, &value);

    if (value)
    {
        g_bDoRestoreFactorySetting = 0;
        g_u32ButtonFactorySettingNextPhase = u32CurMs;
    }
    else if ((u32CurMs - g_u32ButtonFactorySettingNextPhase) >= DEF_DO_FACTORY_SETTING_MS)
    {
        g_bDoRestoreFactorySetting = 1;
        g_u32ButtonFactorySettingNextPhase = u32CurMs; // Wait next DEF_DO_FACTORY_SETTING_MS timeout
    }
}
#endif //DEF_ENABLE_RESTORE_FACTORY_SETTING

void HAL_SYS_TICK_InvokeCallback(void)
{
#if DEF_ENABLE_HMCU_INDICTOR_BLINK
    Led_Heartbeat();
#endif
#if DEF_ENABLE_RESTORE_FACTORY_SETTING
    Button_DoRestoreFactorySetting();
#endif
}

void platform_ack_irq_handler_rx(void *psUARTDev, E_UART_EVENT eHalUartEvent)
{

}

void platform_ack_irq_handler_tx(void *psUARTDev, E_UART_EVENT eHalUartEvent)
{

}

