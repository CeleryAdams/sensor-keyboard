#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include "Fsr.h"

const int buttonPin = 2;
const int fsr1Pin = A4;
const int fsr2Pin = A5;
const int hallPin = A3;

//initial key input states per sensor
bool buttonOn = true;
bool tiltOn = false;
bool fsrOn = false;
bool bounceOn = false;
bool hallOn = false;

int buttonState = 0;
int initialHallValue;
int hallValue;
const int minHallValue = 15;

//instantiate accelerometer
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345, &Wire1);

//instantiate force sensitive resistors and set read value thresholds (fsrPin, minReading, maxReading)
Fsr fsr1(fsr1Pin, 200, 800);
Fsr fsr2(fsr2Pin, 300, 900);



/*
class that creates a new timer and assigns a key press for each sensor
*/
class TimedWrite
{
  int delay;
  char key;
  unsigned long previousMillis;
  unsigned long currentMillis;

  public:
  TimedWrite(int delay, char key)
  {
    this->delay = delay;
    this->key = key;

    previousMillis = 0;
    currentMillis = millis();
  }

  void Update()
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= delay)
    {
      Keyboard.write(key);
      previousMillis = currentMillis;
    }
  }
};

//set keys and delays for each sensor
TimedWrite buttonWrite(300, 'G');
TimedWrite tiltLeftWrite(300, 'C');
TimedWrite tiltRightWrite(300, 'X');
TimedWrite bounceWrite(400, 'B');
TimedWrite hallWrite(800, 'H');

void setup() {
  
  Serial.begin(9600);
  while (!Serial);

  // initialize accelerometer
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

  accel.setRange(ADXL343_RANGE_4_G);

  pinMode(buttonPin, INPUT);
  initialHallValue = analogRead(hallPin);
}

void loop() {
  //get button input state
  buttonState = digitalRead(buttonPin);
  hallValue = analogRead(hallPin);

  //get accelerometer sensor event
  sensors_event_t event;
  accel.getEvent(&event);

  //accelerometer key input when tilted along X-axis
  if (tiltOn)
  {
    if (event.acceleration.x > 3.0)
    {
      tiltLeftWrite.Update();
    }

    if (event.acceleration.x < -2.0)
    {
      tiltRightWrite.Update();
    }
  }

  //accelerometer key input when bounced along Z-axis
  if (bounceOn)
  {
    bool didBounce = detectBounce(event, 12.0);
    if (didBounce)
    {
      bounceWrite.Update();
    }
  }

  //button key input
  if (buttonOn && buttonState == HIGH)
  {
    buttonWrite.Update();
  }

  //hall effect key input
  if (hallOn)
  {
     if (abs(initialHallValue - hallValue) > minHallValue)
    {
      hallWrite.Update();
    } 
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
      hallOn = false;
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
