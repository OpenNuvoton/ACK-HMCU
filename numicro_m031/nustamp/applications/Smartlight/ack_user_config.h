/*
 * Copyright (c) 2018-Present Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * AMAZON PROPRIETARY/CONFIDENTIAL
 *
 * This file is being provided to you and is licensed as "Software" under the Master Equipment Agreement
 * signed by you and Amazon.com, Inc. or its affiliates (the "Agreement"). See the Agreement for the
 * specific terms and conditions of the Agreement. If you did not receive this file directly from
 * Amazon.com. Inc. or its affiliates subject to an Agreement that governs its use, you have no license
 * to use this file.
 *
 * This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 */

/*
    Configuration file for Alexa Connect Kit. Contains #defines to control its behavior,
    and to determine which features to include when building it.
*/

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
#define ACK_SAMPLE_APPLICATIONS_LED_PIN
#define ACK_HOST_FIRMWARE_UPDATE

#if defined(ACK_SAMPLE_APPLICATIONS_LED_PIN)
    #define ACK_HW_PIN_PWM_WARM   ACK_HW_PIN_SAMPLE_APPLICATIONS_LED
    #define ACK_HW_PIN_PWM_COLD   (ACK_HW_PIN_SAMPLE_APPLICATIONS_LED+1)
#endif

#endif // def __ACK_USER_CONFIG_H__
