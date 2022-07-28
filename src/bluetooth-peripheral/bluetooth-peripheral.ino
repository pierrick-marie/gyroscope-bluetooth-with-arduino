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

#define WAIT_DELAY 1000

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define NORTH 0
#define EAST 1
#define UP 0
#define DOWN 1

byte pole, direction, value;
bool poleChanged = false;
bool directionChanged = false;
bool valueChanged = false;


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
// BLEByteCharacteristic accelCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);
BLEByteCharacteristic poleCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1215", BLERead | BLEWrite);
BLEByteCharacteristic directionCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1216", BLERead | BLEWrite);
BLEByteCharacteristic valueCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1217", BLERead | BLEWrite);

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
	accelService.addCharacteristic(poleCharacteristic);
	accelService.addCharacteristic(directionCharacteristic);
	accelService.addCharacteristic(valueCharacteristic);

	// add service
	BLE.addService(accelService);

	// set the initial value for the characeristic:
	// accelCharacteristic.writeValue(0);

	// start advertising
	BLE.advertise();

	Serial.println("BLE Accelerometer Peripheral");
}

void loop() {
	// listen for Bluetooth® Low Energy peripherals to connect:
	BLEDevice central = BLE.central();

	// const uint8_t* res;
	// if a central is connected to peripheral:
	if (central) {
		Serial.print("Connected to central: ");
		// print the central's MAC address:
		Serial.println(central.address());

		// while the central is still connected to peripheral:
		while (central.connected()) {
			// if the remote device wrote to the characteristic,
			// use the value to control the LED:
			if (poleCharacteristic.written()) {
				poleCharacteristic.readValue(pole);
				poleChanged = true;
			}
			if (directionCharacteristic.written()) {
				directionCharacteristic.readValue(direction);
				directionChanged = true;
			}
			if (valueCharacteristic.written()) {
				valueCharacteristic.readValue(value);
				valueChanged = true;
			}

			if (poleChanged && directionChanged && valueChanged) {

				Serial.print("Pole: ");
				Serial.println(pole);
				Serial.print("Direction: ");
				Serial.println(direction);
				Serial.print("Value: ");
				Serial.println(value);

				if (pole == NORTH && value <= 1) {
						fn_draw_line(0);
				} else {
					if (pole == EAST && value <= 1) {
						fn_draw_line(90);
					} else {
						if (direction == UP) {
							fn_draw_up_arrow(2);
						} else {
							fn_draw_down_arrow(2);
						}
					}
				}

				poleChanged = false;
				directionChanged = false;
				valueChanged = false;
			}
		}

		// when the central disconnects, print it out:
		Serial.print(F("Disconnected from central: "));
		Serial.println(central.address());
	}
}

void fn_draw_up_arrow(int nb_arrow) {

	display.clearDisplay();
	switch (nb_arrow) {
		case 1:
			display.drawLine(50, 0, 74, 15, WHITE);
			display.drawLine(74, 15, 50, 31, WHITE);
			display.drawLine(51, 0, 75, 15, WHITE);
			display.drawLine(75, 15, 51, 31, WHITE);
			break;
		case 2:
			display.drawLine(40, 0, 64, 15, WHITE);
			display.drawLine(64, 15, 40, 31, WHITE);
			display.drawLine(41, 0, 65, 15, WHITE);
			display.drawLine(65, 15, 41, 31, WHITE);

			display.drawLine(65, 0, 89, 15, WHITE);
			display.drawLine(89, 15, 65, 31, WHITE);
			display.drawLine(66, 0, 90, 15, WHITE);
			display.drawLine(90, 15, 66, 31, WHITE);
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
	}
	display.display();
}

void fn_draw_down_arrow(int nb_arrow) {

	display.clearDisplay();
	switch (nb_arrow) {
		case 1:
			display.drawLine(74, 0, 50, 15, WHITE);
			display.drawLine(50, 15, 74, 31, WHITE);
			display.drawLine(75, 0, 51, 15, WHITE);
			display.drawLine(51, 15, 75, 31, WHITE);
			break;
		case 2:
			display.drawLine(84, 0, 60, 15, WHITE);
			display.drawLine(60, 15, 84, 31, WHITE);
			display.drawLine(85, 0, 61, 15, WHITE);
			display.drawLine(61, 15, 85, 31, WHITE);

			display.drawLine(58, 0, 34, 15, WHITE);
			display.drawLine(34, 15, 58, 31, WHITE);
			display.drawLine(59, 0, 35, 15, WHITE);
			display.drawLine(35, 15, 59, 31, WHITE);
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
	}
	display.display();
}

void fn_draw_line(int value) {

	display.clearDisplay();
	switch (value) {
			case 0:
		case 180:
		case 360:
							display.drawLine(64, 0, 64, 31, WHITE);
							display.drawLine(65, 0, 65, 31, WHITE);
							break;
		case 90:
		case 270:
			display.drawLine(24, 16, 104, 16, WHITE);
			display.drawLine(24, 17, 104, 17, WHITE);
			break;
	}
	display.display();
}
