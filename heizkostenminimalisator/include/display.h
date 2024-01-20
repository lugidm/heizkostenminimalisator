#ifndef DISPLAY_H
#define DISPLAY_H
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

#define POS_STATE 0,0
#define POS_TEMPERATURE 0,21
#define POS_MOTOR 0,42
#define HEADERS_WIDTH 35

class OutputDisplay{
    private:
        Adafruit_SH1106G display;
        void displaySetupMotor();
        void displaySetupTemperature();
        void displaySetupState();
    public:
        OutputDisplay();
        void writeTemp(double* temperature_measurements);
        void writeMotor(double cur_percentage);
        void writeState(uint8_t cur_state, uint32_t time);
};




#endif