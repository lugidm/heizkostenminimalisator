#include "motor_control.h"
#define UNIT_TESTS //uncomment this line for normal behaviour! this simplifies some functions for testing

Motor::Motor() {
    this->cur_position.cur_closure = -1;
    this->cur_position.cur_pwm = -1;
    //this->closure = 0; // make here a read from the right pin
}

void Motor::open_completely() {
    this->cur_position.cur_closure = MOTOR_OPENING_BURNING;
    this->cur_position.cur_pwm = this->pwm_opened;
    
    dacWrite(PIN_MOTOR_OUT, this->pwm_opened);
}

void Motor::close_completely(){
    this->cur_position.cur_closure = 0.f;
    this->cur_position.cur_pwm = this->pwm_closed;
    dacWrite(PIN_MOTOR_OUT, this->cur_position.cur_pwm);
}

void Motor::adjust(float percentage){
    this->cur_position.cur_closure = percentage;
    this->cur_position.cur_pwm = int((abs(this->pwm_closed-this->pwm_opened)/100)*percentage+this->pwm_closed);
    dacWrite(PIN_MOTOR_OUT, int((abs(this->pwm_closed-this->pwm_opened)/100)*percentage+this->pwm_closed));
}

MotorPosition Motor::get_cur_position(){
    return MotorPosition{.cur_pwm = this->cur_position.cur_pwm, .cur_closure=this->cur_position.cur_closure};
}