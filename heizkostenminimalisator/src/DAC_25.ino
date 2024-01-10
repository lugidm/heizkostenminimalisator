/*
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * 
 * ESP32 DAC - Digital To Analog Conversion Example
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DAC1 25
char Carray[100];
char x;
int i = 0;

//Test

void setup() {
  Serial.begin(9600);
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.setContrast (0); // dim display
  display.display();
  delay(1000);
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
}

void loop(){
  for (i=0;i<=99;i++) {Carray[i] = char(0);}
  i=0;
  while ((Serial.available() > 0) & (i < 3)) {
    x=Serial.read();
    switch (x) {
     case '0' ... '9':  {Carray[i]=x;i++;Serial.print(x);Serial.print("|");Serial.print(i);Serial.print("|");}
    }
  }
  if (Carray[0] != char(0)) {
    Serial.println();
    Serial.println(Carray);
    unsigned int dutyCycle = (atoi(Carray));
    Serial.println(dutyCycle);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Vout:");
    display.println(dutyCycle);
    display.print("Soll:");
    display.print(int((dutyCycle-120)*100/120));
    display.println(" %");
    display.print("V in:");
    display.println("0 V");
    display.print("Ist: ");
    display.println("0 %");
    display.display();
    
    dacWrite(DAC1, dutyCycle);
    delay(2000);
  }
  delay(100);
}
