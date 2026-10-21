#pragma once

namespace AcControl {

enum class AcProtocol {
    Unknown,
    Fujitsu,
};

// Compatible remote variants, not exact indoor-unit models.
enum class AcModel {
    Unknown,
    FujitsuArrah2e,
};

struct AcProfile {
    AcProtocol protocol;
    AcModel model;
};

enum class AcMode {
    Unknown,
    Auto,
    Cool,
    Heat,
    Dry,
    Fan,
};

enum class AcFanSpeed {
    Unknown,
    Auto,
    Quiet,
    Low,
    Medium,
    High,
};

enum class AcSwing {
    Unknown,
    Off,
    Auto,
};

// Supply every setting on each call. No previous request is retained.
struct AcSettings {
    bool power;
    AcMode mode;
    float temperatureC;
    AcFanSpeed fan;
    AcSwing verticalSwing;
    AcSwing horizontalSwing;
};

enum class SendResult {
    Sent,
    NotInitialized,
    UnsupportedProfile,
    InvalidTemperature,
    UnsupportedMode,
    UnsupportedFanSpeed,
    UnsupportedSwing,
};

// Initialize IR control without transmitting a command.
void begin();

// Rejected requests transmit nothing. Sent does not confirm physical AC state.
SendResult send(const AcProfile& profile, const AcSettings& settings);

}
