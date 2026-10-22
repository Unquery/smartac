#include <Arduino.h>
#include "AcControl.h"
#include "ClimateSensor.h"
#include "Config.h"
#include <Wire.h>

namespace
{
    uint32_t lastClimateReadMs = 0;

    void printClimateReading()
    {
        ClimateSensor::Reading reading{};
        if (ClimateSensor::read(reading))
        {
            Serial.printf("Temperature: %.2f degrees C\n", reading.temperatureC);
            Serial.printf("Humidity: %.2f%% rH\n", reading.humidityPercent);
        }
        else
        {
            Serial.println("SHT4x read failed");
        }
    }
}

void setup()
{
    Serial.begin(Config::SERIAL_BAUD_RATE);

    while (!Serial) delay(10);
    pinMode(Config::PERIPHERAL_POWER_PIN, OUTPUT);
    digitalWrite(Config::PERIPHERAL_POWER_PIN, LOW);
    delay(Config::STARTUP_DELAY_MS);

    Wire.begin(Config::I2C_SDA_PIN, Config::I2C_SCL_PIN);

    if (!ClimateSensor::begin())
    {
        Serial.println("Couldn't find SHT4x");
        while (1) delay(1);
    }

    lastClimateReadMs = millis();
    printClimateReading();

    AcControl::begin();

    const AcControl::AcProfile profile{
        AcControl::AcProtocol::Fujitsu,
        AcControl::AcModel::FujitsuArrah2e,
    };

    // Preserve the prototype's startup OFF request and prior reset settings.
    const AcControl::AcSettings settings{
        false,
        AcControl::AcMode::Cool,
        24.0f,
        AcControl::AcFanSpeed::High,
        AcControl::AcSwing::Off,
        AcControl::AcSwing::Off,
    };

    const auto result = AcControl::send(profile, settings);
    if (result == AcControl::SendResult::Sent) {
        Serial.println("AC OFF request sent");
    } else {
        Serial.printf("AC startup request rejected (%u)\n", static_cast<unsigned>(result));
    }

}

void loop()
{
    const uint32_t now = millis();
    // Unsigned elapsed-time subtraction also works when millis() wraps around.
    if (now - lastClimateReadMs >= Config::CLIMATE_READ_INTERVAL_MS)
    {
        lastClimateReadMs = now;
        printClimateReading();
    }
}
