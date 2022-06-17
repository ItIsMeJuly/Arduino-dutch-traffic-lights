
#include "LED.h"

void LED::init(){
    pinMode(pin, OUTPUT);
    value = LOW;
    counter = 0;
}

LED::LED(byte pin){
    this->pin = pin;

    init();
}

void LED::on(){
    digitalWrite(pin, HIGH);
    value = HIGH; //sets the last value to HIGH
    counter++; //counts the total operations the current object has done
}

void LED::off(){
    digitalWrite(pin, LOW);
    value = LOW; //sets the last value to LOW
    counter++; //count the total operation the current object has done
}

int LED::getCounter(){
    return counter;
}

int LED::getValue(){
    return value;
}

void LED::eraseCounter(){
    counter = 0;
}
