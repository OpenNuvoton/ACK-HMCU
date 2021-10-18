/**************************************************************************//**
 * @file     NuMicro.h
 * @version  V1.00
 * @brief    NuMicro peripheral access layer header file.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2017-2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __NUMICRO_H__
#define __NUMICRO_H__

#include "M451Series.h"

#if USE_ASSERT
    /**
    * @brief      Assert Function
    *
    * @param[in]  expr  Expression to be evaluated
    *
    * @return     None
    *
    * @details    If the expression is false, an error message will be printed out
    *             from debug port (UART0 or UART1).
    */
    #define ASSERT_PARAM(expr)  { if (!(expr)) { AssertError((uint8_t*)__FILE__, __LINE__); } }

    void AssertError(uint8_t *file, uint32_t line);
#else
    #define ASSERT_PARAM(expr)
#endif

#define assert_param(expr)  ASSERT_PARAM(expr)

#endif  /* __NUMICRO_H__ */


