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

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int degreesZ = 0;

int i;
int upAngle, leftAngle;
int upValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int leftValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


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

        delay(500);
    }

    upAngle = simplify(fn_mean(upValues));
    leftAngle = fn_mean(leftValues);

    Serial.print("DEBUG upAngle: ");
    Serial.println(upAngle);
    Serial.print("DEBUG leftAngle: ");
    Serial.println(leftAngle);
    Serial.println("");

    if (leftAngle > 0) { // left side
        upAngle = 270 + upAngle;
    } else { // leftAngle > 0 => rigt side
        upAngle = 90 - upAngle;
    }

    Serial.print("Final up angle = ");
    Serial.print(upAngle);
    Serial.println("°");
}

int fn_decade(int value) {
    int res = value / MAX_VALUE;
    return res * MAX_VALUE;
}

int fn_unit(int value) {
    int decade = fn_decade(value);
    return value - decade;
}

int simplify(int value) {
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
