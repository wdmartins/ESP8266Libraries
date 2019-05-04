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
    flowMeter->setIsFlowing(pulsesCounter > lastRead + 10);
    lastRead = pulsesCounter;
}

FlowMeter::FlowMeter(int signalGpio, unsigned int calibrationFactor)
: signalPort(signalGpio),
    pulsesPerLiter(calibrationFactor),
    isFluidFlowing(false),
    litersCounted(0) {
    pulsesCounter = 0; 
}

void FlowMeter::start() {
    attachInterrupt(digitalPinToInterrupt(signalPort), countPulses, RISING);
    ticker.attach_ms(TICKER_LENGTH_MILISEC, checkFlow, this);
}

void FlowMeter::restart() {
    pulsesCounter = 0;
    litersCounted = 0;
}

unsigned int FlowMeter::getCountedLiters(bool restart) { 
    unsigned int liters = pulsesCounter / pulsesPerLiter;
    pulsesCounter = restart ? 0 : pulsesCounter;
    return litersCounted + liters;
}

bool FlowMeter::isFlowing() {
    return isFluidFlowing;
}

void FlowMeter::setIsFlowing(bool isFlowing) {
    isFluidFlowing = isFlowing;
}

void FlowMeter::run(void) {
    if (pulsesCounter > TRANSLATE_PULSES_TO_LITERS_AND_RESET) {
        litersCounted += (pulsesCounter / pulsesPerLiter);
        pulsesCounter = 0;
    }
}

