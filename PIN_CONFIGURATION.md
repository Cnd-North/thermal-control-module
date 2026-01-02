# Pin Configuration for Thermocouple PID Controller

## Hardware Setup (DO NOT CHANGE - Physical Board Wired)

### MAX31855 Thermocouple Module
- **MAXDO (Data Out):** GPIO 12 (D6)
- **MAXCS (Chip Select):** GPIO 15 (D8)
- **MAXCLK (Clock):** GPIO 14 (D5)

### Solid State Relay (SSR) for Peltier Plates
- **SSR_PIN:** GPIO 0 (D3)

### Rotary Encoder
- **ENCODER_CLK:** SCK (GPIO 14) - Note: Shares with MAXCLK
- **ENCODER_DT:** GPIO 9 (SD2)
- **ENCODER_SW (Button):** GPIO 10 (SD3)

### OLED Display (SSD1306)
- **SDA:** GPIO 4 (D2) - I2C Data
- **SCL:** GPIO 5 (D1) - I2C Clock
- **Address:** 0x3C

### Potentiometer (Legacy - Not Currently Used)
- **POT_PIN:** A0

## Pin Conflict Notes
- ENCODER_CLK uses SCK which is typically SPI clock
- This may share with MAXCLK (GPIO 14) - verify if this causes issues
- GPIO 9 and 10 are typically SD card pins on ESP8266

## Board Type
- ESP8266 (based on pin definitions)
- Using software SPI for MAX31855
