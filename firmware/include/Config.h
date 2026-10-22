#pragma once

#include <stdint.h>

namespace Config
{
    constexpr uint8_t PERIPHERAL_POWER_PIN = 4;
    constexpr uint16_t IR_TX_PIN = 6;
    constexpr uint8_t I2C_SDA_PIN = 8;
    constexpr uint8_t I2C_SCL_PIN = 9;

    constexpr uint32_t SERIAL_BAUD_RATE = 115200;
    constexpr uint32_t STARTUP_DELAY_MS = 5000;
    constexpr uint32_t CLIMATE_READ_INTERVAL_MS = 5000;
}
