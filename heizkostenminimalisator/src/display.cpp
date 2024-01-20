#include "display.h"
#include "wakeup_routines.h"

OutputDisplay::OutputDisplay() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET){
    this->display.begin(i2c_Address, true); // Address 0x3C default
    this->display.setContrast (0); // dim display
    this->display.display();
    this->display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    this->display.clearDisplay();

    // lines that seperate the different display headers
    this->display.drawFastHLine(POS_TEMPERATURE,128,SH110X_WHITE);
    this->display.drawFastHLine(POS_MOTOR,128,SH110X_WHITE);
    this->display.display();
    this->display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    this->displaySetupState();
    this->displaySetupTemperature();
    this->displaySetupMotor();
    this->display.display();
    delay(1000);
    this->display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    delay(1000);
}

void OutputDisplay::displaySetupState(){
    this->display.fillRect(POS_STATE, HEADERS_WIDTH, 9, SH110X_WHITE);
    this->display.setCursor(1+POS_STATE+1);
    this->display.print("STATE:");
    this->display.setCursor(HEADERS_WIDTH+8+POS_STATE);
    this->display.println("since    0 sec");
}

void OutputDisplay::displaySetupTemperature(){
    this->display.fillRect(POS_TEMPERATURE, HEADERS_WIDTH, 9, SH110X_WHITE);
    for(uint8_t i = 0; i<NUM_TEMP_MEASUREMENTS; i++){
        this->display.drawRect(3+i*26+POS_TEMPERATURE+10, 19, 10, SH110X_WHITE);
    }
    this->display.setCursor(1+POS_TEMPERATURE+1);
    this->display.println("TEMP.:");
    
}

void OutputDisplay::displaySetupMotor(){
    this->display.fillRect(POS_MOTOR, HEADERS_WIDTH, 9, SH110X_WHITE);
    this->display.setCursor(1+POS_MOTOR+1);
    this->display.println("MOTOR:");
}

void OutputDisplay::writeTemp(double* temperature_measurements){  // writes the <NUM_TEMP_MEASUREMENTS> values out to the display
    for (uint8_t i = 0; i<NUM_TEMP_MEASUREMENTS; i++) {
        this->display.setCursor(4+i*26+POS_TEMPERATURE+11);
        this->display.print(int(temperature_measurements[i]));
    }
    this->display.flush();
    this->display.display();
}

void OutputDisplay::writeMotor(double cur_percentage){
    this->display.setCursor(HEADERS_WIDTH+8+POS_MOTOR);
    this->display.println(cur_percentage, 0);
    this->display.flush();
    this->display.display();
}

void OutputDisplay::writeState(uint8_t cur_state, uint32_t time){
    this->display.setCursor(HEADERS_WIDTH+8+POS_STATE);
    this->display.print("since ");
    this->display.print(time);
    this->display.println(" sec");
    this->display.setCursor(HEADERS_WIDTH+POS_STATE+10);
    switch (cur_state)
    {
    case STATE_BURNING:
        this->display.println("BURNING     "); break;
    case STATE_NORMAL_BOOT:
        this->display.println("NORMAL BOOT "); break;
    case STATE_BURN_OFF:
        this->display.println("BURN OFF    "); break;
    case STATE_DOOR_OPENED:
        this->display.println("DOOR OPENED "); break;
    case STATE_READ_T:
        this->display.println("READ TEMP   "); break;
    case STATE_COOL_DOWN:
        this->display.println("COOL DOWN   "); break;
    default:
        this->display.println("UNDEFINED   "); break;
        break;
    }
    this->display.flush();
    this->display.display();
}