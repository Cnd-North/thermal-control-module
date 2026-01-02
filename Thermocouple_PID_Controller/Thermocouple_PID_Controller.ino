/*
 * Thermocouple PID Temperature Controller
 *
 * Features:
 * - MAX31855 thermocouple temperature sensing
 * - PID control for Peltier plate via SSR with PWM
 * - Rotary encoder for setpoint adjustment
 * - SSD1306 OLED display (128x64)
 * - Two modes: Locked (running) and Adjusting (setpoint change)
 *
 * Hardware:
 * - ESP8266 microcontroller
 * - MAX31855 thermocouple module
 * - SSD1306 OLED display (I2C)
 * - Rotary encoder with push button
 * - Solid State Relay (SSR) for Peltier control
 *
 * Version: 2.0 - Merged best features from all versions
 * Date: 2025-01-01
 */

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_MAX31855.h>
#include <PID_v1_bc.h>

// ============================================================================
// PIN DEFINITIONS (DO NOT CHANGE - Hardware is physically wired)
// ============================================================================

// MAX31855 Thermocouple (Software SPI)
#define MAXDO   12  // D6 - Data Out
#define MAXCS   15  // D8 - Chip Select
#define MAXCLK  14  // D5 - Clock

// Solid-State Relay (SSR) for Peltier Plates
#define SSR_PIN 0   // D3 - PWM output to SSR

// Rotary Encoder
#define ENCODER_CLK SCK  // SCK - Clock (Note: shares GPIO with SPI)
#define ENCODER_DT  9    // SD2 - Data
#define ENCODER_SW  10   // SD3 - Push Button

// OLED Display (I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// ============================================================================
// PID CONFIGURATION
// ============================================================================

// PID Variables
double setpoint = 25.0;      // Desired temperature in Celsius
double input = 0.0;          // Current temperature from thermocouple
double output = 0.0;         // PID output (0-255 for PWM)

// PID Tuning Parameters
// Starting with conservative values - can be tuned later
double kp = 2.0;             // Proportional gain
double ki = 0.5;             // Integral gain
double kd = 1.0;             // Derivative gain

// PID Controller
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);

// ============================================================================
// ENCODER VARIABLES
// ============================================================================

// Encoder state tracking
int lastEncoderClk = LOW;
volatile int encoderPosition = 0;  // Encoder position counter

// Button state and debouncing
bool lastButtonState = HIGH;      // Encoder button (INPUT_PULLUP)
bool setpointAdjustMode = false;  // true = Adjusting, false = Locked
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // ms

// Setpoint adjustment settings
const double TEMP_INCREMENT = 0.5;  // Degrees per encoder click
const double MIN_SETPOINT = 10.0;   // Minimum temperature
const double MAX_SETPOINT = 100.0;  // Maximum temperature
double tempSetpoint = setpoint;     // Temporary setpoint while adjusting

// ============================================================================
// DISPLAY UPDATE TIMING
// ============================================================================

unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 100; // Update display every 100ms

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=== Thermocouple PID Controller v2.0 ===");

  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: SSD1306 allocation failed"));
    for (;;); // Halt if display fails
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("PID CTRL");
  display.setTextSize(1);
  display.setCursor(20, 45);
  display.println("Initializing...");
  display.display();
  delay(2000);

  // Initialize MAX31855 Thermocouple
  Serial.println("Initializing thermocouple...");
  delay(500); // Let MAX chip stabilize

  if (!thermocouple.begin()) {
    Serial.println(F("ERROR: Couldn't find MAX31855"));
    displayError("Thermocouple\nNot Found!");
    for (;;); // Halt if thermocouple fails
  }

  // Read initial temperature
  input = thermocouple.readCelsius();
  if (isnan(input)) {
    Serial.println(F("ERROR: Invalid thermocouple reading"));
    displayError("Thermocouple\nRead Error!");
    for (;;);
  }

  Serial.print("Initial temperature: ");
  Serial.print(input);
  Serial.println(" C");

  // Set up PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255); // PWM range
  Serial.println("PID initialized");

  // Set up SSR pin (PWM output)
  pinMode(SSR_PIN, OUTPUT);
  analogWrite(SSR_PIN, 0); // Ensure SSR is off initially
  Serial.println("SSR configured");

  // Set up encoder pins
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SW, INPUT_PULLUP);
  lastEncoderClk = digitalRead(ENCODER_CLK);
  Serial.println("Encoder configured");

  // Display ready message
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(25, 20);
  display.println("READY!");
  display.display();
  delay(1000);

  Serial.println("=== System Ready ===\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Read temperature from thermocouple
  input = thermocouple.readCelsius();

  // Check for thermocouple errors
  if (isnan(input)) {
    Serial.println("ERROR: Thermocouple read failed");
    displayError("Sensor Error!");
    delay(1000);
    return;
  }

  // Handle encoder rotation
  handleEncoder();

  // Handle button press (mode switching)
  handleButton();

  // Compute PID (only when in locked mode)
  if (!setpointAdjustMode) {
    myPID.Compute();

    // Apply PWM to SSR (output is 0-255)
    analogWrite(SSR_PIN, (int)output);

    // Debug output to Serial
    static unsigned long lastSerialPrint = 0;
    if (millis() - lastSerialPrint > 1000) {
      Serial.print("Temp: ");
      Serial.print(input, 2);
      Serial.print(" C | Setpoint: ");
      Serial.print(setpoint, 2);
      Serial.print(" C | Output: ");
      Serial.print((int)output);
      Serial.println("/255");
      lastSerialPrint = millis();
    }
  } else {
    // In adjust mode, turn off SSR for safety
    analogWrite(SSR_PIN, 0);
  }

  // Update display
  if (millis() - lastDisplayUpdate > displayUpdateInterval) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }

  delay(10); // Small delay for stability
}

// ============================================================================
// ENCODER HANDLING
// ============================================================================

void handleEncoder() {
  int clkState = digitalRead(ENCODER_CLK);

  // Check if encoder has rotated
  if (clkState != lastEncoderClk) {
    int dtState = digitalRead(ENCODER_DT);

    // Determine direction
    if (dtState != clkState) {
      encoderPosition++; // Clockwise
    } else {
      encoderPosition--; // Counter-clockwise
    }

    // Update temporary setpoint if in adjust mode
    if (setpointAdjustMode) {
      tempSetpoint += (dtState != clkState) ? TEMP_INCREMENT : -TEMP_INCREMENT;

      // Constrain to valid range
      if (tempSetpoint < MIN_SETPOINT) tempSetpoint = MIN_SETPOINT;
      if (tempSetpoint > MAX_SETPOINT) tempSetpoint = MAX_SETPOINT;
    }

    lastEncoderClk = clkState;
  }
}

// ============================================================================
// BUTTON HANDLING
// ============================================================================

void handleButton() {
  bool currentButtonState = digitalRead(ENCODER_SW);

  // Detect button press (LOW when pressed due to INPUT_PULLUP)
  if (currentButtonState != lastButtonState) {
    unsigned long currentTime = millis();

    // Debounce
    if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (currentButtonState == LOW) { // Button pressed
        // Toggle mode
        setpointAdjustMode = !setpointAdjustMode;

        if (setpointAdjustMode) {
          // Entering adjust mode - initialize temp setpoint
          tempSetpoint = setpoint;
          Serial.println("Mode: ADJUSTING");
        } else {
          // Exiting adjust mode - save new setpoint
          setpoint = tempSetpoint;
          Serial.print("Mode: LOCKED | New Setpoint: ");
          Serial.print(setpoint, 2);
          Serial.println(" C");
        }
      }
      lastDebounceTime = currentTime;
    }
  }

  lastButtonState = currentButtonState;
}

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (setpointAdjustMode) {
    // ADJUSTING MODE - Show preview of new setpoint
    display.setCursor(0, 0);
    display.println("MODE: ADJUSTING");
    display.println();

    display.setTextSize(2);
    display.print("SET: ");
    display.print(tempSetpoint, 1);
    display.println("C");

    display.setTextSize(1);
    display.println();
    display.println("Turn to adjust");
    display.println("Press to lock");

  } else {
    // LOCKED MODE - Show current temp and operation
    display.setCursor(0, 0);
    display.println("MODE: LOCKED");
    display.println();

    // Current temperature (large)
    display.setTextSize(2);
    display.print(input, 1);
    display.println(" C");

    // Setpoint
    display.setTextSize(1);
    display.print("Target: ");
    display.print(setpoint, 1);
    display.println(" C");

    // PWM output percentage
    display.print("Power: ");
    display.print((int)((output / 255.0) * 100));
    display.println("%");
  }

  display.display();
}

void displayError(const char* message) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("ERROR!");
  display.setTextSize(1);
  display.setCursor(10, 45);
  display.println(message);
  display.display();
}
