#ifndef __BOARD_H
#define __BOARD_H

#include "numicro_hal.h"

#define DEF_ENABLE_HMCU_INDICTOR_BLINK          (1)
#define DEF_ENABLE_RESTORE_FACTORY_SETTING  (0)

typedef enum
{
    // ACK connective pins
    P66 = PE_8,   //UART1_TXD
    P67 = PE_9,   //UART1_RXD
    P68 = PE_10,  //GPIO_INPUT, ACK_INTERRUPT
    P69 = PE_11,  //GPIO_OUTPUT, ACK_RESET
    P45 = PC_1,   //SmartLight
    P38 = PC_9,   //ACK indicator
    P43 = PC_14,  //Factory setting
    // Note: board-specific
    // UART naming
    STDIO_UART_TX   = PD_1,
    STDIO_UART_RX   = PD_0,
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

