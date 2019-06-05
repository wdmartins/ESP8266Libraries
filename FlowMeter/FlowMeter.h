/*
 * FlowMeter.h - Library to abstract flow meter sensor
 * Created by Walter D. Martins, May 4, 2019
 */
#ifndef FLOW_METER_H
#define FLOW_METER_H

class FlowMeter {
  public:
    const static uint8_t MINIMUM_PULSES_BETWEEN_READ_TO_SET_FLOWING = 5;

  private: 
    const static uint16_t DEFAULT_FACTOR_PULSES_PER_LITER = 350;
    const static uint16_t TICKER_LENGTH_MILISEC = 500;
    const static uint16_t TRANSLATE_PULSES_TO_LITERS_AND_RESET =  (uint16_t) (DEFAULT_FACTOR_PULSES_PER_LITER * 1000);

  public:
    FlowMeter(uint8_t signalGpio, uint16_t calibrationFactor = DEFAULT_FACTOR_PULSES_PER_LITER);
    ~FlowMeter() {}

    void start(void);
    void restart(void);
    uint16_t getCountedLiters(bool restart = false);
    void setIsFlowing(bool isFlowing);
    bool isFlowing();
    void run();

  protected:
    uint8_t _signalPort;
    uint16_t _pulsesPerLiter;
    bool _isFluidFlowing;
    uint16_t _litersCounted;

  private:
    friend void countPulses(FlowMeter *);
};

#endif // FLOW_METER_H
