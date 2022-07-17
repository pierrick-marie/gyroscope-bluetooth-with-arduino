/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int COORD_00_1[] = {64, 0, 64, 31};
const int COORD_00_2[] = {65, 0, 65, 31};
const int COORD_10_1[] = {72, 0, 56, 31};
const int COORD_10_2[] = {73, 0, 57, 31};
const int COORD_20_1[] = {80, 0, 48, 31};
const int COORD_20_2[] = {81, 0, 49, 31};
const int COORD_30_1[] = {90, 0, 38, 31};
const int COORD_30_2[] = {91, 0, 39, 31};
const int COORD_40_1[] = {100, 0, 28, 31};
const int COORD_40_2[] = {101, 0, 29, 31};
const int COORD_50_1[] = {24, 28, 104, 3};
const int COORD_50_2[] = {24, 29, 104, 4};
const int COORD_60_1[] = {24, 24, 104, 7};
const int COORD_60_2[] = {24, 25, 104, 8};
const int COORD_70_1[] = {24, 21, 104, 10};
const int COORD_70_2[] = {24, 22, 104, 11};
const int COORD_80_1[] = {24, 19, 104, 13};
const int COORD_80_2[] = {24, 20, 104, 12};
const int COORD_90_1[] = {24, 16, 104, 16};
const int COORD_90_2[] = {24, 17, 104, 17};
const int COORD_100_1[] = {24, 13, 104, 19};
const int COORD_100_2[] = {24, 12, 104, 20};
const int COORD_110_1[] = {24, 10, 104, 21};
const int COORD_110_2[] = {24, 11, 104, 22};
const int COORD_120_1[] = {24, 7, 104, 24};
const int COORD_120_2[] = {24, 8, 104, 25};
const int COORD_130_1[] = {24, 3, 104, 28};
const int COORD_130_2[] = {24, 4, 104, 29};
const int COORD_140_1[] = {28, 0, 100, 31};
const int COORD_140_2[] = {29, 0, 101, 31};
const int COORD_150_1[] = {38, 0, 90, 31};
const int COORD_150_2[] = {39, 0, 91, 31};
const int COORD_160_1[] = {48, 0, 80, 31};
const int COORD_160_2[] = {49, 0, 81, 31};
const int COORD_170_1[] = {56, 0, 72, 31};
const int COORD_170_2[] = {57, 0, 73, 31};


void setup() {
      Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
          Serial.println(F("SSD1306 allocation failed"));
          for(;;); // Don't proceed, loop forever
      }

      // Clear the buffer
      display.clearDisplay();

      // testdrawchar();      // Draw characters of the default font
}

void loop() {
    testdrawline();
}

void testdrawchar(void) {

      display.clearDisplay();

      display.setTextSize(5);      // Normal 1:1 pixel scale
      display.setTextColor(WHITE); // Draw white text
      display.setCursor(18, 0);     // Start at top-left corner
      display.cp437(true);         // Use full 256 char 'Code Page 437' font

      int valeurTemperature = 25;        // Température, exprimée en degrés Celsius
      display.print("+");
      display.print(valeurTemperature);

      display.display();
      delay(1000);
}

void testdrawline() {

      // 00°|360° - 180°
      display.drawLine(COORD_00_1[0], COORD_00_1[1], COORD_00_1[2], COORD_00_1[3], WHITE);
      display.drawLine(COORD_00_2[0], COORD_00_2[1], COORD_00_2[2], COORD_00_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 10° - 190°
      display.drawLine(COORD_10_1[0], COORD_10_1[1], COORD_10_1[2], COORD_10_1[3], WHITE);
      display.drawLine(COORD_10_2[0], COORD_10_2[1], COORD_10_2[2], COORD_10_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 20° - 200°
      display.drawLine(COORD_20_1[0], COORD_20_1[1], COORD_20_1[2], COORD_20_1[3], WHITE);
      display.drawLine(COORD_20_2[0], COORD_20_2[1], COORD_20_2[2], COORD_20_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 30° - 210°
      display.drawLine(COORD_30_1[0], COORD_30_1[1], COORD_30_1[2], COORD_30_1[3], WHITE);
      display.drawLine(COORD_30_2[0], COORD_30_2[1], COORD_30_2[2], COORD_30_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 40° - 220°
      display.drawLine(COORD_40_1[0], COORD_40_1[1], COORD_40_1[2], COORD_40_1[3], WHITE);
      display.drawLine(COORD_40_2[0], COORD_40_2[1], COORD_40_2[2], COORD_40_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 50° - 230°
      display.drawLine(COORD_50_1[0], COORD_50_1[1], COORD_50_1[2], COORD_50_1[3], WHITE);
      display.drawLine(COORD_50_2[0], COORD_50_2[1], COORD_50_2[2], COORD_50_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 60° - 240°
      display.drawLine(COORD_60_1[0], COORD_60_1[1], COORD_60_1[2], COORD_60_1[3], WHITE);
      display.drawLine(COORD_60_2[0], COORD_60_2[1], COORD_60_2[2], COORD_60_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 70° - 250°
      display.drawLine(COORD_70_1[0], COORD_70_1[1], COORD_70_1[2], COORD_70_1[3], WHITE);
      display.drawLine(COORD_70_2[0], COORD_70_2[1], COORD_70_2[2], COORD_70_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 80° - 260°
      display.drawLine(COORD_80_1[0], COORD_80_1[1], COORD_80_1[2], COORD_80_1[3], WHITE);
      display.drawLine(COORD_80_2[0], COORD_80_2[1], COORD_80_2[2], COORD_80_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();


      // 90° - 270°
      display.drawLine(COORD_90_1[0], COORD_90_1[1], COORD_90_1[2], COORD_90_1[3], WHITE);
      display.drawLine(COORD_90_2[0], COORD_90_2[1], COORD_90_2[2], COORD_90_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 100° - 280°
      display.drawLine(COORD_100_1[0], COORD_100_1[1], COORD_100_1[2], COORD_100_1[3], WHITE);
      display.drawLine(COORD_100_2[0], COORD_100_2[1], COORD_100_2[2], COORD_100_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 110° - 290°
      display.drawLine(COORD_110_1[0], COORD_110_1[1], COORD_110_1[2], COORD_110_1[3], WHITE);
      display.drawLine(COORD_110_2[0], COORD_110_2[1], COORD_110_2[2], COORD_110_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 120° - 300°
      display.drawLine(COORD_120_1[0], COORD_120_1[1], COORD_120_1[2], COORD_120_1[3], WHITE);
      display.drawLine(COORD_120_2[0], COORD_120_2[1], COORD_120_2[2], COORD_120_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      //130° - 310°
      display.drawLine(COORD_130_1[0], COORD_130_1[1], COORD_130_1[2], COORD_130_1[3], WHITE);
      display.drawLine(COORD_130_2[0], COORD_130_2[1], COORD_130_2[2], COORD_130_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 140° - 320°
      display.drawLine(COORD_140_1[0], COORD_140_1[1], COORD_140_1[2], COORD_140_1[3], WHITE);
      display.drawLine(COORD_140_2[0], COORD_140_2[1], COORD_140_2[2], COORD_140_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 150° - 330°
      display.drawLine(COORD_150_1[0], COORD_150_1[1], COORD_150_1[2], COORD_150_1[3], WHITE);
      display.drawLine(COORD_150_2[0], COORD_150_2[1], COORD_150_2[2], COORD_150_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 160° - 340°
      display.drawLine(COORD_160_1[0], COORD_160_1[1], COORD_160_1[2], COORD_160_1[3], WHITE);
      display.drawLine(COORD_160_2[0], COORD_160_2[1], COORD_160_2[2], COORD_160_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();

      // 170° - 350°
      display.drawLine(COORD_170_1[0], COORD_170_1[1], COORD_170_1[2], COORD_170_1[3], WHITE);
      display.drawLine(COORD_170_2[0], COORD_170_2[1], COORD_170_2[2], COORD_170_2[3], WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();
}
