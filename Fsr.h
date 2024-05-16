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
    unsigned long previousMillis;
    unsigned long currentMillis;

  public:
    Fsr(int fsrPin, int minReading, int maxReading, int minDelay = 100, int maxDelay = 800);
    int fsrReading();
    bool checkMinReading();
    int getInterval();
    void Write(char key);
};

#endif