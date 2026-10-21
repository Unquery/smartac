#include <Arduino.h>

#include "AcControl.h"

void setup() {
    Serial.begin(115200);

    AcControl::begin();
    AcControl::sendOff();
}

void loop() {
}
