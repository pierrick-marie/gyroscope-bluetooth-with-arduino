/*
    Accelerometer Control

    This program scans for Bluetooth® Low Energy peripherals until one with the advertised service
    "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
    it will remotely control the Bluetooth® Low Energy peripheral's Accelerometer.

    The circuit:
      - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT, Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  
    Licence
      DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                   Version 2, December 2004

      Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

      Everyone is permitted to copy and distribute verbatim or modified
      copies of this license document, and changing it is allowed as long
      as the name is changed.

      DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
      TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

      0. You just DO WHAT THE FUCK YOU WANT TO.

*/

#include <ArduinoBLE.h>

BLEService accelService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Bluetooth® Low Energy Accelerometer Service

// Bluetooth® Low Energy Accelerometer Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic accelCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED
int accelValue = 0;

void setup() {
   
    Serial.begin(9600);

    // set LED pin to output mode
    pinMode(ledPin, OUTPUT);

    // begin initialization
    if (!BLE.begin()) {
      Serial.println("starting Bluetooth® Low Energy module failed!");
      while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("ACCELEROMETER");
    BLE.setAdvertisedService(accelService);

    // add the characteristic to the service
    accelService.addCharacteristic(accelCharacteristic);

    // add service
    BLE.addService(accelService);

    // set the initial value for the characeristic:
    accelCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();

    Serial.println("BLE Accelerometer Peripheral");
}

void loop() {
    // listen for Bluetooth® Low Energy peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
      Serial.print("Connected to central: ");
      // print the central's MAC address:
      Serial.println(central.address());

      // while the central is still connected to peripheral:
      while (central.connected()) {
          // if the remote device wrote to the characteristic,
          // use the value to control the LED:
          if (accelCharacteristic.written()) {
              Serial.println("Reading value");
              accelValue = accelCharacteristic.value();
              Serial.print("Accel value = ");
              Serial.println(accelValue);
              if (accelValue > 90) {   // any value upper than 90
                  Serial.println("LED on");
                  digitalWrite(ledPin, HIGH);         // will turn the LED on
              } else {                              // a value less or equal than 90
                  Serial.println(F("LED off"));
                  digitalWrite(ledPin, LOW);          // will turn the LED off
              }
          }
      }

      // when the central disconnects, print it out:
      Serial.print(F("Disconnected from central: "));
      Serial.println(central.address());
    }
}
