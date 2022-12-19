#pragma once

#include <Arduino.h>

class Player {

    public:

        Player(uint8_t const btn_pin, uint8_t const led_pin);

        void read();
        bool pressed();

        void lightOn();
        void lightOff();
        void toggle();

    private:

        uint8_t btn_pin;
        uint8_t led_pin;
        uint8_t btn_state;
        uint8_t led_state;

        void light(uint8_t const level);

};