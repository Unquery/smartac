# SmartAC Decisions

This file records durable project decisions that should influence future implementation.

## 2026-10-21 — Monorepo structure

**Decision**

Use a single repository for the complete SmartAC system.

Planned top-level modules:

- `firmware/`
- `android/`
- `backend/`
- `web/`
- `infra/`
- `docs/`

**Reason**

The project is one integrated product and is intended as a portfolio system showing embedded, backend, Android, web, and infrastructure work together.

**Consequences**

Shared documentation and project memory live at repository root.
Module-specific instructions should be added only when a module genuinely needs them.

---

## 2026-10-21 — ESP32 firmware environment

**Decision**

Use ESP32-S3 with PlatformIO and the Arduino framework for the firmware.

**Reason**

The current hardware prototype already works with this toolchain.

**Consequences**

Existing working firmware should be preserved and evolved incrementally instead of being rewritten only to change frameworks.

---

## 2026-10-21 — IR protocol library

**Decision**

Use IRremoteESP8266 for IR transmission, reception, and air-conditioner protocol support.

**Reason**

The library has already been tested successfully with the current hardware and Fujitsu AC.

**Consequences**

Protocol handling should build on the library rather than implementing Fujitsu IR encoding from scratch unless a future requirement makes that necessary.

---

## 2026-10-21 — AC pairing strategy

**Decision**

Use an interactive compatibility-pairing flow instead of pretending that the exact AC model can be detected automatically from IR alone.

The device will try safe protocol/model candidates and ask the user whether the AC reacted.

**Reason**

Normal IR control is one-way, so the ESP32 cannot reliably determine the exact indoor-unit model or confirm physical reaction without additional feedback.

**Consequences**

The stored result represents a compatible remote protocol/model variant, not necessarily the exact AC indoor-unit model.

Pairing commands should be conservative and avoid unsafe or disruptive test behavior.

---

## 2026-10-21 — Backend to device communication

**Decision**

Use MQTT as the primary communication mechanism between backend services and ESP32 devices.

**Reason**

MQTT fits lightweight device messaging, state updates, commands, and eventual multi-device support.

**Consequences**

Topic structure, payload contracts, retained-state behavior, reconnect behavior, and device authentication must be designed explicitly before production use.

---

## 2026-10-21 — Backend stack

**Decision**

Use Kotlin or Java with Spring Boot for the backend.

**Reason**

This aligns with the project's portfolio goals and the preferred JVM backend stack.

**Consequences**

The final Kotlin-versus-Java choice may be made when backend implementation begins.
Do not introduce a second backend framework without a strong reason.

---

## 2026-10-21 — Primary database

**Decision**

Use PostgreSQL as the main backend database.

**Reason**

The project requires durable relational data for users, devices, rooms, schedules, measurements, and history.

**Consequences**

The concrete schema should be derived from implemented use cases rather than designed prematurely in excessive detail.

---

## 2026-10-21 — Client authentication

**Decision**

Plan JWT-based authentication for normal client-to-backend authentication.

**Reason**

It fits the planned Android/web clients and Spring backend.

**Consequences**

ESP32 device authentication must be designed separately rather than reusing normal user JWT behavior blindly.

---

## 2026-10-21 — Realtime UI updates

**Decision**

Use WebSocket-style realtime communication between backend and user-facing clients where realtime synchronization is useful.

**Reason**

The Android and web clients should be able to receive device/state updates without constant polling.

**Consequences**

Realtime transport should complement, not replace, normal HTTP APIs where request/response behavior is simpler.

---

## 2026-10-21 — Development strategy

**Decision**

Build the project incrementally around a minimal end-to-end working path before adding advanced architecture.

**Reason**

The project spans embedded, networking, backend, mobile, web, persistence, and infrastructure; validating integration early reduces risk.

**Consequences**

Prefer a working vertical slice over implementing every planned subsystem independently before integration.
