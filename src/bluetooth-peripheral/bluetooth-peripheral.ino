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

byte pole, direction, angle;
bool poleChanged = false;
bool directionChanged = false;
bool angleChanged = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

BLEService accelService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Bluetooth® Low Energy Accelerometer Service

// Bluetooth® Low Energy Accelerometer Characteristic - custom 128-bit UUID, read and writable by central
// BLEByteCharacteristic accelCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);
BLEByteCharacteristic poleCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1215", BLERead | BLEWrite);
BLEByteCharacteristic directionCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1216", BLERead | BLEWrite);
BLEByteCharacteristic angleCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1217", BLERead | BLEWrite);

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
	accelService.addCharacteristic(angleCharacteristic);

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
			if (angleCharacteristic.written()) {
				angleCharacteristic.readValue(angle);
				angleChanged = true;
			}

			if (poleChanged && directionChanged && angleChanged) {

				/*
				Serial.print("Pole: ");
				Serial.print(pole);
				Serial.print(" Direction: ");
				Serial.print(direction);
				Serial.print(" Angle: ");
				Serial.println(angle);
				*/

				if (pole == NORTH && angle == 0) {
						fn_draw_line(0);
				} else {
					if (pole == EAST && angle == 0) {
						fn_draw_line(90);
					} else {
						if (direction == UP) {
							fn_draw_up(fn_get_nb_arrow(angle));
						} else {
							fn_draw_down(fn_get_nb_arrow(angle));
						}
					}
				}

				poleChanged = false;
				directionChanged = false;
				angleChanged = false;
			}
		}

		// when the central disconnects, print it out:
		Serial.print(F("Disconnected from central: "));
		Serial.println(central.address());
	}
}

int fn_get_nb_arrow(int angle) {
	if( angle >= 5 ) {
		return 5;
	} else {
		return angle;
	}
}

void fn_draw_up(int nb_arrow) {

	display.clearDisplay();
	switch (nb_arrow) {
		case 1:
			fn_draw_up_arrow(50);
			break;
		case 2:
			fn_draw_up_arrow(30);
			fn_draw_up_arrow(80);
			break;
		case 3:
			fn_draw_up_arrow(12);
			fn_draw_up_arrow(48);
			fn_draw_up_arrow(86);
			break;
		case 4:
			fn_draw_up_arrow(0);
			fn_draw_up_arrow(32);
			fn_draw_up_arrow(64);
			fn_draw_up_arrow(96);
			break;
		default:
			fn_draw_up_arrow(-24);
			fn_draw_up_arrow(0);
			fn_draw_up_arrow(24);
			fn_draw_up_arrow(48);
			fn_draw_up_arrow(72);
			fn_draw_up_arrow(96);
			fn_draw_up_arrow(120);
			break;
	}
	display.display();
}

void fn_draw_down(int nb_arrow) {

	display.clearDisplay();
	switch (nb_arrow) {
		case 1:
			fn_draw_down_arrow(65);
			break;
		case 2:
			fn_draw_down_arrow(45);
			fn_draw_down_arrow(95);
			break;
		case 3:
			fn_draw_down_arrow(34);
			fn_draw_down_arrow(72);
			fn_draw_down_arrow(110);
			break;
		case 4:
			fn_draw_down_arrow(24);
			fn_draw_down_arrow(56);
			fn_draw_down_arrow(88);
			fn_draw_down_arrow(120);
			break;
		default:
			fn_draw_down_arrow(0);
			fn_draw_down_arrow(24);
			fn_draw_down_arrow(48);
			fn_draw_down_arrow(72);
			fn_draw_down_arrow(96);
			fn_draw_down_arrow(120);
			fn_draw_down_arrow(144);
			break;
	}
	display.display();
}

void fn_draw_up_arrow(int coord) {
	for (int i = 0; i < 8; i++) {
		display.drawLine(coord, 0, (coord + 24), 15, WHITE);
		display.drawLine((coord + 24), 15, coord, 31, WHITE);
		coord = coord + 1;
	}
}

void fn_draw_down_arrow(int coord) {
	for (int i = 0; i < 8; i++) {
		display.drawLine(coord, 0, (coord - 24), 15, WHITE);
		display.drawLine((coord - 24), 15, coord, 31, WHITE);
		coord = coord + 1;
	}
}

void fn_draw_line(int value) {

	display.clearDisplay();
	switch (value) {
			case 0:
		case 180:
		case 360:
			display.drawLine(63, 0, 63, 31, WHITE);
			display.drawLine(64, 0, 64, 31, WHITE);
			display.drawLine(65, 0, 65, 31, WHITE);
			display.drawLine(66, 0, 66, 31, WHITE);
			display.drawLine(67, 0, 67, 31, WHITE);
			break;
		case 90:
		case 270:
			display.drawLine(24, 15, 104, 15, WHITE);
			display.drawLine(24, 16, 104, 16, WHITE);
			display.drawLine(24, 17, 104, 17, WHITE);
			break;
	}
	display.display();
}
