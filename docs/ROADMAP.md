# SmartAC Roadmap

This roadmap tracks major implementation phases rather than every small task.

## Phase 1 — Stabilize firmware prototype

Goals:

- preserve the currently working hardware behavior;
- organize firmware code into clear responsibilities;
- represent AC state cleanly;
- send controlled Fujitsu commands through IRremoteESP8266;
- keep SHT41 and IR receiver support working;
- add a safe interactive AC compatibility-pairing flow.

Completion signal:

The firmware can reliably control the current Fujitsu AC and has a clean structure ready for networking.

## Phase 2 — Device connectivity

Goals:

- add Wi-Fi connectivity;
- define device identity and configuration;
- introduce MQTT connectivity;
- define command/state message contracts;
- implement reconnect behavior;
- define initial device authentication approach.

Completion signal:

The ESP32 can connect to an MQTT broker, receive a command, apply it to the AC, and publish relevant state.

## Phase 3 — Backend foundation

Goals:

- create the Spring Boot backend;
- choose Kotlin or Java for the implementation;
- define core domain entities;
- add PostgreSQL;
- implement device registration;
- implement command/state handling;
- connect the backend to MQTT.

Completion signal:

A backend API can manage a device and communicate with the ESP32 through MQTT.

## Phase 4 — Minimal Android client

Goals:

- create the Kotlin + Jetpack Compose Android app;
- connect it to the backend;
- display device/room information;
- control AC power, mode, temperature, and fan settings;
- show current temperature and humidity.

Completion signal:

The Android app can control the physical AC through the complete Android -> backend -> MQTT -> ESP32 -> IR path.

## Phase 5 — Web client

Goals:

- create the web application;
- reuse backend APIs and domain concepts;
- provide essential device and room controls;
- show live state.

Completion signal:

Both Android and web clients can control and observe the same device state.

## Phase 6 — Authentication and ownership

Goals:

- implement user authentication;
- use JWT for user-facing clients;
- model ownership of rooms and devices;
- design secure device authentication separately;
- enforce authorization in backend APIs.

Completion signal:

Users can authenticate and access only their own devices and rooms.

## Phase 7 — Realtime synchronization

Goals:

- add WebSocket-based realtime client updates where useful;
- synchronize device state between ESP32, backend, Android, and web;
- define authoritative state and conflict behavior.

Completion signal:

State changes propagate to connected clients without manual refresh or constant polling.

## Phase 8 — Schedules and automation

Goals:

- implement scheduled AC actions;
- persist schedules in PostgreSQL;
- define execution responsibility and failure behavior;
- support basic automation rules;
- consider which rules should also work locally on the ESP32.

Completion signal:

A user can create a schedule and the AC action occurs reliably at the intended time.

## Phase 9 — History and charts

Goals:

- persist temperature/humidity measurements;
- store useful state/command history;
- expose historical APIs;
- show charts in Android and web;
- define retention strategy if needed.

Completion signal:

Users can inspect useful historical climate and device data.

## Phase 10 — Multi-device and multi-room polish

Goals:

- support multiple ESP32 devices;
- support multiple rooms;
- improve provisioning and pairing;
- handle offline devices cleanly;
- improve error reporting and recovery.

Completion signal:

The architecture works as a small multi-room smart-home system rather than a single-device demo.

## Phase 11 — Infrastructure and deployment

Goals:

- create Docker Compose configuration;
- run backend, PostgreSQL, MQTT broker, and web dependencies consistently;
- define environment configuration;
- document local startup;
- prepare a deployable setup.

Completion signal:

The complete software stack can be started reproducibly from documented infrastructure configuration.

## Phase 12 — Advanced firmware capabilities

Possible later work:

- OTA firmware updates;
- low-power improvements;
- better battery architecture;
- local/offline automation;
- richer pairing feedback;
- diagnostics and telemetry.

These are intentionally later phases and should not block the minimal end-to-end system.

## Phase 13 — Portfolio polish

Goals:

- improve README and architecture documentation;
- include diagrams and screenshots;
- document hardware setup;
- document major engineering decisions;
- add targeted tests;
- clean repository structure;
- make setup reproducible for reviewers.

Completion signal:

A recruiter or developer can understand the problem, architecture, tradeoffs, and demonstrated skills without reading the entire codebase.
