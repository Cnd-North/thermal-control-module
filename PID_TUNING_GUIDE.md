# PID Tuning Guide for Thermocouple Controller

This guide will help you tune the PID parameters (Kp, Ki, Kd) for optimal temperature control with your specific hardware setup.

## Understanding PID Control

### The Three Components

**Proportional (Kp):**
- Responds to current error (how far from setpoint)
- Higher Kp = faster response but more overshoot
- Too high = oscillation, too low = slow response

**Integral (Ki):**
- Responds to accumulated error over time
- Eliminates steady-state error (offset from setpoint)
- Too high = slow oscillations, too low = never quite reaches setpoint

**Derivative (Kd):**
- Responds to rate of change of error
- Dampens overshoot and oscillation
- Too high = noise sensitivity, too low = overshoot

### Visual Response Patterns

```
Good Tuning:           Too Much Kp:         Too Much Ki:
  Setpoint ─────        Setpoint ─────      Setpoint ─────
  |    ┌───────         |  ┌─┬─┬─┬──        |     ╱╲╱╲
  |  ╱                  |╱ ╲│╲│╲│           |   ╱    ╲
  └──────── Time        └───────── Time     └──────── Time
  Fast, no overshoot    Oscillates badly    Slow waves

Too Little Kp:         Just Right:
  Setpoint ─────        Setpoint ─────
  |        ┌───         |   ┌┬──────
  |      ╱              | ╱ ╲│
  |   ╱──               └────────
  └──────── Time        Slight overshoot,
  Too slow              quick settle
```

## Current Default Settings

```cpp
// In Thermocouple_PID_Controller.ino:
double kp = 2.0;   // Proportional gain
double ki = 0.5;   // Integral gain
double kd = 1.0;   // Derivative gain
```

These are **conservative starting values**. You'll likely need to adjust them.

## Tuning Methods

### Method 1: Manual Tuning (Recommended for Beginners)

#### Step 1: Start with Pure Proportional Control

```cpp
double kp = 1.0;
double ki = 0.0;  // Disable integral
double kd = 0.0;  // Disable derivative
```

1. Upload code and set setpoint 10°C above room temperature
2. Observe response on Serial Monitor
3. If too slow: increase Kp (try 2.0, 4.0, 8.0)
4. If oscillating: decrease Kp (try 0.5, 0.25)
5. Find the Kp where it responds quickly but oscillates slightly

**Goal**: Slight overshoot with small oscillations that don't settle.

#### Step 2: Add Derivative to Dampen Oscillations

```cpp
double kp = <value from Step 1>;
double ki = 0.0;  // Still disabled
double kd = 0.5;  // Start small
```

1. Upload and test again
2. If still oscillating: increase Kd (try 1.0, 2.0)
3. If too sluggish: decrease Kd
4. Find Kd that eliminates oscillations while maintaining speed

**Goal**: Fast response with minimal or no overshoot.

#### Step 3: Add Integral to Eliminate Offset

```cpp
double kp = <value from Step 1>;
double ki = 0.1;  // Start very small!
double kd = <value from Step 2>;
```

1. Upload and test
2. Watch for steady-state error (settles below/above setpoint)
3. If offset remains: increase Ki slightly (0.2, 0.5, 1.0)
4. If slow oscillations appear: decrease Ki

**Goal**: Temperature settles exactly on setpoint with no drift.

### Method 2: Ziegler-Nichols Method (Advanced)

#### Step 1: Find Ultimate Gain (Ku)

```cpp
double kp = 1.0;  // Start here
double ki = 0.0;
double kd = 0.0;
```

1. Set setpoint above room temp
2. Gradually increase Kp until system oscillates consistently
3. Record this Kp value as **Ku** (Ultimate Gain)
4. Measure oscillation period **Tu** in seconds (peak to peak)

#### Step 2: Calculate PID Values

Use Ziegler-Nichols formulas:

```
For PID Control:
Kp = 0.6 × Ku
Ki = 1.2 × Ku / Tu
Kd = 0.075 × Ku × Tu
```

**Example:**
- Ku = 8.0 (oscillates at Kp=8.0)
- Tu = 20 seconds (oscillation period)

```
Kp = 0.6 × 8.0 = 4.8
Ki = 1.2 × 8.0 / 20 = 0.48
Kd = 0.075 × 8.0 × 20 = 12.0
```

Upload these values and test. Fine-tune if needed.

### Method 3: Trial and Error Quick Reference

| Problem | Symptom | Solution |
|---------|---------|----------|
| Too slow to reach temp | Takes minutes to approach | Increase Kp |
| Overshoots badly | Goes way past setpoint | Decrease Kp, increase Kd |
| Oscillates continuously | Bounces around setpoint | Decrease Kp, increase Kd |
| Settles below setpoint | Always 0.5-1°C too low | Increase Ki |
| Slow, wavy oscillations | Big, slow waves | Decrease Ki |
| Noisy, erratic | Jumpy, unstable | Decrease Kd |

## Testing Procedure

### Setup

1. **Serial Monitor**: 115200 baud, watch temp/output
2. **Display**: Shows current temp and power output
3. **Log Data** (optional): Copy Serial output to spreadsheet for graphing

### Test Protocol

```
1. Start at room temperature (e.g., 20°C)
2. Set setpoint to +10°C (e.g., 30°C)
3. Observe response for 5-10 minutes
4. Record:
   - Time to reach setpoint
   - Amount of overshoot
   - Oscillation frequency (if any)
   - Steady-state error
5. Adjust parameters
6. Repeat test
```

### Good Performance Metrics

- **Rise Time**: Reaches 90% of setpoint in < 2 minutes
- **Overshoot**: Less than 1-2°C past setpoint
- **Settling Time**: Stabilizes within ±0.2°C in < 5 minutes
- **Steady State**: Maintains setpoint within ±0.1°C

## Hardware-Specific Considerations

### Thermal Mass

**Large thermal mass** (heavy metal block):
- Slower response → Higher Kp needed
- Less overshoot risk → Can be more aggressive
- Lower Ki may work → Less accumulated error

**Small thermal mass** (thin probe, air):
- Fast response → Lower Kp needed
- More overshoot risk → Need higher Kd
- May need higher Ki → Quick temperature changes

### Peltier Plate Characteristics

**Strong Peltier** (high wattage):
- Can heat/cool faster → Lower Kp
- More overshoot risk → Higher Kd

**Weak Peltier** (low wattage):
- Slower response → Higher Kp
- May never fully reach setpoint → Higher Ki

### Environmental Factors

**Poor insulation:**
- Constant heat loss → Higher Ki needed
- May need offset setpoint

**Air currents:**
- Disturbances → Higher Kd to compensate

## Example Tuning Sessions

### Example 1: Small PCB Heater (Fast Response)

**Starting values:**
```cpp
Kp = 2.0, Ki = 0.5, Kd = 1.0
```

**Problem**: Oscillates ±2°C around setpoint

**Solution:**
```cpp
Kp = 1.0  // Halved
Ki = 0.5  // Unchanged
Kd = 2.0  // Doubled
```

**Result**: Smooth approach, ±0.2°C stability

### Example 2: Large Aluminum Block (Slow Response)

**Starting values:**
```cpp
Kp = 2.0, Ki = 0.5, Kd = 1.0
```

**Problem**: Takes 10 minutes, settles 1°C below target

**Solution:**
```cpp
Kp = 5.0  // Increased for speed
Ki = 1.5  // Increased to eliminate offset
Kd = 3.0  // Increased to prevent overshoot
```

**Result**: Reaches temp in 4 minutes, ±0.3°C stability

## Advanced: Adaptive PID

For future enhancement, consider:

```cpp
// Different PID values for heating vs cooling
if (input < setpoint) {
  // Heating mode
  myPID.SetTunings(kp_heat, ki_heat, kd_heat);
} else {
  // Cooling mode
  myPID.SetTunings(kp_cool, ki_cool, kd_cool);
}
```

Peltier plates often heat and cool at different rates.

## Troubleshooting

### System Doesn't Respond At All

**Check:**
- SSR is wired correctly (GPIO 0 to SSR control)
- SSR LED lights up when output > 0
- Peltier plates have 12V power
- Polarity of Peltier (swap if cooling instead of heating)

### Temperature Runaway

**Immediate actions:**
1. Press encoder button (enters Adjust Mode, SSR off)
2. Unplug Peltier power supply
3. Check code for errors

**Likely causes:**
- Wrong PID direction (change DIRECT to REVERSE)
- Thermocouple reading backwards
- SSR stuck ON (replace SSR)

### Noisy Temperature Readings

**Solutions:**
1. Add averaging to temperature reading:
   ```cpp
   // In loop(), before myPID.Compute():
   static float tempAvg = 0;
   tempAvg = 0.9 * tempAvg + 0.1 * input; // Low-pass filter
   input = tempAvg;
   ```

2. Increase derivative filtering (decrease Kd)

3. Check thermocouple grounding and shielding

## Recording Your Tuned Values

Once you find optimal parameters, record them:

```
=== MY TUNED PID VALUES ===
Hardware: [describe your setup]
Thermal mass: [weight/material]
Peltier: [model/wattage]

Kp = ______
Ki = ______
Kd = ______

Performance:
- Rise time: _____ seconds
- Overshoot: _____ °C
- Settling time: _____ seconds
- Stability: ±_____ °C

Notes:
[Any observations]
```

## Further Reading

- [Wikipedia: PID Controller](https://en.wikipedia.org/wiki/PID_controller)
- [Brett Beauregard's PID Series](http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/)
- [Control Systems Engineering (Nise)](https://www.wiley.com/en-us/Control+Systems+Engineering-p-9781118170519)

## Need Help?

If you're still having trouble, open an issue with:
- Your current PID values
- Serial Monitor output (2-3 minutes)
- Description of your hardware setup
- Photos/videos of temperature response

Good luck tuning! 🎛️
