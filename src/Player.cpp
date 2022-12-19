#include "Player.h"

Player::Player(uint8_t const btn_pin, uint8_t const led_pin)
: btn_pin(btn_pin)
, led_pin(led_pin)
, btn_state(LOW)
, led_state(LOW)
{
    pinMode(btn_pin, INPUT);
    pinMode(led_pin, OUTPUT);
}

void Player::read()    { btn_state = digitalRead(btn_pin); }
bool Player::pressed() { return btn_state == HIGH;         }

void Player::light(uint8_t const level) { digitalWrite(led_pin, led_state = level); }
void Player::lightOn()                  { light(HIGH);                              }
void Player::lightOff()                 { light(LOW);                               }
void Player::toggle()                   { light(!led_state & 0x1);                  }