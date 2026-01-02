# PID Parameter Presets

This file contains PID tuning presets for different thermal control scenarios. Use these as starting points and fine-tune based on your specific hardware.

## How to Use

1. Copy the values for your application scenario
2. Edit `Thermocouple_PID_Controller.ino` lines 67-69
3. Upload to ESP8266
4. Test and fine-tune using the PID Tuning Guide

## Preset Configurations

### 1. Conservative (Default - Good Starting Point)
**Use for:** Initial testing, unknown thermal mass, safety-first approach

```cpp
double kp = 2.0;   // Proportional gain
double ki = 0.5;   // Integral gain
double kd = 1.0;   // Derivative gain
```

**Characteristics:**
- Slow but stable response
- Minimal overshoot
- Good for testing without damage risk

---

### 2. Small Thermal Mass (Fast Response)
**Use for:** PCR blocks, small tubes, thin probes, air heating

**Low-power Peltier (TEC1-12706, 12V 6A):**
```cpp
double kp = 3.0;
double ki = 0.8;
double kd = 2.0;
```

**High-power Peltier (TEC1-12715, 12V 15A):**
```cpp
double kp = 1.5;   // Lower - plate reacts quickly
double ki = 0.6;
double kd = 2.5;   // Higher - prevent overshoot
```

**Characteristics:**
- Reaches temperature quickly (1-2 min)
- Risk of overshoot - needs high Kd
- Good for rapid cycling

---

### 3. Medium Thermal Mass (Bioreactor Vessels)
**Use for:** 50-500mL bioreactor vessels, water baths, culture flasks

**Well-insulated:**
```cpp
double kp = 4.0;   // Can be aggressive - slow heat loss
double ki = 1.2;   // Higher - compensate for drift
double kd = 3.0;   // Prevent overshoot
```

**Poor insulation:**
```cpp
double kp = 5.0;   // Need more power to overcome losses
double ki = 2.0;   // Higher - fight constant heat loss
double kd = 2.0;
```

**Characteristics:**
- 3-5 minute rise time
- Requires steady-state correction (Ki)
- Most common bioreactor application

---

### 4. Large Thermal Mass (Heavy Blocks)
**Use for:** Metal blocks, large water baths, incubators >1L

```cpp
double kp = 8.0;   // Very high - need strong initial push
double ki = 1.5;   // Moderate - eventually reaches target
double kd = 5.0;   // High - prevent slow overshoot
```

**Characteristics:**
- 10-20 minute rise time
- Very stable once at temperature
- Low overshoot risk due to thermal inertia

---

### 5. Precision Applications (±0.1°C stability)
**Use for:** Enzyme reactions, crystal growth, precise experiments

```cpp
double kp = 1.0;   // Low - gentle control
double ki = 0.3;   // Low - avoid oscillation
double kd = 4.0;   // High - dampen any disturbance
```

**Characteristics:**
- Slow to reach setpoint (5-10 min)
- Extremely stable at temperature
- Minimal oscillation

---

### 6. Rapid Cycling (PCR-style)
**Use for:** Thermocycling, heating/cooling cycles

```cpp
double kp = 2.0;
double ki = 0.2;   // Very low - don't accumulate error during ramps
double kd = 3.0;   // High - smooth transitions
```

**Additional code modification needed:**
```cpp
// In setup():
myPID.SetSampleTime(50);  // Faster PID updates (default 100ms)

// Consider bang-bang control for rapid heating:
if (abs(input - setpoint) > 5.0) {
  // Large error: full power
  analogWrite(SSR_PIN, (input < setpoint) ? 255 : 0);
} else {
  // Near setpoint: PID control
  myPID.Compute();
  analogWrite(SSR_PIN, (int)output);
}
```

---

## Hardware-Specific Adjustments

### Peltier Plate Size

**Small plates (20x20mm, <2A):**
- Decrease Kp by 50% (faster response)
- Increase Kd by 50% (prevent overshoot)

**Large plates (40x40mm, >6A):**
- Increase Kp by 50% (more thermal mass to move)
- Decrease Kd by 30% (slower response = less overshoot risk)

### Heatsink Quality

**Good heatsink (large, with fan):**
- Can use higher Kp (Peltier efficiency is good)
- Standard Ki/Kd

**Poor heatsink (small, passive):**
- Reduce Kp by 30% (Peltier efficiency drops at high power)
- Increase Ki by 50% (compensate for efficiency loss)

### Power Supply

**Regulated 12V supply:**
- Standard values

**Unregulated or weak supply (voltage sag):**
- Increase Kp by 20% (compensate for power loss)
- Increase Ki by 30% (fight steady-state error)

---

## Tuning Tips by Symptom

### Temperature oscillates ±1-2°C continuously
**Problem:** Kp too high, Kd too low

**Fix:**
1. Halve Kp
2. Double Kd
3. Test again

### Reaches 95% but never quite hits setpoint
**Problem:** Ki too low

**Fix:**
1. Double Ki
2. If starts oscillating, increase Kd slightly

### Overshoots by 5-10°C then settles
**Problem:** Kp too high, Kd too low

**Fix:**
1. Reduce Kp by 30%
2. Increase Kd by 50%

### Takes forever (>10 min for small load)
**Problem:** Kp too low

**Fix:**
1. Double Kp
2. Increase Kd proportionally to prevent overshoot

### Slow waves (period >30 seconds)
**Problem:** Ki too high

**Fix:**
1. Reduce Ki by 50%
2. Slightly increase Kp to maintain responsiveness

---

## Testing Checklist

Before deploying in production:

- [ ] Test step response (room temp → +20°C)
- [ ] Measure rise time (time to 90% of setpoint)
- [ ] Check overshoot (should be <2°C)
- [ ] Monitor settling time (within ±0.2°C)
- [ ] Verify steady-state stability (±0.1°C for 10 min)
- [ ] Test disturbance rejection (open lid, add cold item)
- [ ] Confirm safe limits (setpoint constraints working)
- [ ] Run overnight stability test (8+ hours)

---

## Recommended Starting Point for Bioreactor

Based on typical bioreactor requirements (37°C ±0.5°C):

```cpp
// For 50-250mL vessel with TEC1-12706 Peltier
double kp = 4.0;
double ki = 1.0;
double kd = 2.5;
```

**Expected performance:**
- Rise time: 3-5 minutes
- Overshoot: <1°C
- Stability: ±0.3°C
- Good for most bacterial/cell culture applications

---

## Advanced: Adaptive Tuning

For systems with different heating/cooling characteristics:

```cpp
// In loop(), before myPID.Compute():
if (input < setpoint - 2.0) {
  // Heating aggressively
  myPID.SetTunings(kp * 1.2, ki, kd);
} else if (input > setpoint + 2.0) {
  // Cooling (often slower with Peltier)
  myPID.SetTunings(kp * 0.8, ki, kd * 1.5);
} else {
  // Near setpoint - use default values
  myPID.SetTunings(kp, ki, kd);
}
```

---

## Support

Having trouble tuning? Open an issue with:
1. Your hardware specs (Peltier model, thermal mass, power supply)
2. Current PID values
3. Serial Monitor output (2-3 min of data)
4. Description of behavior

We'll help you optimize! 🎛️
