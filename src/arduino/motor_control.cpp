#include <AccelStepper.h>

#define STEP_PIN 7
#define DIR_PIN  4
#define EN_PIN   8          // LOW = enable

AccelStepper m(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

/* ------------ motor-driver parameters ------------- */
const int   MICROSTEP   = 2;                           // 1/2-step
const int   STEPS_REV   = 200 * MICROSTEP;             // 400 steps/rev at 1/2-step
const float GEAR_RATIO  = 5.0;                         // 5:1 gearbox
const float STEPS_DEG   = (STEPS_REV * GEAR_RATIO) / 360.0;  // ≈ 5.56 µsteps/deg

// Match first code behavior:
const float MAX_SPEED_DEG = 1400.0;                    // deg/s
const float ACCEL_DEG     = 1800.0;                    // deg/s²
/* -------------------------------------------------- */

const unsigned long IDLE_MS = 1750;                     // auto-centre timeout

void setup() {
  Serial.begin(250000);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);           // enable driver

  // Apply speed/acceleration in deg/s then scale to steps
  m.setMaxSpeed(MAX_SPEED_DEG * STEPS_DEG);
  m.setAcceleration(ACCEL_DEG * STEPS_DEG);

  Serial.println(F("Ready – send  G<angle>  (sign is inverted on motor side)."));
  Serial.print(F("Gear Ratio: "));
  Serial.println(GEAR_RATIO);
  Serial.print(F("Steps per degree: "));
  Serial.println(STEPS_DEG, 4);
}

  // -------------------------------------------------------------------
// Compact binary protocol: 0xA5  <int16_t angle_little_endian>
// -------------------------------------------------------------------
const uint8_t  HDR        = 0xA5;
const uint8_t  FRAME_LEN  = 3;          // header + payload

void loop() {
  static uint8_t buf[FRAME_LEN];
  static uint8_t idx = 0;

  while (Serial.available()) {
    uint8_t b = Serial.read();

    if (idx == 0 && b != HDR) continue;   // resync on header
    buf[idx++] = b;

    if (idx == FRAME_LEN) {               // full frame arrived
      int16_t cmdDeg;
      memcpy(&cmdDeg, &buf[1], 2);        // little-endian
      long tgtSteps = lround(-cmdDeg * STEPS_DEG);
      m.moveTo(tgtSteps);
      idx = 0;                            // ready for next frame
    }
  }
  m.run();
}