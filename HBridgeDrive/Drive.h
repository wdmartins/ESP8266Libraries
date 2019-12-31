/*
 * Drive.h - Library to abstract Half Bridge to Drive two DC motors
 * Created by Walter D. Martins, December 24, 2019
 */
#ifndef Drive_h
#define Drive_h

typedef enum  {
  FORWARD,
  BACKWARD,
  STOP
} MovingState;

class Drive
{
  private:
    /* Default calibration values */
    static const uint8_t PULSES_PER_CALIBRATION_STEP = 100; // Pulses needed to perform calibration
    static const uint16_t DEFAULT_SPEED = 250;              // Default speed
    const float MINIMUM_SPEED_FACTOR = 2;                   // Maximun different to considered calibrated (%)
    const float DEFAULT_SPEED_FACTOR = 100;                 // Default calibration factor (%)
    static const uint16_t DEFAULT_TURN_SPEED = 250;         // Default turn speed
    static const uint16_t MILISECONDS_PER_45_DEGREES = 200; // 45 degrees turn at CALIBRATION_SPEED
    static const int16_t USE_CURRENT_SPEED = -1;            // Use current set speed if not specified
  public:
    static const uint16_t SHORT_MOVE_MS = 400;              // Short adjustment sprint time

  public:
    // Use this constructor when slotted wheel for calibration are not available
    Drive(int IN1, int IN2, int IN3, int IN4);
    // Use this constructor when slotted wheel are furnished and calibration is needed
    Drive(int IN1, int IN2, int IN3, int IN4, int RSW, int LSW);
    // Drive car forward
    bool moveForward(int16_t speed = USE_CURRENT_SPEED);
    // Drive car backward
    bool moveBackward(int16_t speed = USE_CURRENT_SPEED);
    // Turn car to the right
    bool turnRight(int16_t speed = USE_CURRENT_SPEED);
    // Turn car right to a given angle
    void turnRightDegrees(uint16_t degrees, uint16_t speed = USE_CURRENT_SPEED);
    // Turn car to the left
    bool turnLeft(int16_t speed = USE_CURRENT_SPEED);
    // Turn car left to a given angle
    void turnLeftDegrees(uint16_t degrees, uint16_t speed = USE_CURRENT_SPEED);
    // Stop motors
    bool stopMoving(bool pause = false);
    // Resume moving
    void resumeMoving(void);
    // Start turn calibration
    void startTurnCalibration(void);
    // Tune turn
    void tuneTurn(uint16_t miliseconds);
    // Stop calibration
    bool stopCalibration(void);
    // Call in loop
    bool handleDrive(void);
    // Returns true if the calibration is in progress
    bool isCalibrating(void);
    // Report slotted wheel counter. Reset counter optionally
    void report(bool reset = false);
    // Returns true if the car is moving
    bool isMoving(void);
  private:
    void initL298N(int IN1, int IN2, int IN3, int IN4);
    float calculateSpeedFactor(unsigned int rswc, unsigned int lswc);
    // Calculates how long to turn given degrees at given speed
    uint16_t calculateTurningTimeMS(uint16_t degrees, uint16_t speed);
    int _IN1;
    int	_IN2;
    int _IN3;
    int _IN4;
    int _RSW;
    int _LSW;
    float _speedFactor;
    uint16_t _milisecondsPer45Degress; // Miliseconds of turning to turn 45 degrees (1/8 of a circle)
    bool _isTurnCalibrating;
    MovingState _movingState;
    uint16_t _currentMovingSpeed;
};

#endif
