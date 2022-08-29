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

byte pole, direction, angle;

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
				// accelValue = accelCharacteristic.value();
				// accelCharacteristic.readValue(receivedValues, sizeof(receivedValues));

				poleCharacteristic.readValue(pole);

				Serial.print("Pole: ");
				Serial.println(pole);
			}

			if (directionCharacteristic.written()) {
				// accelValue = accelCharacteristic.value();
				// accelCharacteristic.readValue(receivedValues, sizeof(receivedValues));

				directionCharacteristic.readValue(direction);

				Serial.print("Direction: ");
				Serial.println(direction);
			}

			if (angleCharacteristic.written()) {
				// accelValue = accelCharacteristic.value();
				// accelCharacteristic.readValue(receivedValues, sizeof(receivedValues));

				angleCharacteristic.readValue(angle);

				Serial.print("Angle: ");
				Serial.println(angle);
			}
		}

		// when the central disconnects, print it out:
		Serial.print(F("Disconnected from central: "));
		Serial.println(central.address());
	}
}
