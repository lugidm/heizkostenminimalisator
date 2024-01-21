#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "pin_setup.h"

////////////TODO: change according to your wishes///////////
#define MOTOR_OPENING_COOLDOWN 10.f // IN PERCENTAGES (between 0.f and 100.f)
#define MOTOR_OPENING_BURNING 100.f // IN PERCENTAGES (between 0.f and 100.f)

////////////////////

typedef struct MotorPosition{
    float cur_pwm;
    float cur_closure; // percentage of cur_pwm : 0=close 100=open(linear function, see ../documentation/measured_voltage_values_pwm.ods) 
} MotorPosition;


class Motor{
    private:
        uint8_t pwm_closed = 110;
        uint8_t pwm_opened = 235;
        MotorPosition cur_position;
    public:
        Motor();
        void open_completely();
        void adjust(float percentage); 
        void close_completely();
        MotorPosition get_cur_position();
    };


#endif