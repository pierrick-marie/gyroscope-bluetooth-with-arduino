/**
 *	This is the gyroscope part of the project. It gets the angle of the Arduino and send the measure through the Bluetooth connection.
 */

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define MAX_VALUE 10
#define WAIT_VALUE 50

#define NORTH 0
#define EAST 1
#define UP 0
#define DOWN 1

float x, y, z;
float degreesX = 0.0;
float degreesY = 0.0;
float degreesZ = 0.0;

int i;
float xAngle, yAngle, zAngle;
float xValues[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float yValues[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float zValues[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

byte pole, direction, angle;

void setup() {

	Serial.begin(9600);

	if (!IMU.begin()) {
		Serial.println("Failed to initialize IMU!");
		while (1);
	}
	Serial.print(IMU.accelerationSampleRate());

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
		/*
		Serial.print("Found ");
		Serial.print(peripheral.address());
		Serial.print(" '");
		Serial.print(peripheral.localName());
		Serial.print("' ");
		Serial.print(peripheral.advertisedServiceUuid());
		Serial.println();
		*/

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

	// retrieve the Accelerometer characteristic
	BLECharacteristic poleCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1215");
	BLECharacteristic directionCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1216");
	BLECharacteristic angleCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1217");

	if (!poleCharacteristic) {
		Serial.println("Peripheral does not have Accelerator characteristic!");
		peripheral.disconnect();
		return;
	} else if (!poleCharacteristic.canWrite()) {
		Serial.println("Peripheral does not have a writable Accelerator characteristic!");
		peripheral.disconnect();
		return;
	}

	// send values
	while (peripheral.connected()) {

		fn_get_angles();
		if (xAngle >= 75.0) { // NORTH pole
			pole = (byte)NORTH;
			if (zAngle >= 0.0) { // UP side
				direction = (byte)DOWN;
			} else {
				direction = (byte)UP;
			}
			angle = fn_get_round(abs(zAngle));
		} else {
			if (zAngle >= 75.0) { // EAST pole
				pole = (byte)EAST;
				if (xAngle >= 0.0) { // UP side
					direction = (byte)UP;
				} else {
					direction = (byte)DOWN;
				}
				angle = fn_get_round(abs(xAngle));
			}
		}

		/*
		Serial.print("Pole: ");
		Serial.print(pole);
		Serial.print(" Direction: ");
		Serial.print(direction);
		Serial.print(" Angle: ");
		Serial.print("° ----> ");
		Serial.print(angle);
		Serial.println("°");
		*/

		poleCharacteristic.writeValue(pole);
		directionCharacteristic.writeValue(direction);
		angleCharacteristic.writeValue(angle);
	}

	Serial.println("Peripheral disconnected");
}

byte fn_get_round(float value) {
	byte res = (byte) value; // get integer part of finalAngle
	value = value - res; // get decimal part of finalAngle
	if (value >= 0.5 && value < 1) {
		res = res + 1;
		return res;
	}
}

void fn_get_angles() {

	for (i = 0; i < MAX_VALUE; i++) {
		if (IMU.accelerationAvailable()) {
			IMU.readAcceleration(x, y, z);
		}

		x = -100.0 * x;
		degreesX = map(x, -100.9, 99.9, -90.0, 90.0);
		xValues[i] = degreesX;
		// xValues[i] = x;

		y = -100.0 * y;
		degreesY = map(y, -100.0, 100.0, -90.0, 90.0);
		yValues[i] = degreesY;

		z = 100.0 * z;
		degreesZ = map(z, -100.0, 100.0, -90.0, 90.0);
		zValues[i] = degreesZ;

		delay(WAIT_VALUE);
	}

	xAngle = fn_mean(xValues);
	yAngle = fn_mean(yValues);
	zAngle = fn_mean(zValues);

	/*
	Serial.print("DEBUG xAngle: ");
	Serial.print(xAngle);
	Serial.print("° -- yAngle: ");
	Serial.print(yAngle);
	Serial.print("° -- zAngle: ");
	Serial.print(zAngle);
	Serial.println("°");
	*/
}

int fn_decade(float value) {

	int res = value / 10;
	return res * 10;
}

int fn_unit(float value) {

	int decade = fn_decade(value);
	return value - decade;
}

int fn_simplify(float value) {

	int unit = fn_unit(value);

	if (unit >= 5.0) {
		return fn_decade(value) + 10;
	} else {
		return fn_decade(value);
	}
}

float fn_mean(float values[]) {

	float res = 0.0;
	for (i = 0; i < MAX_VALUE; i++) {
		res = res + values[i];
	}
	return res / MAX_VALUE;
}
