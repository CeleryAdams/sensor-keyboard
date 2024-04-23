#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

const int buttonPin = 2;

int buttonState = 0;

//initial key input states per sensor
bool buttonOn = true;
bool accelOn = false;

//instantiate accelerometer
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345, &Wire1);

void setup() {
  
  Serial.begin(9600);

  //initialize accelerometer
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

  accel.setRange(ADXL343_RANGE_8_G);

  pinMode(buttonPin, INPUT);
}

void loop() {
  //get button input state
  buttonState = digitalRead(buttonPin);

  //get accelerometer sensor event
  sensors_event_t event;
  accel.getEvent(&event);

  //accelerometer key input
  if (accelOn)
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

  //button key input
  if (buttonState == HIGH && buttonOn)
  {
    Keyboard.write('B');
  }
  
  
  //keyboard function on/off events received from Unity
  switch(Serial.read())
  {
    case 'A':
      Serial.println("Received A, Accelerometer On");
      accelOn = true;
      break;
    case 'Z':
      Serial.println("Received Z, Accelerometer Off");
      accelOn = false;
      break;
    case 'O':
      Serial.println("Received O, Keyboard On");
      buttonOn = true;
      break;
    case 'F':
      Serial.println("Received F, Keyboard Off");
      buttonOn = false;
      break;
  }
}
