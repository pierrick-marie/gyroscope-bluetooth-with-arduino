# Gyroscope Bluetooth

Gyroscope à affichage déporté en Bluetooth basé sur deux Arduino Nano 33 IoT et un écran LCD.

# Objectif 

L'objectif du projet est de mesurer l'inclinaison d'une pente en degrès en temps réel et l'afficher sur écran déporté. La communication entre les élements du dispositif doit être sans fil.

# Matériel

Le projet est basé sur deux Arduino Nano 33 IoT (https://docs.arduino.cc/hardware/nano-33-iot).
Le premier mesure son inclinaison en degrès puis transmet en Bluetooth la valeur mesurée au second module Arduino. Ce dernier affiche la valeur reçue via un petit écran OLED de 128x64 pixels (https://www.az-delivery.de/en/products/0-96zolldisplay).

Les deux modules Arduino sont autonomes à l'aide d'une petite batterie USB.
