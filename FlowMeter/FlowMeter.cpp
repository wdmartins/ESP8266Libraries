#include <Arduino.h>
#include <Ticker.h>
#include "FlowMeter.h"

// FlowMeter needs a global volatile variable, hence there could be only one instance
// per module. TODO: Support one interrupt per available pin. Make class a singleton
Ticker ticker;
volatile unsigned int pulsesCounter;
unsigned int lastRead;

void countPulses(void) {
  pulsesCounter++;
}

void checkFlow(FlowMeter *flowMeter) {
    // Serial.print("CheckFlow: pulsesCounter: "); Serial.print(pulsesCounter); Serial.print(" lastRead: "); Serial.println(lastRead);
    flowMeter->setIsFlowing(pulsesCounter > lastRead + FlowMeter::MINIMUM_PULSES_BETWEEN_READ_TO_SET_FLOWING);
    lastRead = pulsesCounter;
}

FlowMeter::FlowMeter(int signalGpio, unsigned int calibrationFactor)
: _signalPort(signalGpio),
    _pulsesPerLiter(calibrationFactor),
    _isFluidFlowing(false),
    _litersCounted(0) {
    pulsesCounter = 0; 
}

void FlowMeter::start() {
    attachInterrupt(digitalPinToInterrupt(_signalPort), countPulses, RISING);
    ticker.attach_ms(TICKER_LENGTH_MILISEC, checkFlow, this);
}

void FlowMeter::restart() {
    pulsesCounter = 0;
    _litersCounted = 0;
}

unsigned int FlowMeter::getCountedLiters(bool restart) { 
    unsigned int liters = pulsesCounter / _pulsesPerLiter;
    pulsesCounter = restart ? 0 : pulsesCounter;
    return _litersCounted + liters;
}

bool FlowMeter::isFlowing() {
    return _isFluidFlowing;
}

void FlowMeter::setIsFlowing(bool isFlowing) {
    // Serial.print("setIsFlowing: "); Serial.println(isFlowing);
    _isFluidFlowing = isFlowing;
}

void FlowMeter::run(void) {
    if (pulsesCounter > TRANSLATE_PULSES_TO_LITERS_AND_RESET) {
        _litersCounted += (pulsesCounter / _pulsesPerLiter);
        pulsesCounter = 0;
    }
}

