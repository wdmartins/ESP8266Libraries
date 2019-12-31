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

void resetSwCounters() {
  rswCounter = 0;
  lswCounter = 0;
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
  _speedFactor = DEFAULT_SPEED_FACTOR;
  _milisecondsPer45Degress = MILISECONDS_PER_45_DEGREES;
  _isTurnCalibrating = false;
  _movingState = MovingState::STOP;
}

Drive::Drive(int IN1, int IN2, int IN3, int IN4)
{
  Serial.println("[DRIVE] Initialize drive without calibration");
  initL298N(IN1, IN2, IN3, IN4);
  _RSW = -1;
  _LSW = -1;
}

Drive::Drive(int IN1, int IN2, int IN3, int IN4, int RSW, int LSW) {
  Serial.println("[DRIVE] Initialize drive with calibration");
  initL298N(IN1, IN2, IN3, IN4);
  _RSW = RSW;
  _LSW = LSW;
  // Attach interrupts for slotted wheel pulse counters
  attachInterrupt(digitalPinToInterrupt(_RSW), countRswPulses, RISING);
  attachInterrupt(digitalPinToInterrupt(_LSW), countLswPulses, RISING);
}


float Drive::calculateSpeedFactor(unsigned int rswc, unsigned int lswc) {
  if (rswc < PULSES_PER_CALIBRATION_STEP || lswc < PULSES_PER_CALIBRATION_STEP) {
    Serial.println("[DRIVE] Not enough pulses to calibrate.");
    return 0;
  }
  return 100 * lswc / rswc;
}


bool Drive::handleDrive() {
  if (_movingState == MovingState::FORWARD || _movingState == MovingState::BACKWARD) {
    float speedFactor = calculateSpeedFactor(rswCounter, lswCounter);
    if (speedFactor != 0) {
      _speedFactor = speedFactor;
    }
  }
  return true;
}

void Drive::startTurnCalibration() {
  _isTurnCalibrating = true;
}

void Drive::tuneTurn(uint16_t miliseconds) {
  Serial.printf("[DRIVE] Tune turn at %d\n", miliseconds);
  delay(1000);
  turnRight(DEFAULT_TURN_SPEED);
  delay(miliseconds);
  _milisecondsPer45Degress = miliseconds / 2;
  stopMoving();
}

bool Drive::isCalibrating() {
  return _isTurnCalibrating;
}

bool Drive::stopCalibration() {
  Serial.println("[DRIVE] Stopping motors");
  _isTurnCalibrating = false;
  stopMoving();
  return true;
}

void Drive::report(bool reset ) {
  Serial.printf("[DRIVE] RSW= %d, LSW= %d, SF= %f, TF= %d\n", rswCounter, lswCounter, _speedFactor, _milisecondsPer45Degress);
  if (reset) {
    resetSwCounters();
  }
}

uint16_t Drive::calculateTurningTimeMS(uint16_t degrees, uint16_t speed) {
  return (MILISECONDS_PER_45_DEGREES * (degrees / 45) * (DEFAULT_SPEED / speed));
}

bool Drive::moveForward(int16_t speed)
{
  if (speed == USE_CURRENT_SPEED) {
    speed = _currentMovingSpeed;
  }
  Serial.printf("[DRIVE] Move Forward at speed %d\n", speed);
  analogWrite(_IN1, (int)(speed * _speedFactor / 100));
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, speed);
  analogWrite(_IN4, LOW);
  _movingState = MovingState::FORWARD;
  _currentMovingSpeed = speed;
  return true;
}

bool Drive::moveBackward(int16_t speed)
{
  if (speed == USE_CURRENT_SPEED) {
    speed = _currentMovingSpeed;
  }
  Serial.printf("[DRIVE] Move Backward at speed %d\n", speed);
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, (int)(speed * _speedFactor / 100));
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, speed); 
  _movingState = MovingState::BACKWARD;
  _currentMovingSpeed = speed;
  return true;
}

void Drive::resumeMoving(void) {
  if (_movingState == MovingState::FORWARD) {
    moveForward(_currentMovingSpeed);
  } else if (_movingState == MovingState::BACKWARD) {
    moveBackward(_currentMovingSpeed);
  } else {
    stopMoving();
  }
}

bool Drive::turnLeft(int16_t speed)
{
  if (speed == USE_CURRENT_SPEED) {
    speed = _currentMovingSpeed;
  }
  Serial.printf("[DRIVE] Turn left at speed %d\n", speed);
  analogWrite(_IN1, speed);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, (int)(speed * _speedFactor / 100));
  return true;
}

void Drive::turnLeftDegrees(uint16_t degrees, uint16_t speed) {
  if (speed == USE_CURRENT_SPEED) {
    speed = _currentMovingSpeed;
  }
  turnLeft(speed);
  delay(MILISECONDS_PER_45_DEGREES + calculateTurningTimeMS(degrees, speed));
  resumeMoving();
}

bool Drive::turnRight(int16_t speed)
{
  if (speed == USE_CURRENT_SPEED) {
    speed = _currentMovingSpeed;
  }
  Serial.printf("[DRIVE] Turn right at speed %d\n", speed);
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, (int)(speed * _speedFactor / 100));
  analogWrite(_IN3, speed);
  analogWrite(_IN4, LOW);
  return true;  
}

void Drive::turnRightDegrees(uint16_t degrees, uint16_t speed) {
  if (speed == USE_CURRENT_SPEED) {
    speed = _currentMovingSpeed;
  }
  turnRight(speed);
  delay(MILISECONDS_PER_45_DEGREES + calculateTurningTimeMS(degrees, speed));
  resumeMoving();
}

bool Drive::stopMoving(bool pause)
{
  Serial.println("[DRIVE] Stop moving.");
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, LOW); 
  if (!pause) {
    _movingState = MovingState::STOP;
    _currentMovingSpeed = 0;
  }
  return true;  
}

bool Drive::isMoving() {
  return (_movingState != MovingState::STOP && _currentMovingSpeed >= 0);
}