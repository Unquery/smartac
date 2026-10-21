#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

constexpr uint16_t IR_TX_PIN = 6;

IRFujitsuAC ac(IR_TX_PIN);

void setup() {
    Serial.begin(115200);

    ac.begin();
    ac.stateReset();

    ac.setModel(ARRAH2E);
    ac.setSwing(kFujitsuAcSwingOff);

    ac.setCmd(kFujitsuAcCmdTurnOff);

    Serial.println(ac.toString());
    ac.send();
}

void loop() {
}