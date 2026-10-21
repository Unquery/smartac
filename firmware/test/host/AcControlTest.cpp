#include "AcControl.h"

#include <IRac.h>
#include <cassert>
#include <iostream>
#include <limits>

namespace {

unsigned initializations = 0;
unsigned sendAttempts = 0;
uint16_t transmitterPin = 0;
bool acceptTransmission = true;
stdAc::state_t lastRequest{};

using namespace AcControl;

const AcProfile profile{AcProtocol::Fujitsu, AcModel::FujitsuArrah2e};
const AcSettings startup{false, AcMode::Cool, 24.0f, AcFanSpeed::High,
                         AcSwing::Off, AcSwing::Off};

void expectRejected(const AcProfile& target, const AcSettings& settings,
                    SendResult expected) {
    const auto before = sendAttempts;
    assert(send(target, settings) == expected);
    assert(sendAttempts == before);
}

void testRejectedRequests() {
    expectRejected({AcProtocol::Unknown, AcModel::FujitsuArrah2e}, startup,
                   SendResult::UnsupportedProfile);
    expectRejected({AcProtocol::Fujitsu, AcModel::Unknown}, startup,
                   SendResult::UnsupportedProfile);
    expectRejected({static_cast<AcProtocol>(99), AcModel::FujitsuArrah2e}, startup,
                   SendResult::UnsupportedProfile);
    expectRejected({AcProtocol::Fujitsu, static_cast<AcModel>(99)}, startup,
                   SendResult::UnsupportedProfile);

    const float invalidTemperatures[]{
        15.0f, 31.0f, 24.5f,
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
    };
    for (const auto temperature : invalidTemperatures) {
        auto settings = startup;
        settings.temperatureC = temperature;
        expectRejected(profile, settings, SendResult::InvalidTemperature);
    }

    auto settings = startup;
    settings.mode = static_cast<AcMode>(99);
    expectRejected(profile, settings, SendResult::UnsupportedMode);
    settings.mode = AcMode::Unknown;
    expectRejected(profile, settings, SendResult::UnsupportedMode);

    settings = startup;
    settings.fan = static_cast<AcFanSpeed>(99);
    expectRejected(profile, settings, SendResult::UnsupportedFanSpeed);
    settings.fan = AcFanSpeed::Unknown;
    expectRejected(profile, settings, SendResult::UnsupportedFanSpeed);

    settings = startup;
    settings.verticalSwing = static_cast<AcSwing>(99);
    expectRejected(profile, settings, SendResult::UnsupportedSwing);
    settings.verticalSwing = AcSwing::Unknown;
    expectRejected(profile, settings, SendResult::UnsupportedSwing);
    settings = startup;
    settings.horizontalSwing = static_cast<AcSwing>(99);
    expectRejected(profile, settings, SendResult::UnsupportedSwing);
    settings.horizontalSwing = AcSwing::Unknown;
    expectRejected(profile, settings, SendResult::UnsupportedSwing);

    expectRejected({}, {}, SendResult::UnsupportedProfile);
    expectRejected(profile, {}, SendResult::InvalidTemperature);
}

void testSupportedSettings() {
    struct ModeCase { AcMode requested; stdAc::opmode_t encoded; };
    const ModeCase modes[]{
        {AcMode::Auto, stdAc::opmode_t::kAuto},
        {AcMode::Cool, stdAc::opmode_t::kCool},
        {AcMode::Heat, stdAc::opmode_t::kHeat},
        {AcMode::Dry, stdAc::opmode_t::kDry},
        {AcMode::Fan, stdAc::opmode_t::kFan},
    };
    for (const auto& mode : modes) {
        auto settings = startup;
        settings.power = true;
        settings.mode = mode.requested;
        assert(send(profile, settings) == SendResult::Sent);
        assert(lastRequest.mode == mode.encoded);
    }

    struct FanCase { AcFanSpeed requested; stdAc::fanspeed_t encoded; };
    const FanCase fans[]{
        {AcFanSpeed::Auto, stdAc::fanspeed_t::kAuto},
        {AcFanSpeed::Quiet, stdAc::fanspeed_t::kMin},
        {AcFanSpeed::Low, stdAc::fanspeed_t::kLow},
        {AcFanSpeed::Medium, stdAc::fanspeed_t::kMedium},
        {AcFanSpeed::High, stdAc::fanspeed_t::kMax},
    };
    for (const auto& fan : fans) {
        auto settings = startup;
        settings.power = true;
        settings.fan = fan.requested;
        assert(send(profile, settings) == SendResult::Sent);
        assert(lastRequest.fanspeed == fan.encoded);
    }

    for (const auto temperature : {16.0f, 30.0f}) {
        auto settings = startup;
        settings.power = true;
        settings.temperatureC = temperature;
        assert(send(profile, settings) == SendResult::Sent);
        assert(lastRequest.degrees == temperature);
    }

    for (const auto vertical : {AcSwing::Off, AcSwing::Auto}) {
        for (const auto horizontal : {AcSwing::Off, AcSwing::Auto}) {
            auto settings = startup;
            settings.power = true;
            settings.verticalSwing = vertical;
            settings.horizontalSwing = horizontal;
            assert(send(profile, settings) == SendResult::Sent);
            assert(lastRequest.swingv == (vertical == AcSwing::Off
                ? stdAc::swingv_t::kOff : stdAc::swingv_t::kAuto));
            assert(lastRequest.swingh == (horizontal == AcSwing::Off
                ? stdAc::swingh_t::kOff : stdAc::swingh_t::kAuto));
        }
    }
}

void testIndependentRequests() {
    const AcSettings first{true, AcMode::Heat, 30.0f, AcFanSpeed::Quiet,
                           AcSwing::Auto, AcSwing::Auto};
    const AcSettings second{true, AcMode::Cool, 16.0f, AcFanSpeed::High,
                            AcSwing::Off, AcSwing::Off};
    assert(send(profile, first) == SendResult::Sent);
    assert(send(profile, second) == SendResult::Sent);
    assert(lastRequest.protocol == decode_type_t::FUJITSU_AC);
    assert(lastRequest.model == ARRAH2E);
    assert(lastRequest.power);
    assert(lastRequest.mode == stdAc::opmode_t::kCool);
    assert(lastRequest.celsius);
    assert(lastRequest.degrees == 16.0f);
    assert(lastRequest.fanspeed == stdAc::fanspeed_t::kMax);
    assert(lastRequest.swingv == stdAc::swingv_t::kOff);
    assert(lastRequest.swingh == stdAc::swingh_t::kOff);
    assert(!lastRequest.quiet && !lastRequest.turbo && !lastRequest.econo);
    assert(!lastRequest.light && !lastRequest.filter && !lastRequest.clean);
    assert(!lastRequest.beep && !lastRequest.iFeel);
    assert(lastRequest.sleep == -1 && lastRequest.clock == -1);
    assert(send(profile, startup) == SendResult::Sent);
    assert(!lastRequest.power);
}

}

// Link-time stand-ins for hardware-facing methods. Use the real library types.
IRac::IRac(uint16_t pin, bool, bool) {
    transmitterPin = pin;
}

bool IRac::sendAc(const stdAc::state_t desired, const stdAc::state_t* previous) {
    assert(previous == nullptr);
    ++sendAttempts;
    lastRequest = desired;
    return acceptTransmission;
}

IRsend::IRsend(uint16_t pin, bool, bool) {
    assert(pin == transmitterPin);
}

void IRsend::begin() {
    ++initializations;
}

int main() {
    expectRejected(profile, startup, SendResult::NotInitialized);
    assert(initializations == 0);
    assert(transmitterPin == 6);
    begin();
    assert(initializations == 1);
    assert(sendAttempts == 0);

    testRejectedRequests();
    testSupportedSettings();
    testIndependentRequests();

    const auto before = sendAttempts;
    acceptTransmission = false;
    assert(send(profile, startup) == SendResult::UnsupportedProfile);
    assert(sendAttempts == before + 1);

    std::cout << "AcControl host tests passed\n";
}
