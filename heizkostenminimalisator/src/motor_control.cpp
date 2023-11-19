#include "motor_control.h"

Motor::Motor() {
    Serial.println("Init Motor controller");
    this->closure = 0; // make here a read from the right pin
}

bool Motor::open_completely() {
    return true;
}