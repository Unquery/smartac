#pragma once

namespace ClimateSensor
{

    struct Reading
    {
        float temperatureC;
        float humidityPercent;
    };

    // The caller enables peripheral power and initializes I2C first.
    bool begin();

    // Returns false before initialization or on read failure; output is unchanged.
    bool read(Reading &reading);

}
