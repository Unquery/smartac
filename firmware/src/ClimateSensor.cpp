#include "ClimateSensor.h"
#include "Adafruit_SHT4x.h"

namespace
{
    Adafruit_SHT4x sht4 = Adafruit_SHT4x();
    bool initialized = false;
}

namespace ClimateSensor
{

    bool begin()
    {
        initialized = sht4.begin();
        if (!initialized)
        {
            return false;
        }
        sht4.setPrecision(SHT4X_MED_PRECISION);
        sht4.setHeater(SHT4X_NO_HEATER);
        return true;
    }

    bool read(Reading &reading)
    {
        if (!initialized)
        {
            return false;
        }

        sensors_event_t humidity, temp;

        if (!sht4.getEvent(&humidity, &temp))
        {
            return false;
        }

        reading.humidityPercent = humidity.relative_humidity;
        reading.temperatureC = temp.temperature;
        return true;
    }
}
