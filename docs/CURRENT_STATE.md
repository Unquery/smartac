# SmartAC Current State

## Repository

The project is organized as a monorepo.

Current structure includes:

- `firmware/` — existing PlatformIO ESP32-S3 firmware project.
- `docs/` — project context and persistent documentation.

Android, backend, web, and infrastructure modules have not been created yet.

## Firmware status

The ESP32-S3 firmware project builds with PlatformIO using the Arduino framework.

Confirmed working subsystems:

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

Known project dependencies include:

- Adafruit SHT4x Library
- IRremoteESP8266

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

Before adding network/backend functionality, inspect and cleanly organize the current firmware code while preserving the confirmed working hardware behavior.
