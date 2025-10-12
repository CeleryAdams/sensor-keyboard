# Sensor Keyboard

An Arduino-based keyboard controller that uses various sensors to trigger keyboard inputs. Control which sensors are active at runtime through serial communication with your computer.

## Overview
This project turns an Arduino into a customizable game controller that can use different sensors (pressure sensors, accelerometers, etc.) as input devices. You can dynamically enable or disable sensors during gameplay by sending commands from your computer.

## Features
*Convert sensor readings into keyboard inputs
*Runtime control via serial communication
*Support for multiple sensor types
*Compatible with any Arduino board with HID support
*Tested with Unity using [Ardity](https://github.com/dwilches/Ardity).

This particular configuration uses a button, an accelerometer, 2 force-sensing resistors, and a hall effect sensor. The sensors were placed inside a cat plushie, which could be bounced, squeezed, and fed \(by placing a magnetic object near the hall effect sensor in its nose\). Keypresses sent from the sensors to the computer are used as controls in a custom game developed in Unity.