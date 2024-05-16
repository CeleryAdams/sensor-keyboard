#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include "Fsr.h"

const int buttonPin = 2;
const int fsr1Pin = A4;
const int fsr2Pin = A5;
const int hallPin = 13;

//initial key input states per sensor
bool buttonOn = true;
bool tiltOn = false;
bool fsrOn = false;
bool bounceOn = false;
bool hallOn = false;

int buttonState = 0;
int hallReading;

//instantiate accelerometer
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345, &Wire1);

//instantiate force sensitive resistors
Fsr fsr1(fsr1Pin, 200, 800);
Fsr fsr2(fsr2Pin, 300, 900);

void setup() {
  
  Serial.begin(9600);
  while (!Serial);

  // initialize accelerometer
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

  accel.setRange(ADXL343_RANGE_4_G);

  pinMode(buttonPin, INPUT);
  pinMode(hallPin, INPUT);
}

void loop() {
  //get button input state
  buttonState = digitalRead(buttonPin);
  hallReading = digitalRead(hallPin);

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

  //hall effect key input
  if (hallOn && hallReading == LOW)
  {
    Keyboard.write('H');
    delay(800);
  }

  //fsr key input, enabled if value read from sensor is greater than minimum threshold set during instantiation
  if (fsrOn)
  {
    if(fsr2.checkMinReading()){
      fsr2.Write('G');
    }
  
    if(fsr1.checkMinReading()){
      fsr1.Write('S');
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
      Serial.println("Received O, Button On");
      buttonOn = true;
      break;
    case 'F':
      Serial.println("Received F, Button Off");
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
    case 'I':
      Serial.println("Received I, Hall Effect On");
      hallOn = true;
      break;
    case 'K':
      Serial.println("Received K, Hall Effect Off");
      hallOn = false;
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
