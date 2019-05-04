/*
 * FlowMeter.h - Library to abstract hidraulic valves
 * Created by Walter D. Martins, May 4, 2019
 */

#ifndef VALVES_H
#define VALVES_H

// Pure virtual Valve class. It can only be open or close.
class Valve {
  public:
    virtual void openValve(void) = 0;
    void closeValve(void);
    bool isValveOpen();
   
  protected:
    bool isOpen;          
};

// Solenoid Valve class. Solenoid Valve abstracts H bridge that open/close the valve
// Open/Close status of valve cannot be guaranteed without fluid flow feedback.
// Class assume valve is close upon instantiation. H bridge needs an enable port
// and a signal port which is feed into one control of the H bridge, and to an inverted
// gate the feeds the other signal port.
class SolenoidValve: public Valve {
  private:
    const int SOLENOID_PULSE_LENGTH_MILISEC = 100;
  public:
    SolenoidValve(int enablingGpio, int signalGpio)
    : enablingPort(enablingGpio),
    signalPort(signalGpio),
    solenoidPulseLength(SOLENOID_PULSE_LENGTH_MILISEC) {
      pinMode(enablingPort, OUTPUT);
      pinMode(signalPort, OUTPUT);
      digitalWrite(enablingPort, LOW);
      digitalWrite(signalPort, LOW);
      isOpen = false;
    }
    
    ~SolenoidValve() {};

    bool isValveOpen() {return isOpen;}
    void openValve() {
      if (isOpen) {
        Serial.println("Valve is already open");
        return;
      }
      activateSolenoid();
      isOpen = true;
    }
    void closeValve() {
      if (!isOpen) {
        Serial.println("Valve is already close");
        return;
      }
      activateSolenoid();
      isOpen = false;
    }
    void setSolenoidPulseLengthMiliSec(int length) {solenoidPulseLength = length;}
    int getSolenoidPulseLengthMiliSec(void) {return solenoidPulseLength;}
    
  protected:
    int enablingPort;
    int signalPort;
    int solenoidPulseLength;
  
  protected:
    void activateSolenoid() {
      digitalWrite(signalPort, digitalRead(signalPort) == LOW ? HIGH : LOW);
      digitalWrite(enablingPort, HIGH);
      delay(solenoidPulseLength);
      digitalWrite(enablingPort, LOW);
    }
};

#endif VALVES_H
