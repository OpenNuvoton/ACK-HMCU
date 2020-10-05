/**************************************************************************//**
 * @file     ack_user_config.h
 * @brief    Smartlight implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __ACK_USER_CONFIG_H__
#define __ACK_USER_CONFIG_H__

// Debug print behavior. Leave ACK_DEBUG_PRINT_LEVEL undefined for no debug printing.
//#define ACK_DEBUG_PRINT_LEVEL ACK_DEBUG_PRINT_LEVEL_CRITICAL
#define ACK_DEBUG_PRINT_LEVEL ACK_DEBUG_PRINT_LEVEL_INFO

// Size of buffer from which memory blocks are allocated during processing.
#define ACK_MEMORY_POOL_SIZE 1024

// Turn on to enable asserts in ack functions.
// Recommend leaving off in production to save space.
#define ACK_ENABLE_ASSERT

// Alexa capabilities and other ACK features to support.
#define ACK_LOGGING
#define ACK_MODULE_DIAGNOSTICS
#define ACK_POWER_CONTROLLER
#define ACK_BRIGHTNESS_CONTROLLER
#define ACK_MODE_CONTROLLER
#define ACK_COLOR_TEMPERATURE_CONTROLLER
#define ACK_TOGGLE_CONTROLLER

// Enable support for the Host MCU sample applications LED pin in the platform-specific implementation.
#define ACK_HOST_FIRMWARE_UPDATE

#define ACK_HW_PIN_PWM_WARM   0
#define ACK_HW_PIN_PWM_COLD   1

#endif // def __ACK_USER_CONFIG_H__
