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

`AcControl` provides a brand-independent interface, with the Fujitsu library object, GPIO6 configuration, and transmission details kept inside its implementation. Initialization does not transmit a command; `main.cpp` explicitly requests the startup transmission.

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

The current configured dependency is IRremoteESP8266. The SHT41 was previously tested using Adafruit SHT4x Library, but sensor code and that dependency are absent from the current firmware.

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

Continue organizing firmware around the AC-control boundary, introduce explicit requested AC state, and restore previously tested peripheral support before adding network/backend functionality.
