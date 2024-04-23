#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include "Fsr.h"

const int buttonPin = 2;

int buttonState = 0;

const int fsr1Pin = A4;
const int fsr2Pin = A5;

//initial key input states per sensor
bool buttonOn = true;
bool tiltOn = false;
bool fsrOn = false;
bool bounceOn = true;

//instantiate accelerometer
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345, &Wire1);

//instantiate force sensitive resistors
Fsr fsr1(fsr1Pin, 200, 800, 100, 800);
Fsr fsr2(fsr2Pin, 300, 900, 100, 800);

void setup() {
  
  Serial.begin(9600);

  //initialize accelerometer
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

  accel.setRange(ADXL343_RANGE_4_G);

  pinMode(buttonPin, INPUT);
}

void loop() {
  //get button input state
  buttonState = digitalRead(buttonPin);

  //get accelerometer sensor event
  sensors_event_t event;
  accel.getEvent(&event);

  //accelerometer key input when tilted along X-axis
  if (tiltOn)
  {
    if (event.acceleration.x > 3.0)
    {
      Keyboard.write('C');
      delay(300);
    }

    if (event.acceleration.x < -2.0)
    {
      Keyboard.write('X');
      delay(300);
    }
  }

  //accelerometer key input when bounced along Z-axis
  if (bounceOn)
  {
    bool didBounce = detectBounce(event, 12.0);
    if (didBounce)
    {
      Keyboard.write('G');
      delay(400);
    }
  }

  //button key input
  if (buttonOn && buttonState == HIGH)
  {
    Keyboard.write('B');
  }

  //fsr key input, interval between keypresses decreases with increased pressure
  if (fsrOn)
  {
    if(fsr2.checkInterval()){
      Keyboard.write('N');
      delay(fsr2.getInterval());
    }
  
    if(fsr1.checkInterval()){
      Keyboard.write('M');
      delay(fsr1.getInterval());
    }
  }
  
  
  //keyboard function on/off events received from Unity
  switch(Serial.read())
  {
    case 'A':
      Serial.println("Received A, Tilt On");
      tiltOn = true;
      break;
    case 'Z':
      Serial.println("Received Z, Tilt Off");
      tiltOn = false;
      break;
    case 'T':
      Serial.println("Received T, Bounce On");
      bounceOn = true;
      break;
    case 'Y':
      Serial.println("Received Y, Bounce Off");
      bounceOn = false;
    case 'O':
      Serial.println("Received O, Keyboard On");
      buttonOn = true;
      break;
    case 'F':
      Serial.println("Received F, Keyboard Off");
      buttonOn = false;
      break;
    case 'Q':
      Serial.println("Received Q, FSR On");
      fsrOn = true;
      break;
    case 'W':
      Serial.println("Received W, FSR Off");
      fsrOn = false;
      break;
    case '/':
      Serial.println("Received /, all sensors off");
      buttonOn = false;
      tiltOn = false;
      fsrOn = false;
      bounceOn = false;
      break;
  }
}

//check for Z axis movement
bool detectBounce(sensors_event_t event, float threshold)
{
  float Z = 0;

  for (int i=0; i<10; i++)
  {
    Z += event.acceleration.z;
    delay(1);
  }

  Z /= 10;
  float totalAccel = abs(Z);

  if (totalAccel > threshold)
  {
    return true;
  } else {
    return false;
  }
}
