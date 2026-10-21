# SmartAC Project Context

## Purpose

SmartAC is a portfolio smart-home climate-control system.

The system allows a conventional air conditioner to be controlled through an ESP32-based infrared controller, with Android and web clients connected through a backend service.

The project is intended both as a usable system and as a portfolio project demonstrating Android, backend, embedded, networking, and deployment skills.

## High-level architecture

The system is planned as a monorepo with these main parts:

- `firmware/` — ESP32-S3 firmware.
- `android/` — Android application.
- `backend/` — backend service.
- `web/` — web client.
- `infra/` — deployment and local infrastructure.
- `docs/` — persistent project context and technical documentation.

## Embedded device

The controller is based on an ESP32-S3.

Main responsibilities:

- transmit IR commands to an air conditioner;
- receive IR signals for testing, learning, and diagnostics;
- measure temperature and humidity;
- connect to the backend over the network;
- support future autonomous/offline behavior where appropriate.

The firmware currently uses PlatformIO with the Arduino framework.

## Air-conditioner control

The system controls conventional air conditioners through infrared signals.

The first confirmed target is Fujitsu.

The firmware uses IRremoteESP8266 for IR protocol support.

A compatible Fujitsu protocol variant can be discovered through an interactive pairing flow:

1. the device sends a safe candidate command;
2. the user confirms whether the air conditioner reacted;
3. unsuccessful candidates are skipped;
4. the confirmed compatible protocol/model variant is stored.

This pairing identifies a compatible remote-control protocol variant, not necessarily the exact indoor-unit model.

## Android application

The Android client is planned in Kotlin with Jetpack Compose.

It will provide features such as:

- device and room control;
- current climate state;
- temperature settings;
- schedules;
- device setup and pairing;
- history and usage information.

## Backend

The backend is planned with Kotlin or Java using Spring Boot.

Primary responsibilities include:

- user authentication;
- device registration;
- room/device ownership;
- commands and device state;
- schedules;
- historical data;
- coordination between clients and ESP32 devices.

## Communication

MQTT is planned for communication between the backend and ESP32 devices.

WebSocket-based communication is planned for realtime updates between the backend and user interfaces where useful.

The exact topic structure and message contracts should be documented when they are implemented.

## Persistence

PostgreSQL is planned as the main backend database.

Persistent data may include:

- users;
- devices;
- rooms;
- schedules;
- measurements;
- command/state history.

The concrete schema is not yet fixed and should be derived from implemented requirements.

## Authentication

JWT-based authentication is planned for client-to-backend authentication.

Device authentication should be designed separately from normal user authentication.

## Infrastructure

Docker Compose is planned for local development and deployment of infrastructure components such as:

- backend;
- PostgreSQL;
- MQTT broker;
- web frontend where appropriate.

## Product capabilities

Planned major capabilities include:

- multiple devices;
- multiple rooms;
- remote AC control;
- temperature and humidity monitoring;
- schedules;
- realtime state updates;
- historical measurements and charts;
- usage/history tracking;
- interactive IR protocol pairing;
- future autonomous device behavior;
- future OTA firmware updates.

Not every planned capability is implemented yet.

## Development principles

The project should be developed incrementally.

Prefer a minimal end-to-end working path before adding advanced architecture.

Project-specific implementation decisions belong in `DECISIONS.md`.
Current progress belongs in `CURRENT_STATE.md`.
Hardware wiring and confirmed electrical details belong in `HARDWARE.md`.
Planned milestones belong in `ROADMAP.md`.
