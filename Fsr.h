#ifndef FSR_H
#define FSR_H

#include <Arduino.h>

class Fsr {
  private: 
    int fsrPin;
    int minReading;
    int maxReading;
    int minDelay;
    int maxDelay;

  public:
    Fsr(int fsrPin, int minReading, int maxReading, int minDelay = 100, int maxDelay = 800);
    int fsrReading();
    bool checkInterval();
    int getInterval();
};

#endif