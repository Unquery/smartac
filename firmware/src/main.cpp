#include <Arduino.h>
#include "AcControl.h"
#include "ClimateSensor.h"
#include <Wire.h>

constexpr uint8_t PERIPHERAL_POWER_PIN = 4;

void setup()
{
    Serial.begin(115200);

    while (!Serial) delay(10);
    pinMode(PERIPHERAL_POWER_PIN, OUTPUT);
    digitalWrite(PERIPHERAL_POWER_PIN, LOW);
    delay(10);

    Wire.begin(8,9);

    if (!ClimateSensor::begin())
    {
        Serial.println("Couldn't find SHT4x");
        while (1) delay(1);
    }

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
}
