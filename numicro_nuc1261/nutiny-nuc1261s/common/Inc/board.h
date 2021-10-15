#ifndef __BOARD_H
#define __BOARD_H

#include "numicro_hal.h"

#define DEF_ENABLE_HMCU_INDICTOR_BLINK      (1)
#define DEF_ENABLE_RESTORE_FACTORY_SETTING  (0)

typedef enum
{
    // ACK connective pins
    P37 = PE_8, //UART1_TXD
    P38 = PE_9, //UART1_RXD

    P39 = PE_10,  //GPIO_INPUT, ACK_INTERRUPT
    P40 = PE_11,  //GPIO_OUTPUT, ACK_RESET

    P28 = PC_1,   //SmartLight

    P32 = PC_5,   //ACK indicator

    P31 = PC_4,   //Factory setting

    // Note: board-specific
    // UART naming
    VCOM_TXD = PA_2,
    VCOM_RXD = PA_3,
    STDIO_UART_TX   = VCOM_TXD,
    STDIO_UART_RX   = VCOM_RXD,
} Board_PinName;

typedef enum
{
    eUserGPIODev_HeartBeat,
    eUserGPIODev_RestoreFactorySetting,
    eUserGPIODev_ACK_PowerEnable
} E_USERGPIODEV;

typedef enum
{
    eUartDev_DBG,
    eUartDev_ACK
} E_UARTDEV;

extern S_UARTDev    g_asBoardUartDev[];
extern S_GPIODev    g_asBoardGpioDev[];
extern S_GPIODev    g_asBoardUserGpioDev[];
extern S_PWMDev   g_asBoardPwmDev[];
extern S_CRCDev     g_sCrcDev;

void peripheral_init(void);

#endif /* __BOARD */

