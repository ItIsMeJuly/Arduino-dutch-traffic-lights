#include <Arduino.h>

//Constants
#define TIME_OUT 8000
#define BLINKING_YELLOW_TIME 400

//LED pins
const int RedLed = 4;
const int YellowLed = 3;
const int GreenLed = 2;

//bit masks for the LEDs
const int redMask = 0b00010000;
const int yellowMask = 0b00001000;
const int greenMask = 0b00000100;

//timers
unsigned long timeOutTimer;
unsigned long blinkingYellowTimer;
unsigned long yellowLightTimer;

//buffer
String receive;

//counter
int counter = 0;

//when this function times out this is the final stage of the program(blinking yellow)
void yellowBlinkingTimeOut() {
  if (millis() - timeOutTimer > TIME_OUT)
  {
    DDRD &= 0;  //set all the LEDs to LOW
    int state = 0; 
    while (true)
    {
      //switch between yellow on and off
      if (millis() - blinkingYellowTimer >= BLINKING_YELLOW_TIME)
      {
        if (state == 0)
        {
          DDRD |= yellowMask; //on
          state = 1;
        }
        else
        {
          state = 0;
          DDRD &= 0;   //off
        }
        blinkingYellowTimer = millis();
      }
    }
    timeOutTimer = millis();
  }
}

void setup(){
  Serial.begin(9600);
  PORTD |= 0b00011100;
  timeOutTimer = millis();
  delay(1000);
}

void loop(){
  if(Serial.available()){
    receive += (char)Serial.read();
    if(receive.equals("RED")){
      Serial.print("ACK");
      delay(500);
      DDRD &= 0;
      DDRD |= redMask;
      receive = "";
    }
    if(receive.equals("YELLOW")){
      Serial.print("ACK");
      delay(1800);
      DDRD &= 0;
      DDRD |= yellowMask;
      receive = "";
    }
    if(receive.equals("GREEN")){
      Serial.print("ACK");
      DDRD &= 0;
      DDRD |= greenMask;
      receive = "";
    }
    if(receive.equals("LONG")){
      Serial.print("ACK");
      DDRD &= 0;
      DDRD |= yellowMask;
      receive = "";
    }
    timeOutTimer = millis();
  }
  else{
    if(millis() - timeOutTimer > TIME_OUT){
      yellowBlinkingTimeOut();
    }
  }
}