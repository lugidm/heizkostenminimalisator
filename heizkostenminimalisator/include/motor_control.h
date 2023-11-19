#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H
#include <Arduino.h>


class Motor{
    private:
        char v_zero_out = 110;
        char v_max_out = 235;
        float v_closed_in = 3.233;
        float v_open_in = 0.66666;
        float x = 10;
        float closure;
        bool apply(float position);
    public:
        Motor();
        bool open_completely();
        bool adjust(float percentage);
        bool close_completely();
    };


#endif