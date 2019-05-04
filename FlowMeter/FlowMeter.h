/*
 * FlowMeter.h - Library to abstract flow meter sensor
 * Created by Walter D. Martins, May 4, 2019
 */
#ifndef FLOW_METER_H
#define FLOW_METER_H

class FlowMeter {
  private: 
    const static unsigned int DEFAULT_FACTOR_PULSES_PER_LITER = 450;
    const static unsigned int TICKER_LENGTH_MILISEC = 500;
    const static unsigned int TRANSLATE_PULSES_TO_LITERS_AND_RESET = DEFAULT_FACTOR_PULSES_PER_LITER * 1000;

  public:
    FlowMeter(int signalGpio, unsigned int calibrationFactor = DEFAULT_FACTOR_PULSES_PER_LITER);
    ~FlowMeter() {}

    void start(void);
    void restart(void);
    unsigned int getCountedLiters(bool restart = false);
    void setIsFlowing(bool isFlowing);
    bool isFlowing();
    void run();

  protected:
    int signalPort;
    unsigned int pulsesPerLiter;
    unsigned int litersCounted;
    bool isFluidFlowing;

  private:
    friend void countPulses(FlowMeter *);
};

#endif FLOW_METER_H