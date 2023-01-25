#include <ArduinoBLE.h>

byte pole, direction, angle;

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
	BLECharacteristic angleCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1217");

	int i = 1;
	// send values
	while (peripheral.connected()) {

		pole = (byte)i;
		Serial.print("Pole: ");
		Serial.println(pole);
		direction = (byte)(i + 3);
		Serial.print(" Direction: ");
		Serial.println(direction);
		angle = (byte)(i + 10);
		Serial.print(" Angle: ");
		Serial.println(angle);

		poleCharacteristic.writeValue(pole);
		directionCharacteristic.writeValue(direction);
		angleCharacteristic.writeValue(angle);

		delay(1000);
		i++;
	}
	Serial.println("Peripheral disconnected");
}
