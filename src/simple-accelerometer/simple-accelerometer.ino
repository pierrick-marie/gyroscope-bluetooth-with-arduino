/*
	Arduino LSM6DS3 - Simple Accelerometer

	This example reads the acceleration values from the LSM6DS3
	sensor and continuously prints them to the Serial Monitor
	or Serial Plotter.

	The circuit:
	- Arduino Uno WiFi Rev 2 or Arduino Nano 33 IoT

	created 10 Jul 2019
	by Riccardo Rizzo

	This example code is in the public domain.
*/

#include <Arduino_LSM6DS3.h>

float x, y, z;
int angl;

void setup() {

	Serial.begin(9600);
	while (!Serial);

	if (!IMU.begin()) {
		Serial.println("Failed to initialize IMU!");
		while (1);
	}

	Serial.print("Accelerometer sample rate = ");
	Serial.print(IMU.accelerationSampleRate());
	Serial.println(" Hz");
	Serial.println();
	Serial.println("Acceleration in G's");
	Serial.println("X\tY\tZ");
}

void loop() {

	if (IMU.accelerationAvailable()) {

		IMU.readAcceleration(x, y, z);
		angl = x * (-90);

		Serial.print(angl);
		Serial.println(" °");
	}
	
	delay(1000);
}
