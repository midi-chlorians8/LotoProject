#pragma once
#include <Arduino.h>

class ButtonStart{
private:
    // Обработка кнопки старт

    //const int ledPin = 13;      // the number of the LED pin

    // Variables will change:
    bool ledState = HIGH;         // the current state of the output pin
    bool buttonState;             // the current reading from the input pin
    bool lastButtonState = LOW;   // the previous reading from the input pin

    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    unsigned int debounceDelay = 20;    // the debounce time; increase if the output flickers
    // Обработка кнопки старт

    const int8_t pinButtonStart = 4;
    bool isPressed = false; // Была ли нажата кнопка старт
public:
  ButtonStart(){
      pinMode(pinButtonStart,INPUT_PULLUP);
  }
   ButtonStart(int newDebounceDelay){
      debounceDelay = newDebounceDelay;
      pinMode(pinButtonStart,INPUT_PULLUP);
  }
  void Read(){
      bool reading = digitalRead(pinButtonStart);
      if (reading != lastButtonState) {
          // reset the debouncing timer
          lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {

          if (reading != buttonState) {
              buttonState = reading;

              // only toggle the LED if the new button state is HIGH
              if (buttonState == LOW) {
                  ledState = !ledState;
                  isPressed = true;
                  Serial.println(F("Button start pressed"));
              }
          }
      }

      // set the LED:
      //digitalWrite(ledPin, ledState);

      // save the reading. Next time through the loop, it'll be the lastButtonState:
      lastButtonState = reading;
      }
  bool GetIsPressed (){
    if(isPressed == false){
      return false;
    }
    else{
      return true;
    }
  }
  void SetResetButton(){
      isPressed = false;
  }
};
ButtonStart *MyButtonStartPtr = nullptr; // Cоздали указатель