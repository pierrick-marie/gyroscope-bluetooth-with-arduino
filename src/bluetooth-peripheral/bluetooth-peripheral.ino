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

const int COORD_00_1[] = {64, 0, 64, 31};
const int COORD_00_2[] = {65, 0, 65, 31};
const int COORD_10_1[] = {72, 0, 56, 31};
const int COORD_10_2[] = {73, 0, 57, 31};
const int COORD_20_1[] = {80, 0, 48, 31};
const int COORD_20_2[] = {81, 0, 49, 31};
const int COORD_30_1[] = {90, 0, 38, 31};
const int COORD_30_2[] = {91, 0, 39, 31};
const int COORD_40_1[] = {100, 0, 28, 31};
const int COORD_40_2[] = {101, 0, 29, 31};
const int COORD_50_1[] = {24, 28, 104, 3};
const int COORD_50_2[] = {24, 29, 104, 4};
const int COORD_60_1[] = {24, 24, 104, 7};
const int COORD_60_2[] = {24, 25, 104, 8};
const int COORD_70_1[] = {24, 21, 104, 10};
const int COORD_70_2[] = {24, 22, 104, 11};
const int COORD_80_1[] = {24, 19, 104, 13};
const int COORD_80_2[] = {24, 20, 104, 12};
const int COORD_90_1[] = {24, 16, 104, 16};
const int COORD_90_2[] = {24, 17, 104, 17};
const int COORD_100_1[] = {24, 13, 104, 19};
const int COORD_100_2[] = {24, 12, 104, 20};
const int COORD_110_1[] = {24, 10, 104, 21};
const int COORD_110_2[] = {24, 11, 104, 22};
const int COORD_120_1[] = {24, 7, 104, 24};
const int COORD_120_2[] = {24, 8, 104, 25};
const int COORD_130_1[] = {24, 3, 104, 28};
const int COORD_130_2[] = {24, 4, 104, 29};
const int COORD_140_1[] = {28, 0, 100, 31};
const int COORD_140_2[] = {29, 0, 101, 31};
const int COORD_150_1[] = {38, 0, 90, 31};
const int COORD_150_2[] = {39, 0, 91, 31};
const int COORD_160_1[] = {48, 0, 80, 31};
const int COORD_160_2[] = {49, 0, 81, 31};
const int COORD_170_1[] = {56, 0, 72, 31};
const int COORD_170_2[] = {57, 0, 73, 31};

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
				accelValue = accelValue * 10;
				Serial.print("Accel value = ");
				Serial.println(accelValue);
				fn_draw_line(accelValue);
			}
		}

		// when the central disconnects, print it out:
		Serial.print(F("Disconnected from central: "));
		Serial.println(central.address());
	}
}

void fn_draw_line(int value) {

	display.clearDisplay();
	switch (value) {
  		case 0:
		case 180:
		case 360:
            	display.drawLine(COORD_00_1[0], COORD_00_1[1], COORD_00_1[2], COORD_00_1[3], WHITE);
            	display.drawLine(COORD_00_2[0], COORD_00_2[1], COORD_00_2[2], COORD_00_2[3], WHITE);
            	break;
		case 10:
		case 190:
			display.drawLine(COORD_10_1[0], COORD_10_1[1], COORD_10_1[2], COORD_10_1[3], WHITE);
			display.drawLine(COORD_10_2[0], COORD_10_2[1], COORD_10_2[2], COORD_10_2[3], WHITE);
			break;
		case 20:
		case 200:
			display.drawLine(COORD_20_1[0], COORD_20_1[1], COORD_20_1[2], COORD_20_1[3], WHITE);
			display.drawLine(COORD_20_2[0], COORD_20_2[1], COORD_20_2[2], COORD_20_2[3], WHITE);
			break;
		case 30:
		case 210:
			display.drawLine(COORD_30_1[0], COORD_30_1[1], COORD_30_1[2], COORD_30_1[3], WHITE);
			display.drawLine(COORD_30_2[0], COORD_30_2[1], COORD_30_2[2], COORD_30_2[3], WHITE);
			break;
		case 40:
		case 220:
			display.drawLine(COORD_40_1[0], COORD_40_1[1], COORD_40_1[2], COORD_40_1[3], WHITE);
			display.drawLine(COORD_40_2[0], COORD_40_2[1], COORD_40_2[2], COORD_40_2[3], WHITE);
			break;
		case 50:
		case 230:
			display.drawLine(COORD_50_1[0], COORD_50_1[1], COORD_50_1[2], COORD_50_1[3], WHITE);
			display.drawLine(COORD_50_2[0], COORD_50_2[1], COORD_50_2[2], COORD_50_2[3], WHITE);
			break;
		case 60:
		case 240:
			display.drawLine(COORD_60_1[0], COORD_60_1[1], COORD_60_1[2], COORD_60_1[3], WHITE);
			display.drawLine(COORD_60_2[0], COORD_60_2[1], COORD_60_2[2], COORD_60_2[3], WHITE);
			break;
		case 70:
		case 250:
			display.drawLine(COORD_70_1[0], COORD_70_1[1], COORD_70_1[2], COORD_70_1[3], WHITE);
			display.drawLine(COORD_70_2[0], COORD_70_2[1], COORD_70_2[2], COORD_70_2[3], WHITE);
			break;
		case 80:
		case 260:
			display.drawLine(COORD_80_1[0], COORD_80_1[1], COORD_80_1[2], COORD_80_1[3], WHITE);
			display.drawLine(COORD_80_2[0], COORD_80_2[1], COORD_80_2[2], COORD_80_2[3], WHITE);
			break;
		case 90:
		case 270:
			display.drawLine(COORD_90_1[0], COORD_90_1[1], COORD_90_1[2], COORD_90_1[3], WHITE);
			display.drawLine(COORD_90_2[0], COORD_90_2[1], COORD_90_2[2], COORD_90_2[3], WHITE);
			break;
		case 100:
		case 280:
			display.drawLine(COORD_100_1[0], COORD_100_1[1], COORD_100_1[2], COORD_100_1[3], WHITE);
			display.drawLine(COORD_100_2[0], COORD_100_2[1], COORD_100_2[2], COORD_100_2[3], WHITE);
			break;
		case 110:
		case 290:
			display.drawLine(COORD_110_1[0], COORD_110_1[1], COORD_110_1[2], COORD_110_1[3], WHITE);
			display.drawLine(COORD_110_2[0], COORD_110_2[1], COORD_110_2[2], COORD_110_2[3], WHITE);
			break;
		case 120:
		case 300:
			display.drawLine(COORD_120_1[0], COORD_120_1[1], COORD_120_1[2], COORD_120_1[3], WHITE);
			display.drawLine(COORD_120_2[0], COORD_120_2[1], COORD_120_2[2], COORD_120_2[3], WHITE);
			break;
		case 130:
		case 310:
			display.drawLine(COORD_130_1[0], COORD_130_1[1], COORD_130_1[2], COORD_130_1[3], WHITE);
			display.drawLine(COORD_130_2[0], COORD_130_2[1], COORD_130_2[2], COORD_130_2[3], WHITE);
			break;
		case 140:
		case 320:
			display.drawLine(COORD_140_1[0], COORD_140_1[1], COORD_140_1[2], COORD_140_1[3], WHITE);
			display.drawLine(COORD_140_2[0], COORD_140_2[1], COORD_140_2[2], COORD_140_2[3], WHITE);
			break;
		case 150:
		case 330:
			display.drawLine(COORD_150_1[0], COORD_150_1[1], COORD_150_1[2], COORD_150_1[3], WHITE);
			display.drawLine(COORD_150_2[0], COORD_150_2[1], COORD_150_2[2], COORD_150_2[3], WHITE);
			break;
		case 160:
		case 340:
			display.drawLine(COORD_160_1[0], COORD_160_1[1], COORD_160_1[2], COORD_160_1[3], WHITE);
			display.drawLine(COORD_160_2[0], COORD_160_2[1], COORD_160_2[2], COORD_160_2[3], WHITE);
			break;
		case 170:
		case 350:
			display.drawLine(COORD_170_1[0], COORD_170_1[1], COORD_170_1[2], COORD_170_1[3], WHITE);
			display.drawLine(COORD_170_2[0], COORD_170_2[1], COORD_170_2[2], COORD_170_2[3], WHITE);
			break;
		default: // Draw vertical line
			display.drawLine(COORD_00_1[0], COORD_00_1[1], COORD_00_1[2], COORD_00_1[3], WHITE);
			display.drawLine(COORD_00_2[0], COORD_00_2[1], COORD_00_2[2], COORD_00_2[3], WHITE);
			break;
	}
	display.display();
}
