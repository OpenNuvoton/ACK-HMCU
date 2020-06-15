#ifndef __BOARD_H
#define __BOARD_H

#include "numicro_hal.h"

#define DEF_ENABLE_HMCU_INDICTOR_BLINK          (1)
#define DEF_ENABLE_RESTORE_FACTORY_SETTING  (1)

typedef enum
{
    // Arduino UNO naming
    A0 = PB_7,
    A1 = PB_6,
    A2 = PB_5,
    A3 = PB_4,

    A4 = PB_0,
    A5 = PB_1,

    D0 = PB_2,
    D1 = PB_3,

    D2 = PC_4,
    D3 = PC_5,

    D4 = PC_3,
    D5 = PC_2,
    D6 = PA_7,
    D7 = PA_6,
    D8 = PA_5,
    D9 = PA_4,
    D10 = PA_3,
    D11 = PA_0,
    D12 = PA_1,
    D13 = PA_2,

    D14 = PC_0,
    D15 = PC_1,

    I2C_SCL = D15,
    I2C_SDA = D14,

    PWM1_CH1 = D2,  //PC_4
    PWM1_CH0 = D3,  //PC_5
    PWM1_CH2 = D4,  //PC_3
    PWM1_CH3 = D5,  //PC_2
    PWM1_CH4 = D6,  //PA_7
    PWM1_CH5 = D7,  //PA_6
    PWM0_CH0 = D8,  //PA_5
    PWM0_CH1 = D9,  //PA_4

    // Note: board-specific
    // UART naming
    USBTX = PB_13,
    USBRX = PB_12,
    STDIO_UART_TX   = USBTX,
    STDIO_UART_RX   = USBRX,
    SERIAL_TX = USBTX,
    SERIAL_RX = USBRX,

    // LED naming
    LEDR1 = PB_14,

    LED_RED = LEDR1,

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

