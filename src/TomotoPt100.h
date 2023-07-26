#ifndef TomotoPT100_h
#define TomotoPT100_h

#include <Arduino.h>

class TomotoPT100 {
  private:
    int analogPin;
    float adcValue = 0.00;
    float Voltage = 0.00;
    float Temperature = 0.00;
    float PT100Resistor = 0.00;

    float sumPt100Readings = 00.00;
    float temperatureReading =0.00;
    float tReadingAverage = 0.00;

  public:
    TomotoPT100(int pin);
    void getAnalogRead();
    void getPT100Reading();
    float printADCvalue();
    float getVoltage();
    float getTemperatureValue();
    float getPT100ResistorValue();
    float getTemperatureReadingAverage(int averageSize);
    
};

#endif