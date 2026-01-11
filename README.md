# Thermal Control Module

A modular, precision thermal control system designed for bioreactors and biochemical devices. Features PID-controlled Peltier plates with thermocouple sensing for accurate temperature regulation in bioprocessing applications.

![Version](https://img.shields.io/badge/version-2.0-blue.svg)
![Arduino](https://img.shields.io/badge/Arduino-ESP8266-00979D.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Application](https://img.shields.io/badge/Application-Bioreactor-green.svg)

## Overview

This thermal control module serves as a fundamental building block for environmental control systems in biotechnology applications including:

- 🧫 **Bioreactors** - Maintain optimal growth temperatures for cell cultures
- 🔬 **Lab Incubators** - Precise temperature regulation for experiments
- 🧬 **PCR Thermal Cyclers** - Custom thermocycling for molecular biology
- 🌡️ **Environmental Chambers** - Controlled conditions for biological studies
- 💉 **Fermentation Systems** - Temperature-controlled bacterial/yeast cultures

The modular design allows integration into larger control systems for comprehensive bioprocess monitoring and automation.

## Features

- **Precision Temperature Sensing** - MAX31855 thermocouple module with ±2°C accuracy
- **PID Control** - Smooth temperature regulation with tunable parameters
- **PWM Output** - Proportional control of Peltier plates via SSR (0-255 levels)
- **Interactive Display** - 128x64 OLED showing temp, setpoint, and power output
- **Rotary Encoder Control** - Easy setpoint adjustment with physical knob
- **Dual Mode Operation**:
  - **Locked Mode**: PID actively controlling temperature
  - **Adjust Mode**: Change setpoint, SSR disabled for safety
- **Safety Features**:
  - Automatic SSR shutdown in adjust mode
  - Temperature range limits (10-100°C configurable)
  - Sensor error detection and display
- **Modular Architecture** - Designed for integration into larger control systems

## Hardware Requirements

### Components

All components available on Amazon (affiliate links support this project at no extra cost to you):

| Component | Model/Type | Est. Price | Amazon Link |
|-----------|------------|------------|-------------|
| **Microcontroller** | ESP8266 NodeMCU | ~$8-12 | [Shop ESP8266](https://www.amazon.com/s?k=esp8266+nodemcu&tag=electrobio-20) |
| **Thermocouple Module** | MAX31855 Breakout | ~$10-15 | [Shop MAX31855](https://www.amazon.com/s?k=MAX31855+thermocouple+module&tag=electrobio-20) |
| **K-Type Thermocouple** | Probe with wire | ~$8-15 | [Shop K-Type Probe](https://www.amazon.com/s?k=k+type+thermocouple+probe&tag=electrobio-20) |
| **OLED Display** | SSD1306 128x64 I2C | ~$5-10 | [Shop OLED Display](https://www.amazon.com/s?k=ssd1306+128x64+oled+i2c&tag=electrobio-20) |
| **Rotary Encoder** | KY-040 Module | ~$5-8 | [Shop Rotary Encoder](https://www.amazon.com/s?k=ky-040+rotary+encoder&tag=electrobio-20) |
| **Solid State Relay** | 25A DC-DC SSR | ~$8-15 | [Shop SSR](https://www.amazon.com/s?k=25a+dc+dc+solid+state+relay&tag=electrobio-20) |
| **Peltier Plates** | TEC1-12706 (12V 6A) | ~$8-12 | [Shop Peltier](https://www.amazon.com/s?k=tec1-12706+peltier&tag=electrobio-20) |
| **Power Supply** | 12V 5A+ Adapter | ~$10-15 | [Shop 12V Power Supply](https://www.amazon.com/s?k=12v+5a+power+supply&tag=electrobio-20) |
| **Heatsink + Fan** | 40mm Aluminum | ~$10-15 | [Shop Heatsink](https://www.amazon.com/s?k=40mm+heatsink+fan+12v&tag=electrobio-20) |

**Total Cost:** ~$70-120 depending on components selected

**Optional Components:**
- [Breadboard & Jumper Wires](https://www.amazon.com/s?k=breadboard+jumper+wires&tag=electrobio-20) (~$10) - For prototyping
- [Project Enclosure](https://www.amazon.com/s?k=electronic+project+enclosure&tag=electrobio-20) (~$8-15) - To house electronics
- [Thermal Paste](https://www.amazon.com/s?k=thermal+paste&tag=electrobio-20) (~$5) - For Peltier to heatsink
- [Wire Kit](https://www.amazon.com/s?k=22awg+wire+kit&tag=electrobio-20) (~$12) - Various gauge wire

### Recommended Complete Kits

**Budget Build (~$70-90):**
- Generic ESP8266 NodeMCU
- Basic MAX31855 module with probe
- Standard SSD1306 display
- Economy SSR and Peltier
- Basic 12V power supply

**Pro Build (~$110-130):**
- Quality ESP8266 with USB-C
- Adafruit MAX31855 module
- High-quality K-type probe
- Industrial-grade SSR (Fotek or Crydom)
- TEC1-12706 with heatsink/fan combo
- Regulated 12V 10A power supply

*Affiliate links help support open-source biohacking projects at electrobio.xyz*

### Pin Connections

**⚠️ DO NOT CHANGE - Physical board is wired to these pins**

#### MAX31855 Thermocouple (Software SPI)
- `MAXDO` (Data Out) → GPIO 12 (D6)
- `MAXCS` (Chip Select) → GPIO 15 (D8)
- `MAXCLK` (Clock) → GPIO 14 (D5)
- `VCC` → 3.3V
- `GND` → GND

#### SSD1306 OLED Display (I2C)
- `SDA` → GPIO 4 (D2)
- `SCL` → GPIO 5 (D1)
- `VCC` → 3.3V
- `GND` → GND

#### Rotary Encoder
- `CLK` → SCK (GPIO 14) *Note: Shares with MAXCLK*
- `DT` → GPIO 9 (SD2)
- `SW` (Button) → GPIO 10 (SD3)
- `VCC` → 3.3V
- `GND` → GND

#### Solid State Relay
- `Control (+)` → GPIO 0 (D3) via 1kΩ resistor
- `Control (-)` → GND
- `Load` → Peltier plates (12V circuit)

See [PIN_CONFIGURATION.md](PIN_CONFIGURATION.md) for detailed wiring diagram.

## Software Dependencies

### Arduino Libraries (Install via Library Manager)

```
Adafruit GFX Library by Adafruit
Adafruit SSD1306 by Adafruit
Adafruit MAX31855 library by Adafruit
PID_v1_bc by Brett Beauregard
```

### Board Support

Install ESP8266 board support in Arduino IDE:
- **Boards Manager URL**: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- **Board**: Generic ESP8266 Module (or your specific board)

## Installation

1. **Clone this repository**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/thermocouple-pid-controller.git
   cd thermocouple-pid-controller
   ```

2. **Install Arduino libraries** (see dependencies above)

3. **Open sketch**:
   ```
   Thermocouple_PID_Controller/Thermocouple_PID_Controller.ino
   ```

4. **Configure Arduino IDE**:
   - Board: ESP8266 Generic Module (or your board)
   - Upload Speed: 115200
   - CPU Frequency: 80 MHz
   - Flash Size: 4MB (FS:2MB OTA:~1019KB)

5. **Upload** to your ESP8266

## Usage

### First Boot

1. Display shows "PID CTRL - Initializing..."
2. System checks thermocouple connection
3. Display shows "READY!" when initialization complete

### Normal Operation (Locked Mode)

```
MODE: LOCKED

25.3 C          <- Current temperature
Target: 25.0 C  <- Setpoint
Power: 45%      <- SSR output (0-100%)
```

- PID automatically maintains temperature at setpoint
- SSR modulates power to Peltier plates

### Adjusting Setpoint

1. **Press encoder button** to enter Adjust Mode
2. **Rotate encoder** to change setpoint (±0.5°C per click)
3. **Press button again** to lock in new setpoint

```
MODE: ADJUSTING

SET: 30.0C      <- New setpoint preview

Turn to adjust
Press to lock
```

**Note**: SSR is automatically disabled in Adjust Mode for safety.

### Serial Monitor Output

Connect at 115200 baud to see debug information:

```
=== Thermocouple PID Controller v2.0 ===
Initializing thermocouple...
Initial temperature: 24.8 C
PID initialized
SSR configured
Encoder configured
=== System Ready ===

Temp: 24.82 C | Setpoint: 25.00 C | Output: 115/255
Temp: 24.87 C | Setpoint: 25.00 C | Output: 98/255
...
```

## PID Tuning

### Default Parameters

```cpp
double kp = 2.0;   // Proportional gain
double ki = 0.5;   // Integral gain
double kd = 1.0;   // Derivative gain
```

These are conservative starting values. For optimal performance, tune for your specific hardware.

### Tuning Guide

See [PID_TUNING_GUIDE.md](PID_TUNING_GUIDE.md) for step-by-step instructions on:
- Ziegler-Nichols method
- Manual tuning process
- Interpreting temperature response curves
- Troubleshooting oscillations

### Common Adjustments

**Too slow to reach setpoint:**
- Increase `kp` (try doubling)
- Slightly increase `ki`

**Oscillating around setpoint:**
- Decrease `kp` (try halving)
- Increase `kd` to dampen oscillations

**Steady-state error (doesn't quite reach setpoint):**
- Increase `ki` in small increments (0.1 at a time)

## Project Structure

```
thermocouple-pid-controller/
├── Thermocouple_PID_Controller/
│   └── Thermocouple_PID_Controller.ino  # Main sketch
├── ChatThermo_Display_PID/               # Development version 1
├── Encoder-Temp-Display/                 # Development version 2
├── Setpoint_Encoder_test/                # Encoder test code
├── Thermocouple_Display_PID/             # Original version
├── Pin_Check/                            # Hardware diagnostic
├── README.md                             # This file
├── PIN_CONFIGURATION.md                  # Detailed wiring
├── PID_TUNING_GUIDE.md                   # Tuning instructions
└── .gitignore
```

## Version History

### v2.0 (2025-01-01) - Merged Best Features
- ✅ Combined best features from all development versions
- ✅ Restored PWM control for smooth temperature regulation
- ✅ Improved encoder handling (no negative value bugs)
- ✅ Clean display updates with dedicated functions
- ✅ Constrained setpoint range (10-100°C)
- ✅ Safety shutdown in adjust mode
- ✅ Comprehensive error handling

### v1.x (2024) - Development Versions
- Various experimental versions testing encoder, display, PID approaches
- See individual sketches for details

### v0.1 (Jul 2024) - Original
- Basic thermocouple + PID + potentiometer control
- AutoPID library
- WiFi/MQTT capability (commented out)

## Troubleshooting

### Display shows "Thermocouple Not Found!"
- Check MAX31855 wiring (especially MAXCS on GPIO 15)
- Verify 3.3V power to MAX31855
- Check SPI connections (CLK, DO, CS)

### Display shows "Sensor Error!"
- Thermocouple may be disconnected
- Check K-type thermocouple connections to MAX31855
- Verify thermocouple polarity (+ and -)

### Temperature oscillates wildly
- PID parameters need tuning (see tuning guide)
- Check SSR is properly switching Peltier plates
- Verify Peltier thermal coupling to target

### Encoder doesn't change setpoint
- Press button to enter Adjust Mode first
- Check encoder wiring (CLK, DT, SW pins)
- Verify encoder has pull-up resistors or use INPUT_PULLUP

### SSR not activating
- Check GPIO 0 (D3) connection to SSR control
- Verify SSR requires 3.3V logic (or use level shifter)
- Add 1kΩ resistor between GPIO and SSR control input
- Check SSR load circuit (Peltier + 12V supply)

## Safety Warnings

⚠️ **IMPORTANT SAFETY INFORMATION**

- **High Current**: Peltier plates can draw 5-6A at 12V. Use adequate wire gauge and connectors.
- **Heat Dissipation**: Peltier hot side needs heatsink and fan to prevent damage.
- **Thermal Limits**: Do not exceed thermocouple max temperature (K-type: 1000°C).
- **Electrical Isolation**: SSR load circuit should be isolated from microcontroller.
- **Testing**: Test with low setpoints first, verify control before high temperatures.

## Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

MIT License - see [LICENSE](LICENSE) file for details

## Acknowledgments

- Adafruit for excellent sensor libraries
- Brett Beauregard for PID_v1 library
- ESP8266 community for board support

## Support

- **Issues**: [GitHub Issues](https://github.com/YOUR_USERNAME/thermocouple-pid-controller/issues)
- **Discussions**: [GitHub Discussions](https://github.com/YOUR_USERNAME/thermocouple-pid-controller/discussions)

## Support This Project

If you find this thermal control module useful for your biohacking or bioprocess projects, consider supporting development:

- ⭐ **Star this repository** - Help others discover this project
- 🛒 **Use the affiliate links above** when purchasing hardware (supports open-source development at no extra cost)
- ☕ **[Buy me a coffee](https://buymeacoffee.com/electrobio)** - Direct support for new features
- 🐛 **Report bugs and suggest features** - Help make this better for everyone
- 📖 **Share your build** - Open an issue or discussion showing your bioreactor setup!

## Author

**ElectroBio Team** (Cnd-North)
🌐 Website: [electrobio.xyz](https://www.electrobio.xyz)
☕ Support: [Buy me a coffee](https://buymeacoffee.com/electrobio)
🔬 Passion: Making precision biotech tools accessible to everyone

---

**Made with ❤️ for precision temperature control and open-source biohacking**
