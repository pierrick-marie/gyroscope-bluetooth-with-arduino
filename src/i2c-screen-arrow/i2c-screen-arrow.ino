/*
 *    Accelerometer Control
 *
 *    This program scans for Bluetooth® Low Energy peripherals until one with the advertised service
 *    "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
 *    it will remotely control the Bluetooth® Low Energy peripheral's Accelerometer.
 *
 *    The circuit:
 *      - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT, Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
 *
 *    Licence
 *      DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                   Version 2, December 2004
 *
 *      Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 *      Everyone is permitted to copy and distribute verbatim or modified
 *      copies of this license document, and changing it is allowed as long
 *      as the name is changed.
 *
 *      DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *      TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *      0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define WAIT_DELAY 1000

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define NORTH 0
#define EAST 1
#define UP 0
#define DOWN 1

byte pole, direction, value;
bool poleChanged = false;
bool directionChanged = false;
bool valueChanged = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
}

void loop() {
	fn_draw_up(1);
	delay(2000);
	fn_draw_up(2);
	delay(2000);
	fn_draw_up(3);
	delay(2000);
	fn_draw_up(4);
	delay(2000);
	fn_draw_up(5);
	delay(2000);
	fn_draw_down(1);
	delay(2000);
	fn_draw_down(2);
	delay(2000);
	fn_draw_down(3);
	delay(2000);
	fn_draw_down(4);
	delay(2000);
	fn_draw_down(5);
	delay(2000);
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
