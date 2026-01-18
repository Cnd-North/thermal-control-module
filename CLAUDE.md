# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Repository:** https://github.com/Cnd-North/thermal-control-module
**License:** MIT

This is an ESP8266-based Thermocouple PID Temperature Controller (v2.0) designed for bioreactors and biochemical devices. The main production code is in `Thermocouple_PID_Controller/Thermocouple_PID_Controller.ino`.

## Build and Upload

**Arduino IDE Configuration:**
- Board: Generic ESP8266 Module (or specific NodeMCU)
- Upload Speed: 115200
- CPU Frequency: 80 MHz
- Flash Size: 4MB (FS:2MB OTA:~1019KB)
- Board Support URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`

**Required Libraries (Arduino Library Manager):**
- Adafruit GFX Library
- Adafruit SSD1306
- Adafruit MAX31855 library
- PID_v1_bc (Brett Beauregard)

**Serial Monitor:** 115200 baud for debug output

## Hardware Pin Configuration

**DO NOT CHANGE** - Physical board is wired to these pins:

| Component | Pin | GPIO |
|-----------|-----|------|
| MAX31855 DO | D6 | GPIO 12 |
| MAX31855 CS | D8 | GPIO 15 |
| MAX31855 CLK | D5 | GPIO 14 |
| SSR (PWM) | D3 | GPIO 0 |
| Encoder CLK | SCK | GPIO 14 (shared) |
| Encoder DT | SD2 | GPIO 9 |
| Encoder SW | SD3 | GPIO 10 |
| OLED SDA | D2 | GPIO 4 |
| OLED SCL | D1 | GPIO 5 |
| OLED Address | - | 0x3C |

## Architecture

The main sketch (`Thermocouple_PID_Controller.ino`) follows this structure:

1. **Pin Definitions** (lines 28-49): Hardware constants, DO NOT modify
2. **Global Objects** (lines 51-56): Display, thermocouple, PID instances
3. **PID Configuration** (lines 58-74): Tunable parameters (Kp=2.0, Ki=0.5, Kd=1.0)
4. **Encoder Variables** (lines 76-94): State tracking, debouncing, setpoint range (10-100°C)
5. **setup()** (lines 107-176): Sequential initialization with error handling
6. **loop()** (lines 182-231): Non-blocking main loop using millis() timing
7. **handleEncoder()** (lines 237-262): Rotation detection and setpoint adjustment
8. **handleButton()** (lines 268-298): Mode toggle with 200ms debounce
9. **Display Functions** (lines 304-361): updateDisplay() and displayError()

**Key Design Patterns:**
- Non-blocking loop using `millis() - lastTime` comparisons
- Dual-mode operation: LOCKED (PID active) vs ADJUSTING (SSR disabled for safety)
- SSR automatically disabled when adjusting setpoint
- PID output range: 0-255 for PWM control

## Directory Structure

- `Thermocouple_PID_Controller/` - Main production sketch
- `ChatThermo_Display_PID/`, `Encoder-Temp-Display/`, `Thermocouple_Display_PID/` - Development iterations
- `Setpoint_Encoder_test/`, `Pin_Check/` - Hardware test sketches
- `libraries/` - Local copies of dependencies
- `PID_TUNING_GUIDE.md`, `PID_PRESETS.md` - PID tuning documentation

## PID Tuning Quick Reference

| Symptom | Fix |
|---------|-----|
| Too slow to reach setpoint | Increase Kp |
| Oscillating around setpoint | Decrease Kp, increase Kd |
| Steady-state error | Increase Ki |

Presets for common applications are in `PID_PRESETS.md`.

## Contributing

Standard GitHub workflow:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Report bugs and feature requests via GitHub Issues.

## Project Info

- **Author:** ElectroBio Team (Cnd-North)
- **Website:** [electrobio.xyz](https://www.electrobio.xyz)
- **Support:** [Buy me a coffee](https://buymeacoffee.com/electrobio)

Hardware purchase links with affiliate support are in the README.md.
