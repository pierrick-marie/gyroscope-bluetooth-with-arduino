/*
	Arduino LSM6DS3 - Accelerometer Application

	This example reads the acceleration values as relative direction and degrees,
	from the LSM6DS3 sensor and prints them to the Serial Monitor or Serial Plotter.

	The circuit:
	- Arduino Nano 33 IoT

	Created by Riccardo Rizzo

	Modified by Jose García
	27 Nov 2020

	This example code is in the public domain.
*/

#include <Arduino_LSM6DS3.h>

#define MAX_VALUE 10
#define WAIT_VALUE 200

#define NORT 0
#define EAST 1
#define PLUS 0
#define MINUS 1

#define NB_SEND_VALUE 3
#define POLE_POS 0
#define DIRECTION_POS 1
#define VALUE_POS 2

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int degreesZ = 0;

int i;
int angle, upAngle, leftAngle;
int upValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int leftValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int sendValues[] = {NORT, PLUS, 0};

void setup() {
	Serial.begin(9600);
	// Serial.println("Started");
	while (!Serial);

	if (!IMU.begin()) {
		// Serial.println("Failed to initialize IMU!");
		while (1);
	}
}

void loop() {

	angle = fn_get_angle();
	Serial.print("Final up angle = ");
	Serial.print(angle);
	Serial.println("°");

	if (angle > 315 && angle <= 360) {
		sendValues[POLE_POS] = NORT;
		sendValues[DIRECTION_POS] = MINUS;
		sendValues[VALUE_POS] = abs(360 - angle);
	}
	if (angle >= 0 && angle < 45) {
		sendValues[POLE_POS] = NORT;
		sendValues[DIRECTION_POS] = PLUS;
		sendValues[VALUE_POS] = angle;
	}
	if (angle > 225 && angle <= 270) {
		sendValues[POLE_POS] = EAST;
		sendValues[DIRECTION_POS] = MINUS;
		sendValues[VALUE_POS] = abs(270 - angle);
	}
	if (angle >= 270 && angle < 315) {
		sendValues[POLE_POS] = EAST;
		sendValues[DIRECTION_POS] = PLUS;
		sendValues[VALUE_POS] = abs(270 - angle);
	}

	Serial.print("Pole: ");
	Serial.print(sendValues[POLE_POS]);
	Serial.print(" Direction: ");
	Serial.print(sendValues[DIRECTION_POS]);
	Serial.print(" Value: ");
	Serial.println(sendValues[VALUE_POS]);
}

int fn_get_angle() {

	for (i = 0; i < MAX_VALUE; i++) {

		if (IMU.accelerationAvailable()) {
			IMU.readAcceleration(x, y, z);
		}

		x = -100 * x;
		degreesX = map(x, -100, 100, -90, 90);
		upValues[i] = degreesX;

		z = 100 * z;
		degreesZ = map(z, -100, 100, -90, 90);
		leftValues[i] = degreesZ;

		delay(WAIT_VALUE);
	}

	upAngle = fn_mean(upValues);
	leftAngle = fn_mean(leftValues);

	if (leftAngle > 0) { // left side
		upAngle = 270 + upAngle;
	} else { // leftAngle > 0 => rigt side
		leftAngle = fn_mean(leftValues);
		upAngle = 90 - upAngle;
	}

	return upAngle;
}

int fn_decade(int value) {

	int res = value / MAX_VALUE;
	return res * MAX_VALUE;
}

int fn_unit(int value) {

	int decade = fn_decade(value);
	return value - decade;
}

int fn_simplify(int value) {

	int unit = fn_unit(value);

	if (unit >= 5) {
		return fn_decade(value) + 10;
	} else {
		return fn_decade(value);
	}
}

int fn_mean(int values[]) {

	int res = 0;
	for (int i = 0; i < MAX_VALUE; i++) {
		res = res + values[i];
	}
	return res / MAX_VALUE;
}
