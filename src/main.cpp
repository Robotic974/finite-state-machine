#include <Arduino.h>
#include "Player.h"

uint16_t toggle_delay = 0;

enum class State : uint8_t { RESET, GET_READY, PLAY, STOP };

State state;

Player player1(A3,  A5);
Player player2(DD2, DD4);

void reset() {
    player1.lightOn();
    player2.lightOff();
    toggle_delay = 0;
    state = State::GET_READY;
}

void getReady() {

         if (player1.pressed()) { player1.lightOff(); player2.lightOn(); state = State::STOP; }
    else if (player2.pressed()) { player2.lightOff(); player1.lightOn(); state = State::STOP; }

    static bool     off     = false;
    static bool     flashed = false;
    static uint32_t last    = millis();
           uint32_t now     = millis();

    if (toggle_delay < 224) {

        if (now - last < toggle_delay) return;

        player1.toggle();
        player2.toggle();
        toggle_delay += 8;
        last = now;
        return;

    }

    if (now - last < toggle_delay) {
        if (!off) {
            player1.lightOff();
            player2.lightOff();
            off     = true;
            flashed = false;
        }
        return;
    }

    if (now - last < toggle_delay + 50) {
        if (!flashed) {
            player1.lightOn();
            player2.lightOn();
            flashed = true;
        }
        return;
    }

    player1.lightOff();
    player2.lightOff();
    off   = false;
    state = State::PLAY;

}

void play() {

         if (player1.pressed()) { player1.lightOn(); state = State::STOP; }
    else if (player2.pressed()) { player2.lightOn(); state = State::STOP; }

}

void stop() { delay(2000); state = State::RESET; }

void setup() { state = State::RESET; }

void loop() {

    player1.read();
    player2.read();

    switch (state) {

        case State::RESET:     reset();    break;
        case State::GET_READY: getReady(); break;
        case State::PLAY:      play();     break;
        case State::STOP:      stop();

    }

}