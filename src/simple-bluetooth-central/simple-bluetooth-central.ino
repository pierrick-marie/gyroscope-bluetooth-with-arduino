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

#define MAX_VALUE 10
#define WAIT_VALUE 200

#define NORT 0
#define EAST 1
#define PLUS 0
#define MINUS 1

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int degreesZ = 0;

int i;
int upAngle, leftAngle;
int upValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int leftValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

byte pole, direction, value;

// BLECharacteristic accelCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite, false, sizeof(sendValues));

void setup() {

	Serial.begin(9600);

	// initialize the Bluetooth® Low Energy hardware
	BLE.begin();

	Serial.println("Bluetooth® Low Energy Central - LED control");

	// start scanning for peripherals
	BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop() {

	// check if a peripheral has been discovered
	BLEDevice peripheral = BLE.available();

	if (peripheral) {
		// discovered a peripheral, print out address, local name, and advertised service
		Serial.print("Found ");
		Serial.print(peripheral.address());
		Serial.print(" '");
		Serial.print(peripheral.localName());
		Serial.print("' ");
		Serial.print(peripheral.advertisedServiceUuid());
		Serial.println();

		if (peripheral.localName() != "ACCELEROMETER") {
			return;
		}

		// stop scanning
		BLE.stopScan();

		fn_start_service(peripheral);

		// peripheral disconnected, start scanning again
		BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
	}
}

void fn_start_service(BLEDevice peripheral) {

	// connect to the peripheral
	Serial.println("Connecting ...");

	if (peripheral.connect()) {
		Serial.println("Connected");
	} else {
		Serial.println("Failed to connect!");
		return;
	}

	// discover peripheral attributes
	Serial.println("Discovering attributes ...");
	if (peripheral.discoverAttributes()) {
		Serial.println("Attributes discovered");
	} else {
		Serial.println("Attribute discovery failed!");
		peripheral.disconnect();
		return;
	}

	// retrieve the pole characteristic
	BLECharacteristic poleCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1215");
	BLECharacteristic directionCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1216");
	BLECharacteristic valueCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1217");

	int i = 1;
	// send values
	while (peripheral.connected()) {

		pole = (byte)i;
		Serial.print("Pole: ");
		Serial.println(pole);
		direction = (byte)(i + 3);
		Serial.print(" Direction: ");
		Serial.println(direction);
		value = (byte)(i + 10);
		Serial.print(" Value: ");
		Serial.println(value);

		poleCharacteristic.writeValue(pole);
		directionCharacteristic.writeValue(direction);
		valueCharacteristic.writeValue(value);

		delay(1000);
		i++;
	}
	Serial.println("Peripheral disconnected");
}
