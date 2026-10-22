# SmartAC Current State

## Repository

The project is organized as a monorepo.

Current structure includes:

- `firmware/` — existing PlatformIO ESP32-S3 firmware project.
- `docs/` — project context and persistent documentation.

Android, backend, web, and infrastructure modules have not been created yet.

## Firmware status

The ESP32-S3 firmware project builds with PlatformIO using the Arduino framework.

The current program initializes IR control and transmits one Fujitsu OFF request at startup, using `ARRAH2E` with swing disabled. The main loop is empty.

SHT41 access is encapsulated in `ClimateSensor`, exposing initialization and temperature/humidity readings without Adafruit types in its public interface. It uses medium precision with the heater disabled and leaves the output unchanged if a read fails or initialization has not succeeded. `main.cpp` enables peripheral power, initializes I2C, and prints a single startup reading. The user confirmed serial readings before this extraction on the switched peripheral rail, using SDA GPIO8 and SCL GPIO9; the extracted module still needs a hardware retest.

`AcControl` accepts an explicit `AcProfile` and complete `AcSettings` for each request. Its implementation uses IRremoteESP8266's common `IRac` interface on GPIO6. Initialization does not transmit a command; `main.cpp` explicitly requests the startup transmission.

Only the Fujitsu `ARRAH2E` profile is currently enabled. Requests support power, auto/cool/heat/dry/fan mode, whole-degree Celsius settings from 16 to 30, auto/quiet/low/medium/high fan, and off/auto swing on each axis. Invalid or unsupported requests are rejected before transmission. No previous request is used to fill omitted settings, and transmission does not establish observed physical AC state.

The common-interface implementation passes the ESP32-S3 build and host tests for validation and request translation. The user has physically verified ON and OFF commands through the `AcControl` / `IRac` transmission path with the Fujitsu `ARRAH2E` profile and swing OFF. Operation without an added startup delay has also been confirmed in the tested setup. Other supported settings have not all been physically verified.

Previously physically tested subsystems (not all present in the current program):

- SHT41 temperature and humidity sensor over I2C.
- CHQ1838D 38 kHz IR receiver.
- TSAL6400 IR transmitter driven through a 2N2222A transistor.
- AO3401A high-side power switch for peripheral 3.3 V power control.
- IR transmit/receive self-test using IRremoteESP8266.
- Fujitsu AC command transmission using IRremoteESP8266.

## Confirmed hardware behavior

The SHT41 has produced valid temperature and humidity readings.

The IR receiver has detected and decoded transmitted IR signals.

The ESP32 successfully transmitted a Fujitsu AC command that turned the target air conditioner on.

The confirmed working Fujitsu remote protocol/model variant for the current target is:

`ARRAH2E`

Swing should remain disabled during protocol testing unless intentionally required.

## Current firmware-related libraries

The current configured dependencies are IRremoteESP8266 and Adafruit SHT4x Library. The latter provides the restored SHT41 readings.

The exact current contents of `platformio.ini` remain the source of truth for firmware dependencies.

## Current development focus

The hardware prototype and basic IR control path are working.

The next meaningful milestone is to turn the existing experiments into a clean firmware structure that can:

- represent AC state;
- send controlled Fujitsu commands;
- support a safe pairing workflow;
- prepare for Wi-Fi and MQTT integration.

## Not implemented yet

The following major areas are still planned rather than implemented:

- production firmware architecture;
- persistent device configuration;
- Wi-Fi provisioning;
- MQTT communication;
- backend;
- PostgreSQL persistence;
- authentication;
- Android app;
- web app;
- Docker Compose infrastructure;
- realtime WebSocket updates;
- schedules;
- historical charts;
- multi-device / multi-room support;
- OTA update flow.

## Immediate next step

Continue Phase 1 with previously tested peripheral support and safe compatibility pairing before adding network/backend functionality.
