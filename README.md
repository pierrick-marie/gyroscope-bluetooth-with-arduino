# Gyroscope Bluetooth

Gyroscope à affichage déporté en Bluetooth basé sur deux Arduino Nano 33 IoT et un écran LCD.

# Objectif 

L'objectif du projet est de mesurer l'inclinaison d'une pente en degrès en temps réel et l'afficher sur écran déporté.
C'est une sorte de niveau à bulle électronique avec un affichage déporté.
La communication entre les élements du dispositif doit être sans fil.

# Matériel

Le projet est basé sur deux Arduino Nano 33 IoT (https://docs.arduino.cc/hardware/nano-33-iot).
Le premier mesure son inclinaison en degrès puis transmet en Bluetooth la valeur mesurée au second module Arduino. Ce dernier affiche la valeur reçue via un petit écran OLED de 128x64 pixels (https://www.az-delivery.de/en/products/0-96zolldisplay).

Les deux modules Arduino sont autonomes à l'aide d'une petite batterie USB.

![Schema d'ensemble du montage](https://framagit.org/pierrick/gyroscope-ble/-/raw/main/docs/schema/vue-densemble.png)

# Documentation

* Comment connecter deux Arduino en Bluetooth : https://docs.arduino.cc/tutorials/nano-33-ble-sense/ble-device-to-device?utm_source=pocket_mylist
* Utiliser l'accelerometre : https://docs.arduino.cc/tutorials/nano-33-iot/imu-accelerometer?utm_source=pocket_mylist
* Utiliser l'écran OLED : https://passionelectronique.fr/ecran-oled-i2c-arduino/?utm_source=pocket_mylist
