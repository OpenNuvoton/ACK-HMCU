/**************************************************************************//**
 * @file     peripheral_pins.c
 * @brief    Peripheral module pin declaration.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
 
#include "peripheral_pins.h"

const PinMap PinMap_ADC[] =
{
    { PB_0,      ADC_0_0,       SYS_GPB_MFPL_PB0MFP_ADC0_CH0  },
    { PB_1,      ADC_0_1,       SYS_GPB_MFPL_PB1MFP_ADC0_CH1  },
    { PB_2,      ADC_0_2,       SYS_GPB_MFPL_PB2MFP_ADC0_CH2  },
    { PB_3,      ADC_0_3,       SYS_GPB_MFPL_PB3MFP_ADC0_CH3  },
    { PB_4,      ADC_0_4,       SYS_GPB_MFPL_PB4MFP_ADC0_CH4  },
    { PB_5,      ADC_0_5,       SYS_GPB_MFPL_PB5MFP_ADC0_CH5  },
    { PB_6,      ADC_0_6,       SYS_GPB_MFPL_PB6MFP_ADC0_CH6  },
    { PB_7,      ADC_0_7,       SYS_GPB_MFPL_PB7MFP_ADC0_CH7  },
    { PB_8,      ADC_0_8,       SYS_GPB_MFPH_PB8MFP_ADC0_CH8  },
    { PB_9,      ADC_0_9,       SYS_GPB_MFPH_PB9MFP_ADC0_CH9  },
    { PB_10,     ADC_0_10,      SYS_GPB_MFPH_PB10MFP_ADC0_CH10  },
    { PB_11,     ADC_0_11,      SYS_GPB_MFPH_PB11MFP_ADC0_CH11  },
    { PB_12,     ADC_0_12,      SYS_GPB_MFPH_PB12MFP_ADC0_CH12  },
    { PB_13,     ADC_0_13,      SYS_GPB_MFPH_PB13MFP_ADC0_CH13  },
    { PB_14,     ADC_0_14,      SYS_GPB_MFPH_PB14MFP_ADC0_CH14  },
    { PB_15,     ADC_0_15,      SYS_GPB_MFPH_PB15MFP_ADC0_CH15  },
    { NC,   NC,    0 }
};

const PinMap PinMap_I2C_SDA[] =
{
    { PA_2,      I2C_1,     SYS_GPA_MFPL_PA2MFP_I2C1_SDA  },
    { PA_4,      I2C_0,     SYS_GPA_MFPL_PA4MFP_I2C0_SDA  },
    { PA_6,      I2C_1,     SYS_GPA_MFPL_PA6MFP_I2C1_SDA  },
    { PA_13,     I2C_1,     SYS_GPA_MFPH_PA13MFP_I2C1_SDA  },
    { PB_0,      I2C_1,     SYS_GPB_MFPL_PB0MFP_I2C1_SDA  },
    { PB_2,      I2C_1,     SYS_GPB_MFPL_PB2MFP_I2C1_SDA  },
    { PB_4,      I2C_0,     SYS_GPB_MFPL_PB4MFP_I2C0_SDA  },
    { PB_10,     I2C_1,     SYS_GPB_MFPH_PB10MFP_I2C1_SDA  },
    { PC_0,      I2C_0,     SYS_GPC_MFPL_PC0MFP_I2C0_SDA  },
    { PC_4,      I2C_1,     SYS_GPC_MFPL_PC4MFP_I2C1_SDA  },
    { PC_8,      I2C_0,     SYS_GPC_MFPH_PC8MFP_I2C0_SDA  },
    { PC_11,     I2C_0,     SYS_GPC_MFPH_PC11MFP_I2C0_SDA  },
    { PD_4,      I2C_1,     SYS_GPD_MFPL_PD4MFP_I2C1_SDA  },
    { PD_6,      I2C_0,     SYS_GPD_MFPL_PD6MFP_I2C0_SDA  },
    { PE_0,      I2C_1,     SYS_GPE_MFPL_PE0MFP_I2C1_SDA  },
    { PF_1,      I2C_1,     SYS_GPF_MFPL_PF1MFP_I2C1_SDA  },
    { PF_2,      I2C_0,     SYS_GPF_MFPL_PF2MFP_I2C0_SDA  },
    { PG_3,      I2C_1,     SYS_GPG_MFPL_PG3MFP_I2C1_SDA  },
    { NC,   NC,    0 }
};

const PinMap PinMap_I2C_SCL[] =
{
    { PA_3,      I2C_1,     SYS_GPA_MFPL_PA3MFP_I2C1_SCL  },
    { PA_5,      I2C_0,     SYS_GPA_MFPL_PA5MFP_I2C0_SCL  },
    { PA_7,      I2C_1,     SYS_GPA_MFPL_PA7MFP_I2C1_SCL  },
    { PA_12,     I2C_1,     SYS_GPA_MFPH_PA12MFP_I2C1_SCL  },
    { PB_1,      I2C_1,     SYS_GPB_MFPL_PB1MFP_I2C1_SCL  },
    { PB_3,      I2C_1,     SYS_GPB_MFPL_PB3MFP_I2C1_SCL  },
    { PB_5,      I2C_0,     SYS_GPB_MFPL_PB5MFP_I2C0_SCL  },
    { PB_11,     I2C_1,     SYS_GPB_MFPH_PB11MFP_I2C1_SCL  },
    { PC_1,      I2C_0,     SYS_GPC_MFPL_PC1MFP_I2C0_SCL  },
    { PC_5,      I2C_1,     SYS_GPC_MFPL_PC5MFP_I2C1_SCL  },
    { PC_12,     I2C_0,     SYS_GPC_MFPH_PC12MFP_I2C0_SCL  },
    { PD_5,      I2C_1,     SYS_GPD_MFPL_PD5MFP_I2C1_SCL  },
    { PD_7,      I2C_0,     SYS_GPD_MFPL_PD7MFP_I2C0_SCL  },
    { PE_1,      I2C_1,     SYS_GPE_MFPL_PE1MFP_I2C1_SCL  },
    { PE_13,     I2C_0,     SYS_GPE_MFPH_PE13MFP_I2C0_SCL  },
    { PF_0,      I2C_1,     SYS_GPF_MFPL_PF0MFP_I2C1_SCL  },
    { PF_3,      I2C_0,     SYS_GPF_MFPL_PF3MFP_I2C0_SCL  },
    { PG_2,      I2C_1,     SYS_GPG_MFPL_PG2MFP_I2C1_SCL  },
    { NC,   NC,    0 }
};

const PinMap PinMap_PWM[] =
{
    { PA_0,      PWM_0_5,   SYS_GPA_MFPL_PA0MFP_PWM0_CH5  },
    { PA_1,      PWM_0_4,   SYS_GPA_MFPL_PA1MFP_PWM0_CH4  },
    { PA_2,      PWM_0_3,   SYS_GPA_MFPL_PA2MFP_PWM0_CH3  },
    { PA_3,      PWM_0_2,   SYS_GPA_MFPL_PA3MFP_PWM0_CH2  },
    { PA_4,      PWM_0_1,   SYS_GPA_MFPL_PA4MFP_PWM0_CH1  },
    { PA_5,      PWM_0_0,   SYS_GPA_MFPL_PA5MFP_PWM0_CH0  },
    { PA_6,      PWM_1_5,   SYS_GPA_MFPL_PA6MFP_PWM1_CH5  },
    { PA_7,      PWM_1_4,   SYS_GPA_MFPL_PA7MFP_PWM1_CH4  },
    { PB_0,      PWM_0_5,   SYS_GPB_MFPL_PB0MFP_PWM0_CH5  },
    { PB_0,      PWM_1_5,   SYS_GPB_MFPL_PB0MFP_PWM1_CH5  },
    { PB_1,      PWM_0_4,   SYS_GPB_MFPL_PB1MFP_PWM0_CH4  },
    { PB_1,      PWM_1_4,   SYS_GPB_MFPL_PB1MFP_PWM1_CH4  },
    { PB_2,      PWM_0_3,   SYS_GPB_MFPL_PB2MFP_PWM0_CH3  },
    { PB_3,      PWM_0_2,   SYS_GPB_MFPL_PB3MFP_PWM0_CH2  },
    { PB_4,      PWM_0_1,   SYS_GPB_MFPL_PB4MFP_PWM0_CH1  },
    { PB_5,      PWM_0_0,   SYS_GPB_MFPL_PB5MFP_PWM0_CH0  },
    { PB_6,      PWM_1_5,   SYS_GPB_MFPL_PB6MFP_PWM1_CH5  },
    { PB_7,      PWM_1_4,   SYS_GPB_MFPL_PB7MFP_PWM1_CH4  },
    { PB_12,     PWM_1_3,   SYS_GPB_MFPH_PB12MFP_PWM1_CH3  },
    { PB_13,     PWM_1_2,   SYS_GPB_MFPH_PB13MFP_PWM1_CH2  },
    { PB_14,     PWM_1_1,   SYS_GPB_MFPH_PB14MFP_PWM1_CH1  },
    { PB_15,     PWM_1_0,   SYS_GPB_MFPH_PB15MFP_PWM1_CH0  },
    { PC_0,      PWM_1_5,   SYS_GPC_MFPL_PC0MFP_PWM1_CH5  },
    { PC_1,      PWM_1_4,   SYS_GPC_MFPL_PC1MFP_PWM1_CH4  },
    { PC_2,      PWM_1_3,   SYS_GPC_MFPL_PC2MFP_PWM1_CH3  },
    { PC_3,      PWM_1_2,   SYS_GPC_MFPL_PC3MFP_PWM1_CH2  },
    { PC_4,      PWM_1_1,   SYS_GPC_MFPL_PC4MFP_PWM1_CH1  },
    { PC_5,      PWM_1_0,   SYS_GPC_MFPL_PC5MFP_PWM1_CH0  },
    { PC_6,      PWM_1_3,   SYS_GPC_MFPL_PC6MFP_PWM1_CH3  },
    { PC_7,      PWM_1_2,   SYS_GPC_MFPL_PC7MFP_PWM1_CH2  },
    { PC_8,      PWM_1_1,   SYS_GPC_MFPH_PC8MFP_PWM1_CH1  },
    { PC_9,      PWM_1_3,   SYS_GPC_MFPH_PC9MFP_PWM1_CH3  },
    { PC_10,     PWM_1_2,   SYS_GPC_MFPH_PC10MFP_PWM1_CH2  },
    { PC_11,     PWM_1_1,   SYS_GPC_MFPH_PC11MFP_PWM1_CH1  },
    { PC_12,     PWM_1_0,   SYS_GPC_MFPH_PC12MFP_PWM1_CH0  },
    { PD_14,     PWM_0_4,   SYS_GPD_MFPH_PD14MFP_PWM0_CH4  },
    { PD_15,     PWM_0_5,   SYS_GPD_MFPH_PD15MFP_PWM0_CH5  },
    { PE_2,      PWM_0_5,   SYS_GPE_MFPL_PE2MFP_PWM0_CH5  },
    { PE_3,      PWM_0_4,   SYS_GPE_MFPL_PE3MFP_PWM0_CH4  },
    { PE_4,      PWM_0_3,   SYS_GPE_MFPL_PE4MFP_PWM0_CH3  },
    { PE_5,      PWM_0_2,   SYS_GPE_MFPL_PE5MFP_PWM0_CH2  },
    { PE_6,      PWM_0_1,   SYS_GPE_MFPL_PE6MFP_PWM0_CH1  },
    { PE_7,      PWM_0_0,   SYS_GPE_MFPL_PE7MFP_PWM0_CH0  },
    { PE_8,      PWM_0_0,   SYS_GPE_MFPH_PE8MFP_PWM0_CH0  },
    { PE_9,      PWM_0_1,   SYS_GPE_MFPH_PE9MFP_PWM0_CH1  },
    { PE_10,     PWM_0_2,   SYS_GPE_MFPH_PE10MFP_PWM0_CH2  },
    { PE_11,     PWM_0_3,   SYS_GPE_MFPH_PE11MFP_PWM0_CH3  },
    { PE_12,     PWM_0_4,   SYS_GPE_MFPH_PE12MFP_PWM0_CH4  },
    { PE_13,     PWM_0_5,   SYS_GPE_MFPH_PE13MFP_PWM0_CH5  },
    { PE_13,     PWM_1_0,   SYS_GPE_MFPH_PE13MFP_PWM1_CH0  },
    { PF_4,      PWM_0_1,   SYS_GPF_MFPL_PF4MFP_PWM0_CH1  },
    { PF_5,      PWM_0_0,   SYS_GPF_MFPL_PF5MFP_PWM0_CH0  },
    { PF_14,     PWM_0_4,   SYS_GPF_MFPH_PF14MFP_PWM0_CH4  },
    { PF_15,     PWM_0_1,   SYS_GPF_MFPH_PF15MFP_PWM0_CH1  },
    { PH_11,     PWM_0_5,   SYS_GPH_MFPH_PH11MFP_PWM0_CH5  },
    { NC,   NC,    0 }
};

const PinMap PinMap_UART_TX[] =
{
    { PA_1,      UART_0,    SYS_GPA_MFPL_PA1MFP_UART0_TXD  },
    { PA_3,      UART_4,    SYS_GPA_MFPL_PA3MFP_UART4_TXD  },
    { PA_3,      UART_1,    SYS_GPA_MFPL_PA3MFP_UART1_TXD  },
    { PA_5,      UART_0,    SYS_GPA_MFPL_PA5MFP_UART0_TXD  },
    { PA_5,      UART_5,    SYS_GPA_MFPL_PA5MFP_UART5_TXD  },
    { PA_7,      UART_0,    SYS_GPA_MFPL_PA7MFP_UART0_TXD  },
    { PA_9,      UART_1,    SYS_GPA_MFPH_PA9MFP_UART1_TXD  },
    { PA_9,      UART_7,    SYS_GPA_MFPH_PA9MFP_UART7_TXD  },
    { PA_11,     UART_6,    SYS_GPA_MFPH_PA11MFP_UART6_TXD  },
    { PA_12,     UART_4,    SYS_GPA_MFPH_PA12MFP_UART4_TXD  },
    { PA_14,     UART_0,    SYS_GPA_MFPH_PA14MFP_UART0_TXD  },
    { PB_1,      UART_2,    SYS_GPB_MFPL_PB1MFP_UART2_TXD  },
    { PB_3,      UART_1,    SYS_GPB_MFPL_PB3MFP_UART1_TXD  },
    { PB_5,      UART_5,    SYS_GPB_MFPL_PB5MFP_UART5_TXD  },
    { PB_5,      UART_2,    SYS_GPB_MFPL_PB5MFP_UART2_TXD  },
    { PB_7,      UART_1,    SYS_GPB_MFPL_PB7MFP_UART1_TXD  },
    { PB_9,      UART_0,    SYS_GPB_MFPH_PB9MFP_UART0_TXD  },
    { PB_9,      UART_7,    SYS_GPB_MFPH_PB9MFP_UART7_TXD  },
    { PB_11,     UART_4,    SYS_GPB_MFPH_PB11MFP_UART4_TXD  },
    { PB_13,     UART_0,    SYS_GPB_MFPH_PB13MFP_UART0_TXD  },
    { PB_15,     UART_3,    SYS_GPB_MFPH_PB15MFP_UART3_TXD  },
    { PC_1,      UART_2,    SYS_GPC_MFPL_PC1MFP_UART2_TXD  },
    { PC_3,      UART_3,    SYS_GPC_MFPL_PC3MFP_UART3_TXD  },
    { PC_5,      UART_2,    SYS_GPC_MFPL_PC5MFP_UART2_TXD  },
    { PC_5,      UART_4,    SYS_GPC_MFPL_PC5MFP_UART4_TXD  },
    { PC_7,      UART_4,    SYS_GPC_MFPL_PC7MFP_UART4_TXD  },
    { PC_7,      UART_6,    SYS_GPC_MFPL_PC7MFP_UART6_TXD  },
    { PC_10,     UART_3,    SYS_GPC_MFPH_PC10MFP_UART3_TXD  },
    { PC_12,     UART_0,    SYS_GPC_MFPH_PC12MFP_UART0_TXD  },
    { PC_12,     UART_6,    SYS_GPC_MFPH_PC12MFP_UART6_TXD  },
    { PC_13,     UART_2,    SYS_GPC_MFPH_PC13MFP_UART2_TXD  },
    { PD_1,      UART_3,    SYS_GPD_MFPL_PD1MFP_UART3_TXD  },
    { PD_3,      UART_0,    SYS_GPD_MFPL_PD3MFP_UART0_TXD  },
    { PD_7,      UART_1,    SYS_GPD_MFPL_PD7MFP_UART1_TXD  },
    { PD_9,      UART_7,    SYS_GPD_MFPH_PD9MFP_UART7_TXD  },
    { PD_11,     UART_1,    SYS_GPD_MFPH_PD11MFP_UART1_TXD  },
    { PE_1,      UART_3,    SYS_GPE_MFPL_PE1MFP_UART3_TXD  },
    { PE_3,      UART_7,    SYS_GPE_MFPL_PE3MFP_UART7_TXD  },
    { PE_5,      UART_6,    SYS_GPE_MFPL_PE5MFP_UART6_TXD  },
    { PE_7,      UART_5,    SYS_GPE_MFPL_PE7MFP_UART5_TXD  },
    { PE_8,      UART_2,    SYS_GPE_MFPH_PE8MFP_UART2_TXD  },
    { PE_10,     UART_3,    SYS_GPE_MFPH_PE10MFP_UART3_TXD  },
    { PE_13,     UART_1,    SYS_GPE_MFPH_PE13MFP_UART1_TXD  },
    { PE_14,     UART_2,    SYS_GPE_MFPH_PE14MFP_UART2_TXD  },
    { PE_14,     UART_6,    SYS_GPE_MFPH_PE14MFP_UART6_TXD  },
    { PF_0,      UART_1,    SYS_GPF_MFPL_PF0MFP_UART1_TXD  },
    { PF_0,      UART_0,    SYS_GPF_MFPL_PF0MFP_UART0_TXD  },
    { PF_3,      UART_0,    SYS_GPF_MFPL_PF3MFP_UART0_TXD  },
    { PF_4,      UART_2,    SYS_GPF_MFPL_PF4MFP_UART2_TXD  },
    { PF_7,      UART_4,    SYS_GPF_MFPL_PF7MFP_UART4_TXD  },
    { PF_11,     UART_5,    SYS_GPF_MFPH_PF11MFP_UART5_TXD  },
    { PG_11,     UART_7,    SYS_GPG_MFPH_PG11MFP_UART7_TXD  },
    { PG_13,     UART_6,    SYS_GPG_MFPH_PG13MFP_UART6_TXD  },
    { PH_4,      UART_6,    SYS_GPH_MFPL_PH4MFP_UART6_TXD  },
    { PH_6,      UART_7,    SYS_GPH_MFPL_PH6MFP_UART7_TXD  },
    { PH_8,      UART_1,    SYS_GPH_MFPH_PH8MFP_UART1_TXD  },
    { PH_10,     UART_4,    SYS_GPH_MFPH_PH10MFP_UART4_TXD  },
    { PH_10,     UART_0,    SYS_GPH_MFPH_PH10MFP_UART0_TXD  },
    { NC,   NC,    0 }
};

const PinMap PinMap_UART_RX[] =
{
    { PA_0,      UART_0,    SYS_GPA_MFPL_PA0MFP_UART0_RXD  },
    { PA_2,      UART_4,    SYS_GPA_MFPL_PA2MFP_UART4_RXD  },
    { PA_2,      UART_1,    SYS_GPA_MFPL_PA2MFP_UART1_RXD  },
    { PA_4,      UART_0,    SYS_GPA_MFPL_PA4MFP_UART0_RXD  },
    { PA_4,      UART_5,    SYS_GPA_MFPL_PA4MFP_UART5_RXD  },
    { PA_6,      UART_0,    SYS_GPA_MFPL_PA6MFP_UART0_RXD  },
    { PA_8,      UART_1,    SYS_GPA_MFPH_PA8MFP_UART1_RXD  },
    { PA_8,      UART_7,    SYS_GPA_MFPH_PA8MFP_UART7_RXD  },
    { PA_10,     UART_6,    SYS_GPA_MFPH_PA10MFP_UART6_RXD  },
    { PA_13,     UART_4,    SYS_GPA_MFPH_PA13MFP_UART4_RXD  },
    { PA_15,     UART_0,    SYS_GPA_MFPH_PA15MFP_UART0_RXD  },
    { PB_0,      UART_2,    SYS_GPB_MFPL_PB0MFP_UART2_RXD  },
    { PB_2,      UART_1,    SYS_GPB_MFPL_PB2MFP_UART1_RXD  },
    { PB_4,      UART_5,    SYS_GPB_MFPL_PB4MFP_UART5_RXD  },
    { PB_4,      UART_2,    SYS_GPB_MFPL_PB4MFP_UART2_RXD  },
    { PB_6,      UART_1,    SYS_GPB_MFPL_PB6MFP_UART1_RXD  },
    { PB_8,      UART_0,    SYS_GPB_MFPH_PB8MFP_UART0_RXD  },
    { PB_8,      UART_7,    SYS_GPB_MFPH_PB8MFP_UART7_RXD  },
    { PB_10,     UART_4,    SYS_GPB_MFPH_PB10MFP_UART4_RXD  },
    { PB_12,     UART_0,    SYS_GPB_MFPH_PB12MFP_UART0_RXD  },
    { PB_14,     UART_3,    SYS_GPB_MFPH_PB14MFP_UART3_RXD  },
    { PC_0,      UART_2,    SYS_GPC_MFPL_PC0MFP_UART2_RXD  },
    { PC_2,      UART_3,    SYS_GPC_MFPL_PC2MFP_UART3_RXD  },
    { PC_4,      UART_2,    SYS_GPC_MFPL_PC4MFP_UART2_RXD  },
    { PC_4,      UART_4,    SYS_GPC_MFPL_PC4MFP_UART4_RXD  },
    { PC_6,      UART_4,    SYS_GPC_MFPL_PC6MFP_UART4_RXD  },
    { PC_6,      UART_6,    SYS_GPC_MFPL_PC6MFP_UART6_RXD  },
    { PC_8,      UART_1,    SYS_GPC_MFPH_PC8MFP_UART1_RXD  },
    { PC_9,      UART_3,    SYS_GPC_MFPH_PC9MFP_UART3_RXD  },
    { PC_11,     UART_0,    SYS_GPC_MFPH_PC11MFP_UART0_RXD  },
    { PC_11,     UART_6,    SYS_GPC_MFPH_PC11MFP_UART6_RXD  },
    { PD_0,      UART_3,    SYS_GPD_MFPL_PD0MFP_UART3_RXD  },
    { PD_2,      UART_0,    SYS_GPD_MFPL_PD2MFP_UART0_RXD  },
    { PD_6,      UART_1,    SYS_GPD_MFPL_PD6MFP_UART1_RXD  },
    { PD_8,      UART_7,    SYS_GPD_MFPH_PD8MFP_UART7_RXD  },
    { PD_10,     UART_1,    SYS_GPD_MFPH_PD10MFP_UART1_RXD  },
    { PD_12,     UART_2,    SYS_GPD_MFPH_PD12MFP_UART2_RXD  },
    { PE_0,      UART_3,    SYS_GPE_MFPL_PE0MFP_UART3_RXD  },
    { PE_2,      UART_7,    SYS_GPE_MFPL_PE2MFP_UART7_RXD  },
    { PE_4,      UART_6,    SYS_GPE_MFPL_PE4MFP_UART6_RXD  },
    { PE_6,      UART_5,    SYS_GPE_MFPL_PE6MFP_UART5_RXD  },
    { PE_9,      UART_2,    SYS_GPE_MFPH_PE9MFP_UART2_RXD  },
    { PE_11,     UART_3,    SYS_GPE_MFPH_PE11MFP_UART3_RXD  },
    { PE_15,     UART_2,    SYS_GPE_MFPH_PE15MFP_UART2_RXD  },
    { PE_15,     UART_6,    SYS_GPE_MFPH_PE15MFP_UART6_RXD  },
    { PF_1,      UART_1,    SYS_GPF_MFPL_PF1MFP_UART1_RXD  },
    { PF_1,      UART_0,    SYS_GPF_MFPL_PF1MFP_UART0_RXD  },
    { PF_2,      UART_0,    SYS_GPF_MFPL_PF2MFP_UART0_RXD  },
    { PF_5,      UART_2,    SYS_GPF_MFPL_PF5MFP_UART2_RXD  },
    { PF_6,      UART_4,    SYS_GPF_MFPL_PF6MFP_UART4_RXD  },
    { PF_10,     UART_5,    SYS_GPF_MFPH_PF10MFP_UART5_RXD  },
    { PG_12,     UART_7,    SYS_GPG_MFPH_PG12MFP_UART7_RXD  },
    { PG_14,     UART_6,    SYS_GPG_MFPH_PG14MFP_UART6_RXD  },
    { PH_5,      UART_6,    SYS_GPH_MFPL_PH5MFP_UART6_RXD  },
    { PH_7,      UART_7,    SYS_GPH_MFPL_PH7MFP_UART7_RXD  },
    { PH_9,      UART_1,    SYS_GPH_MFPH_PH9MFP_UART1_RXD  },
    { PH_11,     UART_4,    SYS_GPH_MFPH_PH11MFP_UART4_RXD  },
    { PH_11,     UART_0,    SYS_GPH_MFPH_PH11MFP_UART0_RXD  },
    { NC,   NC,    0 }
};

const PinMap PinMap_UART_RTS[] =
{
    { PA_0,      UART_1,    SYS_GPA_MFPL_PA0MFP_UART1_nRTS  },
    { PA_4,      UART_0,    SYS_GPA_MFPL_PA4MFP_UART0_nRTS  },
    { PB_3,      UART_5,    SYS_GPB_MFPL_PB3MFP_UART5_nRTS  },
    { PB_8,      UART_1,    SYS_GPB_MFPH_PB8MFP_UART1_nRTS  },
    { PB_10,     UART_0,    SYS_GPB_MFPH_PB10MFP_UART0_nRTS  },
    { PB_13,     UART_3,    SYS_GPB_MFPH_PB13MFP_UART3_nRTS  },
    { PB_14,     UART_0,    SYS_GPB_MFPH_PB14MFP_UART0_nRTS  },
    { PC_3,      UART_2,    SYS_GPC_MFPL_PC3MFP_UART2_nRTS  },
    { PC_6,      UART_0,    SYS_GPC_MFPL_PC6MFP_UART0_nRTS  },
    { PC_10,     UART_6,    SYS_GPC_MFPH_PC10MFP_UART6_nRTS  },
    { PD_3,      UART_3,    SYS_GPD_MFPL_PD3MFP_UART3_nRTS  },
    { PD_8,      UART_2,    SYS_GPD_MFPH_PD8MFP_UART2_nRTS  },
    { PE_0,      UART_4,    SYS_GPE_MFPL_PE0MFP_UART4_nRTS  },
    { PE_3,      UART_6,    SYS_GPE_MFPL_PE3MFP_UART6_nRTS  },
    { PE_5,      UART_7,    SYS_GPE_MFPL_PE5MFP_UART7_nRTS  },
    { PE_12,     UART_1,    SYS_GPE_MFPH_PE12MFP_UART1_nRTS  },
    { PE_13,     UART_4,    SYS_GPE_MFPH_PE13MFP_UART4_nRTS  },
    { PF_4,      UART_2,    SYS_GPF_MFPL_PF4MFP_UART2_nRTS  },
    { PF_9,      UART_5,    SYS_GPF_MFPH_PF9MFP_UART5_nRTS  },
    { PH_4,      UART_7,    SYS_GPH_MFPL_PH4MFP_UART7_nRTS  },
    { PH_8,      UART_3,    SYS_GPH_MFPH_PH8MFP_UART3_nRTS  },
    { NC,   NC,    0 }
};

const PinMap PinMap_UART_CTS[] =
{
    { PA_1,      UART_1,    SYS_GPA_MFPL_PA1MFP_UART1_nCTS  },
    { PA_5,      UART_0,    SYS_GPA_MFPL_PA5MFP_UART0_nCTS  },
    { PB_2,      UART_5,    SYS_GPB_MFPL_PB2MFP_UART5_nCTS  },
    { PB_9,      UART_1,    SYS_GPB_MFPH_PB9MFP_UART1_nCTS  },
    { PB_11,     UART_0,    SYS_GPB_MFPH_PB11MFP_UART0_nCTS  },
    { PB_12,     UART_3,    SYS_GPB_MFPH_PB12MFP_UART3_nCTS  },
    { PB_15,     UART_0,    SYS_GPB_MFPH_PB15MFP_UART0_nCTS  },
    { PC_2,      UART_2,    SYS_GPC_MFPL_PC2MFP_UART2_nCTS  },
    { PC_7,      UART_0,    SYS_GPC_MFPL_PC7MFP_UART0_nCTS  },
    { PC_8,      UART_4,    SYS_GPC_MFPH_PC8MFP_UART4_nCTS  },
    { PC_9,      UART_6,    SYS_GPC_MFPH_PC9MFP_UART6_nCTS  },
    { PD_2,      UART_3,    SYS_GPD_MFPL_PD2MFP_UART3_nCTS  },
    { PD_9,      UART_2,    SYS_GPD_MFPH_PD9MFP_UART2_nCTS  },
    { PE_1,      UART_4,    SYS_GPE_MFPL_PE1MFP_UART4_nCTS  },
    { PE_2,      UART_6,    SYS_GPE_MFPL_PE2MFP_UART6_nCTS  },
    { PE_4,      UART_7,    SYS_GPE_MFPL_PE4MFP_UART7_nCTS  },
    { PE_11,     UART_1,    SYS_GPE_MFPH_PE11MFP_UART1_nCTS  },
    { PF_5,      UART_2,    SYS_GPF_MFPL_PF5MFP_UART2_nCTS  },
    { PF_8,      UART_5,    SYS_GPF_MFPH_PF8MFP_UART5_nCTS  },
    { PH_5,      UART_7,    SYS_GPH_MFPL_PH5MFP_UART7_nCTS  },
    { PH_9,      UART_3,    SYS_GPH_MFPH_PH9MFP_UART3_nCTS  },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_MOSI[] =
{
    { PA_0,      SPI_0,     SYS_GPA_MFPL_PA0MFP_SPI0_MOSI  },
    { PB_12,     SPI_0,     SYS_GPB_MFPH_PB12MFP_SPI0_MOSI  },
    { PD_0,      SPI_0,     SYS_GPD_MFPL_PD0MFP_SPI0_MOSI  },
    { PF_6,      SPI_0,     SYS_GPF_MFPL_PF6MFP_SPI0_MOSI  },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_MISO[] =
{
    { PA_1,      SPI_0,     SYS_GPA_MFPL_PA1MFP_SPI0_MISO  },
    { PB_13,     SPI_0,     SYS_GPB_MFPH_PB13MFP_SPI0_MISO  },
    { PD_1,      SPI_0,     SYS_GPD_MFPL_PD1MFP_SPI0_MISO  },
    { PF_7,      SPI_0,     SYS_GPF_MFPL_PF7MFP_SPI0_MISO  },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_SCLK[] =
{
    { PA_2,      SPI_0,     SYS_GPA_MFPL_PA2MFP_SPI0_CLK  },
    { PB_14,     SPI_0,     SYS_GPB_MFPH_PB14MFP_SPI0_CLK  },
    { PD_2,      SPI_0,     SYS_GPD_MFPL_PD2MFP_SPI0_CLK  },
    { PF_8,      SPI_0,     SYS_GPF_MFPH_PF8MFP_SPI0_CLK  },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_SSEL[] =
{
    { PA_3,      SPI_0,     SYS_GPA_MFPL_PA3MFP_SPI0_SS  },
    { PB_15,     SPI_0,     SYS_GPB_MFPH_PB15MFP_SPI0_SS  },
    { PD_3,      SPI_0,     SYS_GPD_MFPL_PD3MFP_SPI0_SS  },
    { PF_9,      SPI_0,     SYS_GPF_MFPH_PF9MFP_SPI0_SS  },
    { NC,   NC,    0 }
};

const PinMap PinMap_QSPI_D0[] =
{
    { PA_0,      QSPI_0,    SYS_GPA_MFPL_PA0MFP_QSPI0_MOSI0  },
    { PC_0,      QSPI_0,    SYS_GPC_MFPL_PC0MFP_QSPI0_MOSI0  },
    { PE_0,      QSPI_0,    SYS_GPE_MFPL_PE0MFP_QSPI0_MOSI0  },
    { NC,   NC,    0 }
};

const PinMap PinMap_QSPI_D1[] =
{
    { PA_1,      QSPI_0,    SYS_GPA_MFPL_PA1MFP_QSPI0_MISO0  },
    { PC_1,      QSPI_0,    SYS_GPC_MFPL_PC1MFP_QSPI0_MISO0  },
    { PE_1,      QSPI_0,    SYS_GPE_MFPL_PE1MFP_QSPI0_MISO0  },
    { NC,   NC,    0 }
};

const PinMap PinMap_QSPI_D2[] =
{
    { PA_4,      QSPI_0,    SYS_GPA_MFPL_PA4MFP_QSPI0_MOSI1  },
    { PB_0,      QSPI_0,    SYS_GPB_MFPL_PB0MFP_QSPI0_MOSI1  },
    { PC_4,      QSPI_0,    SYS_GPC_MFPL_PC4MFP_QSPI0_MOSI1  },
    { PF_4,      QSPI_0,    SYS_GPF_MFPL_PF4MFP_QSPI0_MOSI1  },
    { PH_11,     QSPI_0,    SYS_GPH_MFPH_PH11MFP_QSPI0_MOSI1  },
    { NC,   NC,    0 }
};

const PinMap PinMap_QSPI_D3[] =
{
    { PA_5,      QSPI_0,    SYS_GPA_MFPL_PA5MFP_QSPI0_MISO1  },
    { PB_1,      QSPI_0,    SYS_GPB_MFPL_PB1MFP_QSPI0_MISO1  },
    { PC_5,      QSPI_0,    SYS_GPC_MFPL_PC5MFP_QSPI0_MISO1  },
    { PF_5,      QSPI_0,    SYS_GPF_MFPL_PF5MFP_QSPI0_MISO1  },
    { PH_10,     QSPI_0,    SYS_GPH_MFPH_PH10MFP_QSPI0_MISO1  },
    { NC,   NC,    0 }
};

const PinMap PinMap_QSPI_SCLK[] =
{
    { PA_2,      QSPI_0,    SYS_GPA_MFPL_PA2MFP_QSPI0_CLK  },
    { PC_2,      QSPI_0,    SYS_GPC_MFPL_PC2MFP_QSPI0_CLK  },
    { PC_14,     QSPI_0,    SYS_GPC_MFPH_PC14MFP_QSPI0_CLK  },
    { PF_2,      QSPI_0,    SYS_GPF_MFPL_PF2MFP_QSPI0_CLK  },
    { PH_8,      QSPI_0,    SYS_GPH_MFPH_PH8MFP_QSPI0_CLK  },
    { NC,   NC,    0 }
};

const PinMap PinMap_QSPI_SSEL[] =
{
    { PA_3,      QSPI_0,    SYS_GPA_MFPL_PA3MFP_QSPI0_SS  },
    { PC_3,      QSPI_0,    SYS_GPC_MFPL_PC3MFP_QSPI0_SS  },
    { PH_9,      QSPI_0,    SYS_GPH_MFPH_PH9MFP_QSPI0_SS  },
    { NC,   NC,    0 }
};

