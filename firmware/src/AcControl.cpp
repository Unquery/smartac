#include "AcControl.h"

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

namespace {

constexpr uint16_t IR_TX_PIN = 6;

IRFujitsuAC ac(IR_TX_PIN);

}

namespace AcControl {

void begin() {
    ac.begin();
    ac.stateReset();

    ac.setModel(ARRAH2E);
    ac.setSwing(kFujitsuAcSwingOff);
}

void sendOn() {
    ac.setCmd(kFujitsuAcCmdTurnOn);

    Serial.println(ac.toString());
    ac.send();
}

void sendOff() {
    ac.setCmd(kFujitsuAcCmdTurnOff);

    Serial.println(ac.toString());
    ac.send();
}

}
