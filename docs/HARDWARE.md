# SmartAC Hardware

## Controller

Main controller:

- ESP32-S3 DevKitC-1
- PlatformIO
- Arduino framework

## Confirmed peripherals

### Temperature and humidity

Sensor:

- Grove SHT41

Connections:

- VCC -> switched 3.3 V (`3V3_SW`)
- GND -> common GND
- SDA -> GPIO8
- SCL -> GPIO9

The sensor has been tested successfully and returns valid temperature and humidity readings.

### IR receiver

Receiver:

- CHQ1838D
- 38 kHz IR receiver

Connections:

- OUT -> GPIO5
- GND -> common GND
- VCC -> switched 3.3 V (`3V3_SW`)

Local decoupling:

- 100 nF ceramic capacitor between VCC and GND near the receiver

The receiver has been tested successfully with IRremoteESP8266.

Typical CHQ1838D pinout used during prototyping, viewed from the front lens with leads pointing down:

- left -> OUT
- middle -> GND
- right -> VCC

Always verify the exact component datasheet before rebuilding the circuit.

### IR transmitter

IR LED:

- TSAL6400
- 940 nm

Driver transistor:

- 2N2222A / PN2222A NPN transistor

Control pin:

- GPIO6

Current prototype wiring:

- +5 V -> 39 ohm resistor -> TSAL6400 anode
- TSAL6400 cathode -> transistor collector
- transistor emitter -> GND
- GPIO6 -> 470 ohm resistor -> transistor base
- transistor base -> 10 kohm resistor -> GND

Local bulk capacitor:

- 100 uF electrolytic between +5 V and GND near the IR transmitter stage
- positive terminal -> +5 V
- negative terminal -> GND

A 100 nF ceramic capacitor may also be used in parallel for high-frequency decoupling.

The transmitter has been tested successfully:

- the IR LED is visible flashing through a phone camera;
- the locally transmitted signal was received by the CHQ1838D;
- IRremoteESP8266 decoded the transmitted frame as `FUJITSU_AC`.

Important:

The physical pinout of 2N2222A / PN2222A varies between manufacturers.
Do not assume a generic TO-92 pin order; verify the exact transistor used.

## Peripheral power switching

High-side switch:

- AO3401A P-channel MOSFET

Purpose:

- disconnect selected 3.3 V peripherals when they are not needed;
- reduce power consumption during future low-power operation.

Connections:

- Source -> ESP32 3.3 V
- Drain -> switched rail `3V3_SW`
- Gate -> GPIO4 through 1 kohm
- Gate -> Source / 3.3 V through 100 kohm pull-up

Logic:

- GPIO4 LOW -> MOSFET ON -> `3V3_SW` powered
- GPIO4 input / high-impedance -> pull-up brings Gate near Source -> MOSFET OFF

The switched rail currently powers:

- SHT41
- CHQ1838D

The AO3401A switching circuit has been tested successfully with the sensor.

## Power concept

Prototype battery/power path discussed for the project:

- 18650 battery
- TP4056 charger module
- physical power switch
- boost converter
- approximately 5 V output
- ESP32 DevKit 5 V / VIN input
- IR transmitter 5 V rail

All grounds must be common.

Before connecting the boost converter to the ESP32, its output voltage must be verified with a multimeter.

The exact boost-converter model should be verified from the physical module before treating it as a confirmed project fact.

The current prototype power architecture is suitable for development but is not yet optimized for low-power battery operation.

## GPIO map

| GPIO | Function |
| --- | --- |
| GPIO4 | AO3401A peripheral power control |
| GPIO5 | CHQ1838D IR receiver input |
| GPIO6 | IR transmitter transistor control |
| GPIO8 | SHT41 SDA |
| GPIO9 | SHT41 SCL |

## Confirmed AC compatibility

Target brand:

- Fujitsu

Library:

- IRremoteESP8266

Confirmed compatible remote protocol/model variant:

- `ARRAH2E`

A command generated with this variant successfully turned on the target Fujitsu air conditioner.

Do not assume this identifies the exact indoor-unit model.

## Safety and verification rules

When changing hardware:

- verify supply voltage before connecting the ESP32;
- verify transistor and MOSFET pinouts from the exact part datasheet;
- test one subsystem at a time;
- keep grounds common;
- avoid changing IR protocol and electrical hardware simultaneously when debugging;
- treat unverified component/module names as assumptions until checked physically.
