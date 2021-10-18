/**************************************************************************//**
 * @file     peripheral_pins.c
 * @brief    Peripheral module pin declaration.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "peripheral_pins.h"
/* PWM */
const PinMap PinMap_PWM[] =
{
    { PA_0,      PWM_1_5,    SYS_GPA_MFPL_PA0MFP_PWM1_CH5  },
    { PA_1,      PWM_1_4,    SYS_GPA_MFPL_PA1MFP_PWM1_CH4  },
    { PA_2,      PWM_1_3,    SYS_GPA_MFPL_PA2MFP_PWM1_CH3  },
    { PA_3,      PWM_1_2,    SYS_GPA_MFPL_PA3MFP_PWM1_CH2  },
    { PB_8,      PWM_0_2,    SYS_GPB_MFPH_PB8MFP_PWM0_CH2  },
    { PC_0,      PWM_0_0,    SYS_GPC_MFPL_PC0MFP_PWM0_CH0  },
    { PC_1,      PWM_0_1,    SYS_GPC_MFPL_PC1MFP_PWM0_CH1  },
    { PC_2,      PWM_0_2,    SYS_GPC_MFPL_PC2MFP_PWM0_CH2  },
    { PC_3,      PWM_0_3,    SYS_GPC_MFPL_PC3MFP_PWM0_CH3  },
    { PC_4,      PWM_0_4,    SYS_GPC_MFPL_PC4MFP_PWM0_CH4  },
    { PC_5,      PWM_0_5,    SYS_GPC_MFPL_PC5MFP_PWM0_CH5  },
    { PC_6,      PWM_1_0,    SYS_GPC_MFPL_PC6MFP_PWM1_CH0  },
    { PC_7,      PWM_1_1,    SYS_GPC_MFPL_PC7MFP_PWM1_CH1  },
    { PD_7,      PWM_0_5,    SYS_GPD_MFPL_PD7MFP_PWM0_CH5  },
    { PD_12,     PWM_1_0,    SYS_GPD_MFPH_PD12MFP_PWM1_CH0  },
    { PD_13,     PWM_1_1,    SYS_GPD_MFPH_PD13MFP_PWM1_CH1  },
    { PD_14,     PWM_1_2,    SYS_GPD_MFPH_PD14MFP_PWM1_CH2  },
    { PD_15,     PWM_1_3,    SYS_GPD_MFPH_PD15MFP_PWM1_CH3  },
    { PE_2,      PWM_0_2,    SYS_GPE_MFPL_PE2MFP_PWM0_CH2  },
    { NC,   NC,    0 }
};

/* UART_TX */
const PinMap PinMap_UART_TX[] =
{
    { PA_0,      UART_1,     SYS_GPA_MFPL_PA0MFP_UART1_TXD  },
    { PA_2,      UART_0,     SYS_GPA_MFPL_PA2MFP_UART0_TXD  },
    { PB_1,      UART_2,     SYS_GPB_MFPL_PB1MFP_UART2_TXD  },
    { PB_3,      UART_1,     SYS_GPB_MFPL_PB3MFP_UART1_TXD  },
    { PC_2,      UART_2,     SYS_GPC_MFPL_PC2MFP_UART2_TXD  },
    { PD_1,      UART_0,     SYS_GPD_MFPL_PD1MFP_UART0_TXD  },
    { PD_12,     UART_0,     SYS_GPD_MFPH_PD12MFP_UART0_TXD  },
    { PE_7,      UART_0,     SYS_GPE_MFPL_PE7MFP_UART0_TXD  },
    { PE_8,      UART_1,     SYS_GPE_MFPH_PE8MFP_UART1_TXD  },
    { PE_12,     UART_1,     SYS_GPE_MFPH_PE12MFP_UART1_TXD  },
    { NC,   NC,    0 }
};

/* UART_RX */
const PinMap PinMap_UART_RX[] =
{
    { PA_1,      UART_1,     SYS_GPA_MFPL_PA1MFP_UART1_RXD  },
    { PA_3,      UART_0,     SYS_GPA_MFPL_PA3MFP_UART0_RXD  },
    { PB_0,      UART_2,     SYS_GPB_MFPL_PB0MFP_UART2_RXD  },
    { PB_2,      UART_1,     SYS_GPB_MFPL_PB2MFP_UART1_RXD  },
    { PB_4,      UART_2,     SYS_GPB_MFPL_PB4MFP_UART2_RXD  },
    { PB_5,      UART_2,     SYS_GPB_MFPL_PB5MFP_UART2_RXD  },
    { PC_3,      UART_2,     SYS_GPC_MFPL_PC3MFP_UART2_RXD  },
    { PD_0,      UART_0,     SYS_GPD_MFPL_PD0MFP_UART0_RXD  },
    { PD_9,      UART_0,     SYS_GPD_MFPH_PD9MFP_UART0_RXD  },
    { PD_13,     UART_0,     SYS_GPD_MFPH_PD13MFP_UART0_RXD  },
    { PE_6,      UART_0,     SYS_GPE_MFPL_PE6MFP_UART0_RXD  },
    { PE_9,      UART_1,     SYS_GPE_MFPH_PE9MFP_UART1_RXD  },
    { PE_13,     UART_1,     SYS_GPE_MFPH_PE13MFP_UART1_RXD  },
    { NC,   NC,    0 }
};

/* UART_RTS */
const PinMap PinMap_UART_RTS[] =
{
    { PA_1,      UART_1,     SYS_GPA_MFPL_PA1MFP_UART1_nRTS  },
    { PA_3,      UART_0,     SYS_GPA_MFPL_PA3MFP_UART0_nRTS  },
    { PB_8,      UART_1,     SYS_GPB_MFPH_PB8MFP_UART1_nRTS  },
    { PC_1,      UART_2,     SYS_GPC_MFPL_PC1MFP_UART2_nRTS  },
    { PD_15,     UART_0,     SYS_GPD_MFPH_PD15MFP_UART0_nRTS  },
    { PE_2,      UART_1,     SYS_GPE_MFPL_PE2MFP_UART1_nRTS  },
    { PE_11,     UART_1,     SYS_GPE_MFPH_PE11MFP_UART1_nRTS  },
    { NC,   NC,    0 }
};

/* UART_CTS */
const PinMap PinMap_UART_CTS[] =
{
    { PA_0,      UART_1,     SYS_GPA_MFPL_PA0MFP_UART1_nCTS  },
    { PA_2,      UART_0,     SYS_GPA_MFPL_PA2MFP_UART0_nCTS  },
    { PB_4,      UART_1,     SYS_GPB_MFPL_PB4MFP_UART1_nCTS  },
    { PC_0,      UART_2,     SYS_GPC_MFPL_PC0MFP_UART2_nCTS  },
    { PD_8,      UART_0,     SYS_GPD_MFPH_PD8MFP_UART0_nCTS  },
    { PD_14,     UART_0,     SYS_GPD_MFPH_PD14MFP_UART0_nCTS  },
    { PE_10,     UART_1,     SYS_GPE_MFPH_PE10MFP_UART1_nCTS  },
    { NC,   NC,    0 }
};

