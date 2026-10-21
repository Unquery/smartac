#include <Arduino.h>

#include "AcControl.h"

void setup() {
    Serial.begin(115200);

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

void loop() {
}
