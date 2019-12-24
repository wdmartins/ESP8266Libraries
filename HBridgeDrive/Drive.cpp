/*
 * Drive.h - Library to abstract Half Bridge to Drive two DC motors
 * Created by Walter D. Martins, December 24, 2019
 */
#include "Arduino.h"
#include "Drive.h"

// Right slotted wheel pulse counter
volatile unsigned int rswCounter;
// Right slotted wheel pulse counter
volatile unsigned int lswCounter;

void countRswPulses() {
  rswCounter++;
}

void countLswPulses() {
  lswCounter++;
}

void Drive::initL298N(int IN1, int IN2, int IN3, int IN4) {
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  _IN1 = IN1;
  _IN2 = IN2;
  _IN3 = IN3;
  _IN4 = IN4;
  _fwSpeedFactor = DEFAULT_SPEED_FACTOR;
  _bwSpeedFactor = DEFAULT_SPEED_FACTOR;
}

Drive::Drive(int IN1, int IN2, int IN3, int IN4)
{
  Serial.println("[DRIVE] Initialize drive without calibration");
  initL298N(IN1, IN2, IN3, IN4);
}

Drive::Drive(int IN1, int IN2, int IN3, int IN4, int RSW, int LSW) {
  Serial.println("[DRIVE] Initialize drive with calibration");
  initL298N(IN1, IN2, IN3, IN4);
  _RSW = RSW;
  _LSW = LSW;
}

bool Drive::startCalibration(bool forward) {
  Serial.printf("[DRIVE] Start Calibration in %s direction.\n", (forward ? "forward" : "backward"));
  // Initialize control variables and pulse counters
  _isCalibrating = true;
  _forwardCalibration = forward;
  rswCounter = 0;
  lswCounter = 0;
  _calibrationSteps = CALIBRATION_STEPS;
  // Attach interrupts for slotted wheel pulse counters
  attachInterrupt(digitalPinToInterrupt(_RSW), countRswPulses, RISING);
  attachInterrupt(digitalPinToInterrupt(_LSW), countLswPulses, RISING);
  forward ? moveForward(CALIBRATION_SPEED) : moveBackward(CALIBRATION_SPEED);
  Serial.println("[DRIVE] Starting motors calibration");
  return true;
}

bool Drive::calculateSpeedFactor() {
  Serial.printf("[DRIVE] 1. CS: %d, LSW= %d, RSW= %d, SF= %f\n", _calibrationSteps, lswCounter, rswCounter, (_forwardCalibration ? _fwSpeedFactor : _bwSpeedFactor));
  if (lswCounter == 0 || rswCounter == 0) {
    // Pulses are not being count. Cannot perform calibration
    Serial.printf("[DRIVE] Cannot perform calibration: LSW= %d, RSW= %d\n", lswCounter, rswCounter);
    return true;
  }
  // The speed factor is greater than 1 when left wheel is faster than right wheel
  if (_calibrationSteps == CALIBRATION_STEPS) {
    // First calibration step. Calculate calibration factor as percentage
    if (_forwardCalibration) {
      _fwSpeedFactor = 100 * lswCounter / rswCounter;
    } else {
      _bwSpeedFactor = 100 * lswCounter / rswCounter;
    }
  } else {
    // Subsequent calibration steps. 
    float lSpeedFactor = 100 * lswCounter / rswCounter;
    // Keep adjusting
    Serial.printf("[DRIVE] Adjust speed factor: previous= %f, new= %f\n", (_forwardCalibration ? _fwSpeedFactor : _bwSpeedFactor), lSpeedFactor);
    if (_forwardCalibration) {
      _fwSpeedFactor = _fwSpeedFactor * lSpeedFactor / 100;
    } else {
      _bwSpeedFactor = _bwSpeedFactor * lSpeedFactor / 100;
    }
  }
  stopMoving();
  delay(PAUSE_BTW_CALIBRATION_STEPS);
  _forwardCalibration ? moveForward(CALIBRATION_SPEED) : moveBackward(CALIBRATION_SPEED);

  // Report values
  Serial.printf("[DRIVE] 2. CS: %d, LSW= %d, RSW= %d, SF= %f\n", _calibrationSteps, lswCounter, rswCounter, (_forwardCalibration ? _fwSpeedFactor : _bwSpeedFactor));

  // Decrement calibration tries
  _calibrationSteps--;

  // Reset pulse counters
  lswCounter = 0;
  rswCounter = 0;

  // Return true if calibration is done.
  bool goodEnoughCalibration = abs((_forwardCalibration ? _fwSpeedFactor : _bwSpeedFactor)) <= MINIMUM_SPEED_FACTOR;
  bool tryEnoughCalibration = _calibrationSteps <= 0;
  return goodEnoughCalibration || tryEnoughCalibration;
}

bool Drive::handleDrive() {
  if (_isCalibrating) {
    // More calibration steps remaining
    if (_calibrationSteps > 0) {
      // Check if we have enough pulses to perform calibration
      if (lswCounter >= PULSES_PER_CALIBRATION_STEP) {
        // Recalculate Speed Factor
        if (calculateSpeedFactor()) {
          if (_forwardCalibration) {
            startCalibration(false);
          } else {
            // Calibration ended
            Serial.println("[DRIVE] Calibration Ended");
            stopCalibration();
          }
        }
      }
    }
  }
  return _isCalibrating;
}

bool Drive::isCalibrating() {
  return _isCalibrating;
}

bool Drive::stopCalibration() {
  Serial.println("[DRIVE] Stopping motors");
  _isCalibrating = false;
  stopMoving();
  return true;
}

void Drive::report(bool reset ) {
  Serial.printf("[DRIVE] RSW= %d, LSW= %d\n", rswCounter, lswCounter);
  if (reset) {
    rswCounter = 0;
    lswCounter = 0;
  }
}

bool Drive::moveForward(int speed)
{
  Serial.printf("[DRIVE] Move Forward at speed %d\n", speed);
  analogWrite(_IN1, (int)(speed * _fwSpeedFactor / 100));
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, speed);
  analogWrite(_IN4, LOW);  
  return true;
}

bool Drive::moveBackward(int speed)
{
  Serial.printf("[DRIVE] Move Backward at speed %d\n", speed);
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, (int)(speed * _bwSpeedFactor / 100));
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, speed); 
  return true;
}

bool Drive::turnLeft(int speed)
{
  Serial.printf("[DRIVE] Turn left at speed %d\n", speed);
  analogWrite(_IN1, speed);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, (int)(speed * _bwSpeedFactor / 100));
  return true;
}

bool Drive::turnRight(int speed)
{
  Serial.printf("[DRIVE] Turn right at speed %d\n", speed);
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, (int)(speed * _fwSpeedFactor / 100));
  analogWrite(_IN3, speed);
  analogWrite(_IN4, LOW);
  return true;  
}

bool Drive::stopMoving()
{
  Serial.println("[DRIVE] Stop moving.");
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, LOW); 
  return true;  
}
