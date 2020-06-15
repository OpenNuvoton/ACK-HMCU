/**************************************************************************//**
 * @file     Smartlight.c
 * @brief    Smartlight implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

/*
 *   ACK Host MCU sample application showing implementation of a Smart Light.
 *   The usage of the following capabilities is demonstrated:
 *    * Power - powers the light on/off.
 *    * Brightness - controls the brightness of the LED using PWM output.
 *    * Color temperature - controls the color temperature of the LED using warm/cold PWMs output.
 *    * Mode (Shutoff time) - shuts the light of after the specified time.
 *    * Mode (Toggle) - sets the light blinking speed.
*/

#include "ack.h"
#include "ack_brightness_controller.h"
#include "ack_user_device.h"
#include "ack_logging.h"
#include "ack_mode_controller.h"
#include "ack_power_controller.h"
#include <inttypes.h>
#include <stdint.h>

// Component name used for logs originated from this file.
#define LOG_COMPONENT_NAME "YanKon_BR30"

// Forward declarations.
bool AddPowerProperty(uint32_t propertyOrdinal, unsigned propertyFlags);
bool AddLightBrightnessProperty(uint32_t propertyOrdinal, unsigned propertyFlags);
bool AddLightTimerProperty(uint32_t propertyOrdinal, unsigned propertyFlags);
bool AddLightToggleProperty(uint32_t propertyOrdinal, unsigned propertyFlags);
bool AddLightColorTemperatureProperty(uint32_t propertyOrdinal, unsigned propertyFlags);

ACKPropertiesBits_t ResetBlinkingMode(void);
ACKPropertiesBits_t ResetShutdownTimeMode(void);
void ProcessSetBlinkDirective(int32_t correlationId, bool value);
void ProcessSetTimerModeDirective(int32_t correlationId, bool isDelta, int32_t value);
ACKPropertiesBits_t SetLightToOnIfOff();
void CheckShutoffTimer();
void CheckBlinkingMode();
void Hardware_TurnLightOn();
void Hardware_TurnLightOff();

// Application-specific property ordinals.
#define PROPERTY_POWER_STATE 0 // Power controller ordinal.
#define PROPERTY_BRIGHTNESS 1  // Brightness controller ordinal.
#define PROPERTY_COLORTEMPERATURE_MODE 2  // ColorTemperature (ColorTemperature) controller ordinal.
#define PROPERTY_TOGGLE_MODE 3  // Blinking (toggle) controller ordinal.
#define PROPERTY_TIMER_MODE 4  // Timer (mode) controller ordinal.

// Maps an Alexa capability in this application to a routine used to add the capability's properties
// when an outbound event is being built.
ACKPropertyTableEntry_t ACKUser_PropertyTable[] =
{
    { PROPERTY_POWER_STATE, AddPowerProperty },
    { PROPERTY_BRIGHTNESS, AddLightBrightnessProperty },
    { PROPERTY_COLORTEMPERATURE_MODE, AddLightColorTemperatureProperty },
    { PROPERTY_TOGGLE_MODE, AddLightToggleProperty },
    { PROPERTY_TIMER_MODE, AddLightTimerProperty },
    { 0, NULL }
};

// All Light related properties.
static const ACKPropertiesBits_t c_lightPropertiesBits =
    ACK_PROPERTY_BIT(PROPERTY_POWER_STATE)
    | ACK_PROPERTY_BIT(PROPERTY_BRIGHTNESS)
    | ACK_PROPERTY_BIT(PROPERTY_COLORTEMPERATURE_MODE)
    | ACK_PROPERTY_BIT(PROPERTY_TOGGLE_MODE)
    | ACK_PROPERTY_BIT(PROPERTY_TIMER_MODE);

// Mode controller instance ids.
// When multiple mode controllers are registered, each is assigned with a unique instance id.
// DO NOT CHANGE the instance ids, since they must match the device type configuration.
#define MODE_INSTANCE_TIMER 12
#define MODE_INSTANCE_BLINKING 2

// Timer modes.
// Same as with the instance ids, these values must match the device type configuration.
#define TIMER_MODE_NONE 0
#define TIMER_MODE_5_MIN 1
#define TIMER_MODE_10_MIN 2
#define TIMER_MODE_60_MIN 3

// Blinking modes.
// Same as with the instance ids, these values must match the device type configuration.
#define SPEED_MODE_NONE 0
#define SPEED_MODE_TOGGLE 1

#define MAXIMUM_BRIGHTNESS 100
#define MINIMUM_BRIGHTNESS 0

typedef enum
{
    eColTemp_2200K = 2200,
    eColTemp_2700K = 2700,
    eColTemp_4000K = 4000,
    eColTemp_5500K = 5500,
    eColTemp_7000K = 7000,
    eColTemp_MIN = eColTemp_2200K,
    eColTemp_MAX = eColTemp_7000K
} E_COLORTEMPERATURE;

typedef struct
{
    E_COLORTEMPERATURE  m_eColTemp;
    uint8_t                         m_u8DutyCycle_Warm;
    uint8_t                         m_u8DutyCycle_Cold;
} S_ColTempMap;

const static S_ColTempMap BR30_ColTempMapTbl[] =
{
    {eColTemp_2200K, 100, 0},
    {eColTemp_2700K, 84,    16},
    {eColTemp_4000K, 49,    51},
    {eColTemp_5500K, 25,    75},
    {eColTemp_7000K, 0,     100},
};
const int i32MaxColTempMapTblSize  = sizeof(BR30_ColTempMapTbl) / sizeof(BR30_ColTempMapTbl[0]);


// Global variables.
uint32_t g_previousTick = 0;   // Previous Tick (used by timers logic).
uint8_t g_brightness = MAXIMUM_BRIGHTNESS; // Brightness.
uint8_t g_blinkingMode = SPEED_MODE_NONE;    // Blinking mode.
uint8_t g_timerMode = TIMER_MODE_NONE;   // Timer mode.
uint32_t g_colortemperature = (uint32_t)eColTemp_2200K;

bool g_power = false;           // Light power.
int32_t g_timerModeCountdown = 0;  // Timer countdown.
int32_t g_blinkingCountdown = 0; // Blinking countdown.
bool g_blinkingPower = false;     // Indicates power for blinking mode.

// Called once, for one-time initialization.
void setup()
{
    // Initialize ACK Host MCU Implementation Core.
    ACK_Initialize();

    // Initialize the previous tick.
    g_previousTick = ACKPlatform_TickCount();
}

// Called over and over, for main processing.
void loop()
{
    ACK_Process();

    // Timers are checked every loop iteration.
    CheckShutoffTimer();
    CheckBlinkingMode();

    // In order to work with timers, we save time of the last time timers were called.
    // This line must be after all timer logic is called.
    g_previousTick = ACKPlatform_TickCount();
}

// We designate the device "in use" if (simulated) power is on.
bool ACKUser_IsDeviceInUse(void)
{
    return g_power;
}

// Power Controller directive callback, turns on a small LED.
void ACKUser_OnPowerControllerDirective(int32_t correlationId, bool powerOn)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;
    bool changed;

#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling power directive: %s",
        powerOn ? "ON" : "OFF");
#endif

    ACK_DEBUG_PRINT_I("Controlling LED with power controller directive; light %s", powerOn ? "ON" : "OFF");

    changed = (0 == g_power) != (0 == powerOn);
    if (changed)
    {
        // In case of power mode change, the logic stops blinking and the timer is stopped.
        changedPropertiesBits |= ResetShutdownTimeMode();
        changedPropertiesBits |= ResetBlinkingMode();
        changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_POWER_STATE);
        if (powerOn)
        {
            Hardware_TurnLightOn();
            g_power = true;
        }
        else
        {
            Hardware_TurnLightOff();
            g_power = false;
        }
    }

    // Indicate that processing the directive completed successfully,
    // including a change report if and only if the power state actually changed.
    ACK_CompleteDirectiveWithSuccess(
        correlationId,
        c_lightPropertiesBits,
        changedPropertiesBits);
}


ACKPropertiesBits_t ResetShutdownTimeMode(void)
{
    if (g_timerMode != TIMER_MODE_NONE)
    {
        g_timerMode = TIMER_MODE_NONE;
        return ACK_PROPERTY_BIT(PROPERTY_TIMER_MODE);
    }

    return 0;
}

ACKPropertiesBits_t ResetBlinkingMode(void)
{
    if (g_blinkingMode != SPEED_MODE_NONE)
    {
        g_blinkingMode = SPEED_MODE_NONE;
        g_blinkingCountdown = 0;
        return ACK_PROPERTY_BIT(PROPERTY_TOGGLE_MODE);
    }

    return 0;
}

void ACKUser_OnBrightnessControllerDirective(int32_t correlationId, bool isDelta, int32_t value)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;

#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling Set Brightness directive: %d %"PRIi32,
        isDelta,
        value);
#endif

    // isDelta means that user asked to increase or decrease the brightness.
    if (isDelta)
    {
        if (g_brightness + value > MAXIMUM_BRIGHTNESS)
        {
            if (g_brightness != MAXIMUM_BRIGHTNESS)
            {
                g_brightness = MAXIMUM_BRIGHTNESS;
                changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_BRIGHTNESS);
            }
        }
        else if (g_brightness + value < MINIMUM_BRIGHTNESS)
        {
            if (g_brightness != MINIMUM_BRIGHTNESS)
            {
                g_brightness = MINIMUM_BRIGHTNESS;
                changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_BRIGHTNESS);
            }
        }
        else
        {
            g_brightness += value;
            changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_BRIGHTNESS);
        }
    }
    else
    {
        if ((value > MAXIMUM_BRIGHTNESS) || (value < MINIMUM_BRIGHTNESS))
        {
            ACK_CompleteDirectiveWithOutOfRangeError(
                correlationId,
                MINIMUM_BRIGHTNESS,
                MAXIMUM_BRIGHTNESS,
                NULL);

            return;
        }

        if (g_brightness != value)
        {
            g_brightness = value;
            changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_BRIGHTNESS);
        }
    }

    if (changedPropertiesBits)
    {
        // If power was off, turn it on.
        if (!g_power)
        {
            g_power = true;
            changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_POWER_STATE);
        }

        Hardware_TurnLightOn();
    }


    // Indicate that processing the directive completed successfully,
    // including a change report if and only if the brightness state actually changed.
    ACK_CompleteDirectiveWithSuccess(
        correlationId,
        c_lightPropertiesBits,
        changedPropertiesBits);
}

ACKPropertiesBits_t SetLightToOnIfOff()
{
    if (!g_power)
    {
        g_power = true;
        Hardware_TurnLightOn();
        return ACK_PROPERTY_BIT(PROPERTY_POWER_STATE);
    }

    return 0;
}

// Example: Mode Controller.
// In this example two instances are supported:
//   12 to control light timer
//   13 to control light blinking mode
void ACKUser_OnModeControllerDirective(int32_t correlationId, uint32_t instance, bool isDelta, int32_t value)
{
#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling Mode directive: %"PRIu32" isDelta: %u value: %d",
        instance,
        isDelta,
        value);
#endif

    switch (instance)
    {
    case MODE_INSTANCE_TIMER:
        ProcessSetTimerModeDirective(correlationId, isDelta, value);
        break;
    default:
        ACK_DEBUG_PRINT_E("Mode controller - not supported instance id: %u", instance);
        break;
    }
}

void ACKUser_OnToggleControllerDirective(int32_t correlationId, uint32_t instance, bool state)
{
#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling Toggle directive: %"PRIu32" value: %d",
        instance,
        state);
#endif

    switch (instance)
    {
    case MODE_INSTANCE_BLINKING:
        ProcessSetBlinkDirective(correlationId, state);
        break;
    default:
        ACK_DEBUG_PRINT_E("Mode controller - not supported instance id: %u", instance);
        break;
    }
}

void ProcessSetTimerModeDirective(int32_t correlationId, bool isDelta, int32_t value)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;

    if (isDelta)
    {
        if ((g_timerMode + value > TIMER_MODE_60_MIN) || (g_timerMode + value < TIMER_MODE_NONE))
        {
            // The user tries to get value out of supported range.
            // Complete directive with an error.
            ACK_CompleteDirectiveWithOutOfRangeError(
                correlationId,
                TIMER_MODE_NONE,
                TIMER_MODE_60_MIN,
                NULL);

            return;
        }

        g_timerMode += value;
        changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_TIMER_MODE);
    }
    else
    {
        if (g_timerMode != value)
        {
            g_timerMode = value;
            changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_TIMER_MODE);
        }
    }

    if (changedPropertiesBits)
    {
        // If power was off, turn it on.
        changedPropertiesBits |= SetLightToOnIfOff();

        // Set timer countdown
        switch (g_timerMode)
        {
        case TIMER_MODE_5_MIN:
            g_timerModeCountdown = 300000;
            break;
        case TIMER_MODE_10_MIN:
            g_timerModeCountdown = 600000;
            break;
        case TIMER_MODE_60_MIN:
            g_timerModeCountdown = 3600000;
            break;
        default:
            g_timerModeCountdown = 0;
            break;
        }
    }

    ACK_CompleteDirectiveWithSuccess(
        correlationId,
        c_lightPropertiesBits,
        changedPropertiesBits);
}

void ProcessSetBlinkDirective(int32_t correlationId, bool value)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;

    if (g_blinkingMode != value)
    {
        g_blinkingMode = value;
        changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_TOGGLE_MODE);
    }

    if (changedPropertiesBits)
    {
        // If power was off, turn it on.
        changedPropertiesBits |= SetLightToOnIfOff();
        g_blinkingPower = true;
    }

    ACK_CompleteDirectiveWithSuccess(
        correlationId,
        c_lightPropertiesBits,
        changedPropertiesBits);
}

// Returns milliseconds since the last loop.
// The returned value is used to decrease the countdowns.
uint32_t TimerGap(void)
{
    return ACKPlatform_TickCount() - g_previousTick;
}

// Shutoff timer will shut the device off, when timer is elapsed.
void CheckShutoffTimer()
{
    ACKPropertiesBits_t changedPropertiesBits = 0;

    if (g_timerMode == TIMER_MODE_NONE)
    {
        // Timer is off.
        return;
    }

    if (g_timerModeCountdown <= 0)
    {
        // Timer elapsed.
        g_timerModeCountdown = 0;
        g_timerMode = TIMER_MODE_NONE;
        g_power = false;
        changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_TIMER_MODE);
        changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_POWER_STATE);
        changedPropertiesBits |= ResetBlinkingMode();
        Hardware_TurnLightOff();

        ACK_SendChangeReport(
            v3avs_capabilities_V3Alexa_Cause_CauseType_RULE_TRIGGER,
            c_lightPropertiesBits,
            changedPropertiesBits);
    }
    else
    {
        g_timerModeCountdown -= TimerGap();
    }
}

// Blinking timer powers the LED on/off each time the countdown is reached.
// Once reached, countdown is reset.
void CheckBlinkingMode()
{
    if (g_blinkingMode == SPEED_MODE_NONE)
    {
        // Blinking mode is off.
        return;
    }

    if (g_blinkingCountdown <= 0)
    {
        if (g_blinkingPower)
        {
            Hardware_TurnLightOff();
            g_blinkingPower = false;
        }
        else
        {
            Hardware_TurnLightOn();
            g_blinkingPower = true;
        }

        switch (g_blinkingMode)
        {
        case SPEED_MODE_TOGGLE:
            g_blinkingCountdown = 1500;
            break;
        default:
            if (g_power)
            {
                Hardware_TurnLightOn();
            }

            g_blinkingCountdown = 0;
            g_blinkingMode = SPEED_MODE_NONE;
            break;
        }
    }
    else
    {
        g_blinkingCountdown -= TimerGap();
    }
}

// State-report callback.
void ACKUser_OnReportStateDirective(int32_t correlationId)
{
    // This sends all properties demonstrated in this application.
    ACK_CompleteStateReportWithSuccess(correlationId);
}

bool AddPowerProperty(uint32_t propertyOrdinal, unsigned propertyFlags)
{
    // Populate metadata about the property.
    // Leave the time-of-sample-field 0 to cause the current time to be sent.
    // Set the error margin to 10 milliseconds for illustrative purposes.
    ACKStateCommon_t common = { 0, 10, propertyFlags };
    ACKError_t error;

    error = ACK_AddPowerControllerProperty(&common, g_power);

    if (ACK_NO_ERROR != error)
    {
        ACK_DEBUG_PRINT_E("Error %u adding power property to event", error);
        return false;
    }

    return true;
}

bool AddLightBrightnessProperty(uint32_t propertyOrdinal, unsigned propertyFlags)
{
    // Populate metadata about the property.
    // Leave the time-of-sample-field 0 to cause the current time to be sent.
    // Set the error margin to 10 milliseconds for illustrative purposes.
    ACKStateCommon_t common = { 0, 10, propertyFlags };
    ACKError_t error;

    error = ACK_AddBrightnessControllerProperty(&common, g_brightness);

    if (ACK_NO_ERROR != error)
    {
        ACK_DEBUG_PRINT_E("Error %u adding brightness property to event", error);
        return false;
    }

    return true;
}

bool AddLightTimerProperty(uint32_t propertyOrdinal, unsigned propertyFlags)
{
    // Populate metadata about the property.
    // Leave the time-of-sample-field 0 to cause the current time to be sent.
    // Set the error margin to 10 milliseconds for illustrative purposes.
    ACKStateCommon_t common = { 0, 10, propertyFlags };
    ACKError_t error;

    error = ACK_AddModeControllerProperty(MODE_INSTANCE_TIMER, &common, g_timerMode);

    if (ACK_NO_ERROR != error)
    {
        ACK_DEBUG_PRINT_E("Error %u adding timer mode property to event", error);
        return false;
    }

    return true;
}

bool AddLightToggleProperty(uint32_t propertyOrdinal, unsigned propertyFlags)
{
    // Populate metadata about the property.
    // Leave the time-of-sample-field 0 to cause the current time to be sent.
    // Set the error margin to 10 milliseconds for illustrative purposes.
    ACKStateCommon_t common = { 0, 10, propertyFlags };
    ACKError_t error;

    error = ACK_AddToggleControllerProperty(MODE_INSTANCE_BLINKING, &common, g_blinkingMode);

    if (ACK_NO_ERROR != error)
    {
        ACK_DEBUG_PRINT_E("Error %u adding blinking mode property to event", error);
        return false;
    }

    return true;
}

static int QueryIdxInColTempMapTbl(E_COLORTEMPERATURE eColTemp)
{
    int idx = 0;
    while (idx < i32MaxColTempMapTblSize)
    {
        if (eColTemp == BR30_ColTempMapTbl[idx].m_eColTemp)
            return idx;
        idx++;
    }
    return -1;
}

bool AddLightColorTemperatureProperty(uint32_t propertyOrdinal, unsigned propertyFlags)
{
    // Populate metadata about the property.
    // Leave the time-of-sample-field 0 to cause the current time to be sent.
    // Set the error margin to 10 milliseconds for illustrative purposes.
    ACKStateCommon_t common = { 0, 10, propertyFlags };
    ACKError_t error;

    error = ACK_AddColorTemperatureControllerProperty(&common, g_colortemperature);

    if (ACK_NO_ERROR != error)
    {
        ACK_DEBUG_PRINT_E("Error %u adding color temperature property to event", error);
        return false;
    }

    return true;
}

void ProcessColorTemperatureEvent(int32_t correlationId, int isbChange, int32_t i32NewCTempValue)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;

    if (isbChange)
    {
        g_colortemperature = i32NewCTempValue;
        changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_COLORTEMPERATURE_MODE);
    }

    if (changedPropertiesBits)
    {
        if (!g_power)
        {
            g_power = true;
            changedPropertiesBits |= ACK_PROPERTY_BIT(PROPERTY_POWER_STATE);
        }
        Hardware_TurnLightOn();
    }

    ACK_CompleteDirectiveWithSuccess(
        correlationId,
        c_lightPropertiesBits,
        changedPropertiesBits);
}

void ACKUser_OnColorTemperatureControllerIncreaseTemperatureDirective(int32_t correlationId)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;
    int32_t idx = QueryIdxInColTempMapTbl(g_colortemperature);

#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling Increase color temperature directive: %d ",
        correlationId);
#endif

    ProcessColorTemperatureEvent(correlationId,
                                 g_colortemperature != eColTemp_MAX,
                                 (idx < (i32MaxColTempMapTblSize - 1)) ? BR30_ColTempMapTbl[idx + 1].m_eColTemp : eColTemp_MAX);
}

void ACKUser_OnColorTemperatureControllerDecreaseTemperatureDirective(int32_t correlationId)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;
    int32_t idx = QueryIdxInColTempMapTbl(g_colortemperature);

#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling Decrease color temperature directive: %d ",
        correlationId);
#endif

    ProcessColorTemperatureEvent(correlationId,
                                 g_colortemperature != eColTemp_MIN,
                                 (idx > 0) ? BR30_ColTempMapTbl[idx - 1].m_eColTemp : eColTemp_MIN);
}

void ACKUser_OnColorTemperatureControllerSetTemperatureDirective(int32_t correlationId, uint32_t valueInKelvin)
{
    ACKPropertiesBits_t changedPropertiesBits = 0;

#ifdef ACK_LOGGING
    ACK_WriteLogFormatted(
        acp_log_level_info,
        LOG_COMPONENT_NAME,
        "Handling Set color temperature directive: %d, valueInKelvin: %d ",
        correlationId, valueInKelvin);
#endif

    switch (valueInKelvin)
    {
    case eColTemp_2200K:
    case eColTemp_2700K:
    case eColTemp_4000K:
    case eColTemp_5500K:
    case eColTemp_7000K:
        ProcessColorTemperatureEvent(correlationId,
                                     g_colortemperature != valueInKelvin,
                                     valueInKelvin);
        return;
    default:
        break;
    }

    ACK_CompleteDirectiveWithSuccess(
        correlationId,
        c_lightPropertiesBits,
        changedPropertiesBits);
}

// Turns light on.
// This function is hardware specific. In the current implementation it assumes PWM pin
// with 0 as off and 255 as maximum value.
void Hardware_TurnLightOn()
{
    uint8_t value_warm, value_cold;
    int32_t idx = QueryIdxInColTempMapTbl(g_colortemperature);

    if (idx < 0)
        return;

    value_warm = (uint8_t)(255 * (float)BR30_ColTempMapTbl[idx].m_u8DutyCycle_Warm * g_brightness / 10000);
    value_cold = (uint8_t)(255 * (float)BR30_ColTempMapTbl[idx].m_u8DutyCycle_Cold * g_brightness / 10000);

    //ACK_DEBUG_PRINT_I
    printf("%dkv->(W:%d%%, C:%d%%)#%d%% -> (W:%d%%, C:%d%%) -> (W:%d, C:%d)",
           BR30_ColTempMapTbl[idx].m_eColTemp,
           BR30_ColTempMapTbl[idx].m_u8DutyCycle_Warm,
           BR30_ColTempMapTbl[idx].m_u8DutyCycle_Cold,
           g_brightness,
           (uint16_t)((float)BR30_ColTempMapTbl[idx].m_u8DutyCycle_Warm * g_brightness / 100),
           (uint16_t)((float)BR30_ColTempMapTbl[idx].m_u8DutyCycle_Cold * g_brightness / 100),
           value_warm, value_cold);

    /* 0~255    */
    ACKPlatform_SetDigitalPinPWMLevel(ACK_HW_PIN_PWM_WARM, value_warm);
    ACKPlatform_SetDigitalPinPWMLevel(ACK_HW_PIN_PWM_COLD, value_cold);
}

// Turns light off.
// This function is hardware specific. In the current implementation it assumes PWM pin
// with 0 as off and 255 as maximum value.
void Hardware_TurnLightOff()
{
    ACKPlatform_SetDigitalPinPWMLevel(ACK_HW_PIN_PWM_WARM, 0);
    ACKPlatform_SetDigitalPinPWMLevel(ACK_HW_PIN_PWM_COLD, 0);
}