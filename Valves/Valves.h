/*
 * FlowMeter.h - Library to abstract hidraulic valves
 * Created by Walter D. Martins, May 4, 2019
 */

#ifndef VALVES_H
#define VALVES_H
#include <FlowMeter.h>
#include <Ticker.h>

// Pure virtual Valve class. It can only be open or close.
class Valve {
  public:
    virtual void openValve(void) = 0;
    void closeValve(void);
    bool isValveOpen();
    void resetOpenStatus(bool);
  protected:
    bool _isOpen;          
};

// Solenoid Valve class. Solenoid Valve abstracts H bridge that open/close the valve
// Open/Close status of valve cannot be guaranteed without fluid flow feedback.
// Class assume valve is close upon instantiation. H bridge needs an enable port
// and a signal port which is feed into one control of the H bridge, and to an inverted
// gate the feeds the other signal port.
class SolenoidValve: public Valve {
  private:
    const int SOLENOID_PULSE_LENGTH_MILISEC = 100;
    const int SKIP_SYNC_INTERVAL_SECONDS = 10;

  public:
    SolenoidValve(int enablingGpio, int signalGpio)
    : _skipSyncCheck(false),
    _enablingPort(enablingGpio),
    _signalPort(signalGpio),
    _flowMeter(nullptr),
    _solenoidPulseLength(SOLENOID_PULSE_LENGTH_MILISEC) {
      pinMode(_enablingPort, OUTPUT);
      digitalWrite(_enablingPort, LOW);
      pinMode(_signalPort, OUTPUT);
      digitalWrite(_signalPort, LOW);
      _isOpen = false;
    }
    
    ~SolenoidValve() {};

    bool isValveOpen() {return _isOpen;}
    void openValve(void);
    void closeValve(void);
    void setSolenoidPulseLengthMiliSec(int length) {_solenoidPulseLength = length;}
    int getSolenoidPulseLengthMiliSec(void) {return _solenoidPulseLength;}
    void resetOpenStatus(bool open = false) {
      _isOpen = open;
    }
    void setFlowMeter(FlowMeter *fm) {
      _flowMeter = fm;
    }
    void run(void);

  public:
    bool _skipSyncCheck;

  protected:
    int _enablingPort;
    int _signalPort;
    FlowMeter *_flowMeter;
    int _solenoidPulseLength;
    Ticker _ticker;
  
  protected:
    void _activateSolenoid() {
      digitalWrite(_signalPort, digitalRead(_signalPort) == LOW ? HIGH : LOW);
      digitalWrite(_enablingPort, HIGH);
      delay(_solenoidPulseLength);
      digitalWrite(_enablingPort, LOW);
    }
  private:
    friend void skipCheckTime();

};

#endif // VALVES_H
