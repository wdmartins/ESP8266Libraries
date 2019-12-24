/*
 * Drive.h - Library to abstract Half Bridge to Drive two DC motors
 * Created by Walter D. Martins, December 24, 2019
 */
#ifndef Drive_h
#define Drive_h

const uint8_t CALIBRATION_STEPS = 5;
const uint8_t PULSES_PER_CALIBRATION_STEP = 200;
const uint8_t CALIBRATION_FWD = 0;
const uint8_t CALIBRATION_BKD = 1;
const int CALIBRATION_SPEED = 450;
const float MINIMUM_SPEED_FACTOR = 2; // 2% difference between right and left wheels
const float DEFAULT_SPEED_FACTOR = 100;

class Drive
{
  private:
    const uint8_t CALIBRATION_STEPS = 5;             // Number of wheel calibration steps
    const uint8_t PULSES_PER_CALIBRATION_STEP = 200; // Pulses needed to perform calibration
    const uint8_t CALIBRATION_FWD = 0;               // Forward speed calibration
    const uint8_t CALIBRATION_BKD = 1;               // Backward speed calibration
    const uint8_t PAUSE_BTW_CALIBRATION_STEPS = 250; // Miliseconds
    const int CALIBRATION_SPEED = 250;               // Calibration speed
    const float MINIMUM_SPEED_FACTOR = 2;            // Maximun different to considered calibrated
    const float DEFAULT_SPEED_FACTOR = 100;          // Default calibration factor (%)

  public:
    // Use this constructor when slotted wheel for calibration are not available
    Drive(int IN1, int IN2, int IN3, int IN4);
    // Use this constructor when slotted wheel are furnished and calibration is needed
    Drive(int IN1, int IN2, int IN3, int IN4, int RSW, int LSW);
    // Drive car forward
    bool moveForward(int speed);
    // Drive car backward
    bool moveBackward(int speed);
    // Turn car to the right
    bool turnRight(int speed);
    // Turn car to the left
    bool turnLeft(int speed);
    // Stop motors
    bool stopMoving(void);
    // Start calibration
    bool startCalibration(bool forward = true);
    // Force stop calibration
    bool stopCalibration(void);
    // Call in loop
    bool handleDrive(void);
    // Returns true if the calibration is in progress
    bool isCalibrating(void);
    // Report slotted wheel counter. Reset counter optionally
    void report(bool reset = false);
  private:
    void initL298N(int IN1, int IN2, int IN3, int IN4);
    bool calculateSpeedFactor(void);
    int _IN1;
    int	_IN2;
    int _IN3;
    int _IN4;
    int _RSW;
    int _LSW;
    float _fwSpeedFactor; // How much faster the right motor should run respect of the left one
    float _bwSpeedFactor; // How much faster the right motor should run respect of the left one
    unsigned int _calibrationSteps;
    bool _forwardCalibration;
    bool _isCalibrating;
};

#endif
