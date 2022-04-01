#include <Arduino.h>
#ifndef LED_H
#define LED_H

class LED{
    byte pin;
    int value;
    int counter;

public:

    LED(byte pin);
    void init();
    void on();
    void off();
    int getCounter();
    int getValue();
    void eraseCounter();
};

#endif