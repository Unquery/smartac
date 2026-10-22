#include "AcControl.h"

#include <cmath>
#include <IRac.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

namespace
{

    constexpr uint16_t IR_TX_PIN = 6;

    IRac ac(IR_TX_PIN);
    bool initialized = false;

}

namespace AcControl
{

    void begin()
    {
        IRsend sender(IR_TX_PIN);
        sender.begin();
        initialized = true;
    }

    SendResult send(const AcProfile &profile, const AcSettings &settings)
    {
        if (!initialized)
            return SendResult::NotInitialized;

        // Enable profiles only once their settings can be validated explicitly.
        if (profile.protocol != AcProtocol::Fujitsu ||
            profile.model != AcModel::FujitsuArrah2e)
        {
            return SendResult::UnsupportedProfile;
        }

        if (!std::isfinite(settings.temperatureC) ||
            settings.temperatureC < kFujitsuAcMinTemp ||
            settings.temperatureC > kFujitsuAcMaxTemp ||
            std::floor(settings.temperatureC) != settings.temperatureC)
        {
            return SendResult::InvalidTemperature;
        }

        stdAc::state_t desired{};
        desired.protocol = decode_type_t::FUJITSU_AC;
        desired.model = ARRAH2E;
        desired.power = settings.power;
        desired.celsius = true;
        desired.degrees = settings.temperatureC;

        switch (settings.mode)
        {
        case AcMode::Auto:
            desired.mode = stdAc::opmode_t::kAuto;
            break;
        case AcMode::Cool:
            desired.mode = stdAc::opmode_t::kCool;
            break;
        case AcMode::Heat:
            desired.mode = stdAc::opmode_t::kHeat;
            break;
        case AcMode::Dry:
            desired.mode = stdAc::opmode_t::kDry;
            break;
        case AcMode::Fan:
            desired.mode = stdAc::opmode_t::kFan;
            break;
        default:
            return SendResult::UnsupportedMode;
        }

        switch (settings.fan)
        {
        case AcFanSpeed::Auto:
            desired.fanspeed = stdAc::fanspeed_t::kAuto;
            break;
        case AcFanSpeed::Quiet:
            desired.fanspeed = stdAc::fanspeed_t::kMin;
            break;
        case AcFanSpeed::Low:
            desired.fanspeed = stdAc::fanspeed_t::kLow;
            break;
        case AcFanSpeed::Medium:
            desired.fanspeed = stdAc::fanspeed_t::kMedium;
            break;
        case AcFanSpeed::High:
            desired.fanspeed = stdAc::fanspeed_t::kMax;
            break;
        default:
            return SendResult::UnsupportedFanSpeed;
        }

        switch (settings.verticalSwing)
        {
        case AcSwing::Off:
            desired.swingv = stdAc::swingv_t::kOff;
            break;
        case AcSwing::Auto:
            desired.swingv = stdAc::swingv_t::kAuto;
            break;
        default:
            return SendResult::UnsupportedSwing;
        }

        switch (settings.horizontalSwing)
        {
        case AcSwing::Off:
            desired.swingh = stdAc::swingh_t::kOff;
            break;
        case AcSwing::Auto:
            desired.swingh = stdAc::swingh_t::kAuto;
            break;
        default:
            return SendResult::UnsupportedSwing;
        }

        return ac.sendAc(desired) ? SendResult::Sent : SendResult::UnsupportedProfile;
    }

}
