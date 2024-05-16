#include "Fsr.h"
#include <Keyboard.h>

Fsr::Fsr(int fsrPin, int minReading, int maxReading, int minDelay, int maxDelay) {
  this->fsrPin = fsrPin;
  this->minReading = minReading;
  this->maxReading = maxReading;
  this->minDelay = minDelay;
  this->maxDelay = maxDelay;
  previousMillis = 0;
  currentMillis = millis();
}

int Fsr::fsrReading() {
  int fsrReading = analogRead(fsrPin);
  return fsrReading;
}

bool Fsr::checkMinReading() {
  if (fsrReading() > minReading)
  {
    return true;
  }
  else {return false;}
}

int Fsr::getInterval() {
  int interval = map(fsrReading(), minReading, maxReading, maxDelay, minDelay);
  return interval;
}

void Fsr::Write(char key)
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= getInterval())
  {
    Keyboard.write(key);
    previousMillis = currentMillis;
  }
}