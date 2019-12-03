/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if MBED_CONF_NSAPI_PRESENT

#include "ONBOARD_QUECTEL_EC21.h"
#include <mbed.h>
#include <AT_CellularNetwork.h>

#include <mbed_trace.h>
#define TRACE_GROUP  "WIO_LTE"

static const PinName MODULE_PWR_PIN = PB_5;
static const PinName ANT_PWR_PIN = PB_12;
static const PinName ENABLE_VCCB_PIN = PB_10;
static const PinName RGB_LED_PWR_PIN = PA_8;

static const PinName PWR_KEY_PIN = PC_4;
static const PinName RESET_MODULE_PIN = PC_3;

static const PinName STATUS_PIN = PB_15;

static const PinName DTR_PIN = PA_1;

static const PinName WAKEUP_IN_PIN = PC_0;
static const PinName W_DISABLE_PIN = PC_2;
static const PinName AP_READY_PIN = PC_1;

static const PinName RGB_LED_PIN = PB_1;
static const PinName SD_POWR_PIN = PA_15;

CellularDevice *CellularDevice::get_target_default_instance()
{
    static UARTSerial serial(PA_2, PA_3, 115200);
    static ONBOARD_QUECTEL_EC21 device(&serial);
    return &device;
}

static const intptr_t CELLULAR_PROPERTIES[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,    // C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

ONBOARD_QUECTEL_EC21::ONBOARD_QUECTEL_EC21(FileHandle* fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(CELLULAR_PROPERTIES);

    gpio_t gpio;

    // Power supply
    gpio_init_out_ex(&gpio, MODULE_PWR_PIN, 0);

    // Turn on/off Pins
    gpio_init_out_ex(&gpio, PWR_KEY_PIN, 0);
    gpio_init_out_ex(&gpio, RESET_MODULE_PIN, 1);

    // Status Indication Pins
    gpio_init_in_ex(&gpio, STATUS_PIN, PullNone);

    // UART Interface
    gpio_init_out_ex(&gpio, DTR_PIN, 0);

    // GPIO Pins
    gpio_init_out_ex(&gpio, WAKEUP_IN_PIN, 0);
    gpio_init_out_ex(&gpio, W_DISABLE_PIN, 1);
}

nsapi_error_t ONBOARD_QUECTEL_EC21::hard_power_on()
{
    tr_info("hard_power_on()");

    gpio_t gpio;
    gpio_init_out_ex(&gpio, MODULE_PWR_PIN, 1);
    ThisThread::sleep_for(1000);

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_EC21::hard_power_off()
{
    tr_info("hard_power_off()");

    gpio_t gpio;
    gpio_init_out_ex(&gpio, MODULE_PWR_PIN, 0);
    ThisThread::sleep_for(1000);

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_EC21::soft_power_on()
{
    tr_info("soft_power_on()");

    gpio_t gpio;
    ThisThread::sleep_for(100);
    gpio_init_out_ex(&gpio, PWR_KEY_PIN, 1);
    ThisThread::sleep_for(200);
    gpio_init_out_ex(&gpio, PWR_KEY_PIN, 0);

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_EC21::soft_power_off()
{
    tr_info("soft_power_off()");

    return NSAPI_ERROR_OK;
}

#endif // MBED_CONF_NSAPI_PRESENT
