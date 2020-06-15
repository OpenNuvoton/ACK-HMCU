#ifndef __BOARD_H
#define __BOARD_H

#include "numicro_hal.h"

#define DEF_ENABLE_HMCU_INDICTOR_BLINK          (1)
#define DEF_ENABLE_RESTORE_FACTORY_SETTING  (1)

typedef enum
{
    // Note: board-specific
    // I2C pin
    I2C_SCL = PB_5,
    I2C_SDA = PB_4,

    // UART pin for ACK communication
    ACK_RX  = PB_2,
    ACK_TX  = PB_3,

    // ACK connective
    HOST_INT = PC_4,
    PWR_EN = PB_1,
    RESET_B = PC_5,

    // HMCU misc functions
    GPIO_FS = PB_14,
    GPIO_HB = PB_7,

    // PWM channel pins for lamp board
    PWM0_CH0 =  PA_5,
    PWM0_CH1 =  PA_4,

    // UART pin for HMCU debugging
    STDIO_UART_TX   = PB_13,    //PB13_TXD0
    STDIO_UART_RX   = PB_12,    //PB12_RXD0

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

