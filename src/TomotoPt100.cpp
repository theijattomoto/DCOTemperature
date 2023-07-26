#include "TomotoPt100.h"

TomotoPT100::TomotoPT100(int pin)
{
  analogPin = pin;
}

void TomotoPT100::getAnalogRead()
{
  adcValue = analogRead(analogPin);
}

float TomotoPT100::getVoltage()
{
  Voltage = ((adcValue*3.30)/4095);
  return(Voltage);
}

float TomotoPT100::printADCvalue()
{
  //Serial.println(adcValue);
  return(adcValue);
}

void TomotoPT100::getPT100Reading()
{
  //PT100Resistor = (Voltage*132)/(3.3-Voltage);
  PT100Resistor = (Voltage*120)/(3.26-Voltage);
  Temperature = ((PT100Resistor-100)/(100*0.00391));
}

float TomotoPT100::getTemperatureReadingAverage(int averageSize)
{
  sumPt100Readings =0.00;
  for(int i =0; i<averageSize; i++)
  {
    adcValue = analogRead(analogPin);
    Voltage = ((adcValue*3.3)/4095);
    //Serial.println(Voltage);
    PT100Resistor = (Voltage*134)/(3.3-Voltage);
    Temperature = ((PT100Resistor-100)/(100*0.00391));
    /*
    ADC1.getAnalogRead();
    ADC1.getVoltage();
    ADC1.getPT100Reading();
    */
    delay(10);
    //temperatureReading = ADC1.getTemperatureValue();
    temperatureReading = Temperature;

    sumPt100Readings += temperatureReading;
  }

  tReadingAverage = sumPt100Readings / averageSize;
  Serial.print("Temperature Average = ");
  Serial.println(tReadingAverage);
  return(tReadingAverage);
}

float TomotoPT100::getTemperatureValue()
{
  return(Temperature);
}

float TomotoPT100::getPT100ResistorValue()
{
  return(PT100Resistor);
}