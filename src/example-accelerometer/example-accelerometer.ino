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

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int degreesZ = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
}

void loop() {

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

  }

  /*
  if (x >= 0) {
    x = 100 * x;
    degreesX = map(x, 0, 100, 0, 90);
    Serial.print("Tilting down ");
    Serial.print(degreesX);
    Serial.println("  degrees");
  }
  if (x < 0) {
    x = 100 * x;
    degreesX = map(x, 0, -100, 0, 90);
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println("  degrees");
  }
  */

  x = -100 * x;
  degreesX = map(x, -100, 100, -90, 90);
  Serial.print("Tilting up/down ");
  Serial.print(degreesX);
  Serial.println(" degrees");

  z = 100 * z;
  degreesZ = map(z, -100, 100, -90, 90);
  Serial.print("Tilting left/right ");
  Serial.print(degreesZ);
  Serial.println(" degrees");

  /*
  if (z >= 0) {
    z = 100 * z;
    degreesZ = map(z, 0, 100, 0, 90);
    Serial.print("Tilting left ");
    Serial.print(degreesZ);
    Serial.println("  degrees");
  }
  if (z < 0) {
    z = 100 * z;
    degreesZ = map(z, 0, -100, 0, 90);
    Serial.print("Tilting right ");
    Serial.print(degreesZ);
    Serial.println("  degrees");
  }
  */

  Serial.println("");
  delay(1000);
}
