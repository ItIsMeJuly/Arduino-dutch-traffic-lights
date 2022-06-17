#include <Arduino.h>

//CONSTANTS
#define RED_TIME 8000
#define YELLOW_TIME 2000
#define GREEN_TIME 8000
#define TIME_OUT 10000
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
unsigned long timeOutTimer(0);
unsigned long blinkingYellowTimer(0);
unsigned long timer(0);
unsigned long responseTimer(0);
unsigned int delayLed(0);

//buffer for received messages
String receive;

//counter
int counter = 0;

//variables to track state for the global timer
int stage = 0;
int startNextSequence = 0;


void setup()
{
  Serial.begin(9600);
  DDRD |= 0b00011100; // Set pin 2, 3, 4 output
  timeOutTimer = millis();
  timer = millis();
  delay(1000);
}



//change light for myDelay time
void changeLight(int myDelay, int mask)
{
  startNextSequence = 0;
  PORTD &= 0;                            //set all the LEDs to low
  PORTD |= mask;                         //apply the supplied mask for the specified LED
  while (millis() - timer < myDelay) {
    //stay here until the delay has finished
  }
  stage++;
  startNextSequence = 1;
}


//when this function times out this is the final stage of the program(blinking yellow)
void yellowBlinkingTimeOut() {
  if (millis() - timeOutTimer > TIME_OUT)
  {
    PORTD &= 0;  //set all the LEDs to LOW
    int state = 0; 
    while (true)
    {
      //switch between yellow on and off
      if (millis() - blinkingYellowTimer >= BLINKING_YELLOW_TIME)
      {
        if (state == 0)
        {
          PORTD |= yellowMask; //on
          state = 1;
        }
        else
        {
          state = 0;
          PORTD &= 0;   //off
        }
        blinkingYellowTimer = millis();
      }
    }
    timeOutTimer = millis();
  }
}


void loop()
{
  stage %= 4;

  switch (stage)
  {

    //RED
    case 0:
      //set initial timer for the next sequence(case)
      if (startNextSequence == 1)
      {
        timer = millis();
      }
      if(counter == 0){
        Serial.print("GREEN"); //send green to slave 
        ++counter; 
      }
      if (receive != "ACK") {
        timeOutTimer = millis();
        //while we dont receive ACK, wait here 
        while (receive != "ACK")
        {
          // timeout sequence
          if (Serial.available()) {          //we are reading the mesage from the slave "ACK"
            receive += (char)Serial.read();
          }
          yellowBlinkingTimeOut();  //function monitoring for timeout
        }
      }
      counter = 0;
      receive = ""; //clear the buffer for next message
      changeLight(RED_TIME, redMask); //call the change lights function
      break;

    //Yellow Short
    case 1:
      if (startNextSequence == 1)
      {
        timer = millis();
      }
      if(counter == 0){
        Serial.print("LONG_YELLOW"); //send green to slave 
        ++counter; 
      }
      if (receive != "ACK")      //if the buffer is different than ACK
      {
        timeOutTimer = millis();
        while (receive != "ACK")
        {
          // timeout sequence
          if (Serial.available() > 0) {
            receive += (char)Serial.read();   //if there is a message from the slave, then we are reading it
            delay(20);
          }
          yellowBlinkingTimeOut();  //function monitoring for timeoiuts
        }
      }
      counter = 0;
      receive = "";  //clear the buffer for the next stage
      changeLight(YELLOW_TIME - 1800, yellowMask);     //change light
      break;

    //Green
    case 2:
      if (startNextSequence == 1)
      {
        timer = millis();
      }
      if(counter == 0){
        Serial.print("RED"); //send green to slave 
        ++counter; 
      }
      if (receive != "ACK")
      {
        timeOutTimer = millis();
        while (receive != "ACK")   //while we didnt receive an ACK message from the slave
        {
          // timeout sequence
          if (Serial.available() > 0) {
            receive += (char)Serial.read();   //read the message from the salve if we got one
            delay(20);
          }
          yellowBlinkingTimeOut();    //function to monitor for timeout
        }
      }
      PORTD &= 0;      //set all LEDs to low
      receive = "";  //clear the buffer
      counter = 0;
      changeLight(GREEN_TIME, greenMask);
      break;


    //Long Yellow
    case 3:
      if (startNextSequence == 1)
      {
        timer = millis();
      }
      if(counter == 0){
        Serial.print("YELLOW"); //send green to slave 
        ++counter; 
      }
      if (receive != "ACK")
      {
        timeOutTimer = millis();
        while (receive != "ACK")   //if we didnt receive an ACK, stay here
        {
          // timeout sequence
          if (Serial.available() > 0) {
            receive += (char)Serial.read();  //read the message from slave if we got one
            delay(20);
          }
          yellowBlinkingTimeOut();
        }
      }
      receive = "";   //clear the buffer
      counter = 0;
      changeLight(YELLOW_TIME, yellowMask);
      break;
  }
}