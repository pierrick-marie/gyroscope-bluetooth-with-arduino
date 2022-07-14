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
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

BLEService accelService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Bluetooth® Low Energy Accelerometer Service

// Bluetooth® Low Energy Accelerometer Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic accelCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

// const int ledPin = LED_BUILTIN; // pin to use for the LED
int accelValue = 0;

void setup() {

    Serial.begin(9600);

    // set LED pin to output mode
    // pinMode(ledPin, OUTPUT);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
          Serial.println(F("SSD1306 allocation failed"));
          for(;;); // Don't proceed, loop forever
      }
      // Clear the buffer
      display.clearDisplay();

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
              accelValue = accelCharacteristic.value();

              accelValue = accelValue - 90;
              // display.print("+");

              Serial.print("Accel value = ");
              Serial.println(accelValue);

              display.clearDisplay();
              display.setTextSize(5);      // Normal 1:5 pixel scale
              display.setTextColor(SSD1306_WHITE); // Draw white text
              display.setCursor(18, 0);     // Start at x:18 y:0
              display.cp437(true);         // Use full 256 char 'Code Page 437' font

              if (accelValue < 0) {
                  display.print("-");
                  accelValue = abs(accelValue);
              } else {
                  display.print("+");
              }
              display.print(accelValue);
              display.display();
          }
      }

      // when the central disconnects, print it out:
      Serial.print(F("Disconnected from central: "));
      Serial.println(central.address());
    }
}
