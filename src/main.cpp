#include <Arduino.h>
#include "MyUserServo.h"

const int8_t pinButtonStart = 4;

const int8_t pinIrBallDetector = 8;

const int8_t pinLaser = 3;

enum StatesEndTime{
  NoEnded,
  Ended,
} states;

class Relays{
private:
  const int8_t releElektromagnet = 7;
  const int8_t relePitDvig = 5;
  const int8_t releMoreSpeed = 10;

  // Переменные реле Электромагнит #1
  int delayElektromagnetReleWork = 500;
  bool canOnElektromagnetRele = true;
  bool firsTimeZamerElektromagnetRele=false;
  unsigned long timingElektromagnetRele;
  // Переменные реле Электромагнит #1

  // Переменные реле Питания Двигателя #2
  int delayPitDvigReleWork= 20000;
  bool canOnPitDvigRele = true;
  bool firsTimeZamerPitDvigRele=false;
  unsigned long timingPitDvigRele;

  bool timeTickAftherActivatRele2End = false;
  // Переменные реле Питания Двигателя #2

  // Переменные реле MoreSpeedRele #3
  int delayMoreSpeedReleWork= 30000;
  bool canOnMoreSpeedRele = true;
  bool firsTimeZamerMoreSpeedRele=false;
  unsigned long timingMoreSpeedRele;
  // Переменные реле MoreSpeedRele #3
  
  // Переменные для отсчёта времени после завершения работы MoreSpeedRele #3 CountTimeAfthRele3
  int delayCountTimeAfthRele3 = 10000;
  bool canOnCountTimeAfthRele3 = true;
  bool firsTimeZamerCountTimeAfthRele3=false;
  unsigned long timingCountTimeAfthRele3;

  bool rele3WorkEnd = false;
  // Переменные для отсчёта времени после завершения работы MoreSpeedRele #3
  bool canReadGerkonAndIR = false;
public:
  Relays()
  {
      pinMode(releElektromagnet,OUTPUT);
      pinMode(relePitDvig,OUTPUT);
      pinMode(releMoreSpeed,OUTPUT);digitalWrite(releMoreSpeed,HIGH);
      
  }
  void OnElektromagnetRele(int delayElektromagnetReleWork){
    if(canOnElektromagnetRele == true){
      if(firsTimeZamerElektromagnetRele == false){ // Захватили момет старт отсчёта для 500мсек
        digitalWrite(releElektromagnet,HIGH);
        timingElektromagnetRele = millis();
        firsTimeZamerElektromagnetRele = true;
      }  
      if(millis() - timingElektromagnetRele > delayElektromagnetReleWork){
        digitalWrite(releElektromagnet,LOW);
        canOnElektromagnetRele = false;
      }
    }
  }
  void OnPitDvigRele(){
  
        digitalWrite(relePitDvig,HIGH);
       
  }
  StatesEndTime CountTimeAftherOnPitDvigRele(int delayPitDvigReleWork){
    if(canOnPitDvigRele == true){
      if(firsTimeZamerPitDvigRele == false){ // Захватили момет старт отсчёта для 500мсек

            timingPitDvigRele = millis();
        firsTimeZamerPitDvigRele = true;
        
      }  
      if(millis() - timingPitDvigRele > delayPitDvigReleWork){
        //digitalWrite(relePitDvig,LOW);
        canOnPitDvigRele = false;
        timeTickAftherActivatRele2End = true;
        
      }
    }
    if(timeTickAftherActivatRele2End ==true){
      return Ended;
    }
    else{
      return NoEnded;
    }
  }

  void OnMoreSpeedRele(int delayMoreSpeedReleWork){
      if(canOnMoreSpeedRele == true){
          if(firsTimeZamerMoreSpeedRele == false){ // Захватили момет старт отсчёта 
                digitalWrite(releMoreSpeed,LOW);//digitalWrite(releMoreSpeed,HIGH);
                timingMoreSpeedRele = millis();
            firsTimeZamerMoreSpeedRele = true;
          }  
          if(millis() - timingMoreSpeedRele > delayMoreSpeedReleWork){
            digitalWrite(releMoreSpeed,HIGH);//digitalWrite(releMoreSpeed,LOW);
            canOnMoreSpeedRele = false;
          }
    }
  }
bool OnMoreSpeedReleWorkEnd(){
  if(canOnMoreSpeedRele == false){
    return true;
  }
  else{
    return false;
  }
}

  StatesEndTime CountTimeAftherRele3Activated(int delayCountTimeAfthRele3){
    if(canOnCountTimeAfthRele3 == true){
      if(firsTimeZamerCountTimeAfthRele3 == false){ // Захватили момет старт отсчёта для 500мсек

        timingCountTimeAfthRele3 = millis();
        firsTimeZamerCountTimeAfthRele3 = true;
        
      }  
      if(millis() - timingCountTimeAfthRele3 > delayCountTimeAfthRele3){
        //digitalWrite(relePitDvig,LOW);
        canOnCountTimeAfthRele3 = false;
        rele3WorkEnd = true;
        
      }
    }
    if(rele3WorkEnd ==true){
      return Ended;
    }
    else{
      return NoEnded;
    }
  }



  void ResetRelaysSetting(){
    canOnElektromagnetRele = true;
    firsTimeZamerElektromagnetRele = false;

    canOnPitDvigRele = true;
    firsTimeZamerPitDvigRele = false;

    canOnMoreSpeedRele = true;
    firsTimeZamerMoreSpeedRele = false;

    timeTickAftherActivatRele2End = false;
    firsTimeZamerCountTimeAfthRele3 = false;

    canOnCountTimeAfthRele3 = true;
    rele3WorkEnd = false;

    digitalWrite(relePitDvig,LOW);
  }
};
Relays *MyRelaysPtr = nullptr; // Cоздали указатель

class SensorShare{
private:
  int sharCount = -1;
  bool canRead=true;
  bool isBeTimeZaxvat = false;
  unsigned long timingSensorWait=0;
public:
  void IrSensorRead(){
    //Если прочитан шар перед датчиком
    //Прибавить к счётчику шаров +1
    //Подождать 500 мсек до следущего считывания 
    if(canRead == true){
      if( digitalRead(pinIrBallDetector)== 1){
            digitalWrite(pinLaser,LOW);//Выключить лазер
            sharCount++;
            Serial.print("sharCount:"); Serial.println(sharCount);
            //Один раз захват текущее время
            if(isBeTimeZaxvat == false){
                  timingSensorWait = millis(); canRead = false;
              isBeTimeZaxvat = true;
            }
            //Один раз захват текущее время            //  c
            if(sharCount!=4){
            delay(1500);
            }
            else{
              delay(270);
            }
      }
      
      
    }
    if(millis() - timingSensorWait > 2000 and canRead == false){
      canRead = true;
      isBeTimeZaxvat = false;
      digitalWrite(pinLaser,HIGH);//Включить лазер
    }
  }
  int GetShareCount(){
    return sharCount;
  }
  void ResetShareCount(){
    sharCount=0;
  }
};
SensorShare *MySensorSharePtr = nullptr; // Cоздали указатель
void setup() {
  //pinMode(10, OUTPUT);  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
   
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinButtonStart,INPUT_PULLUP);
  pinMode(pinIrBallDetector,INPUT);

  pinMode(pinLaser,OUTPUT);
  digitalWrite(pinLaser,LOW);//Выключить лазер

  //InitMyServo(); // Создали обьект в области оперативной памяти куча heap
  MyUserServoPtr = new MyUserServo_Gercon(); // Создали обьект в области оперативной памяти куча heap
  MyRelaysPtr = new Relays(); // Создали обьект в области оперативной памяти куча heap
  MySensorSharePtr= new SensorShare(); // Создали обьект в области оперативной памяти куча heap
 
}
 
bool start = false;

// constants won't change. They're used here to set pin numbers:
//const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void loop() {

 int reading = digitalRead(pinButtonStart);
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
        start = true;
        Serial.println("Button start pressed");
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;


  if(digitalRead(pinButtonStart)==0){
    start = true;
  }
  if(start == true){
      MyRelaysPtr->OnElektromagnetRele(500);

      MyRelaysPtr->OnPitDvigRele();
      MyRelaysPtr->CountTimeAftherOnPitDvigRele(20000);

      if(MyRelaysPtr->CountTimeAftherOnPitDvigRele(20000) == Ended){
          MyRelaysPtr->OnMoreSpeedRele(30000);
      }

      if(MyRelaysPtr->OnMoreSpeedReleWorkEnd()==true){
          if(MyRelaysPtr->CountTimeAftherRele3Activated(10000) == Ended){
              Serial.print(" I read sensors "); 

              MySensorSharePtr->IrSensorRead();
              MyUserServoPtr->ReadGerkon();
              MyUserServoPtr->DjigDjig();

             // digitalWrite(pinLaser,HIGH);//Включить лазер

              if(MySensorSharePtr->GetShareCount() == 4){
                  //Всё ресет
                  MySensorSharePtr->ResetShareCount();
                  MyRelaysPtr->ResetRelaysSetting();
                  MyUserServoPtr->ResetMooveServo();
                  start = false;
                  digitalWrite(pinLaser,LOW);//Выключить лазер
              }
          }
      }

  }
Serial.print( " start:"); Serial.print( start); 
Serial.print( " digitalRead(pinIrBallDetector):"); Serial.print( digitalRead(pinIrBallDetector) );
Serial.print( " GetShareCount():"); Serial.print( MySensorSharePtr->GetShareCount() ); Serial.println();
}








//MySensorSharePtr->IrSensorRead();

  //Serial.print(" pinIrBallDetector:");
  //Serial.print(digitalRead(pinIrBallDetector));
  // Serial.println();




/*
  if(digitalRead(pinIrBallDetector)==0){
    sharCount++;  //Serial.print(" sharCount:");  Serial.print(sharCount);  Serial.println();
  }
*/

  //  Serial.print(" Gerkon:");
  // put your main code here, to run repeatedly:
  /*
  Serial.print("Button:");
  Serial.print(digitalRead(pinButtonStart));

  Serial.print(" Gerkon:");
  Serial.print(digitalRead(pinGerkon));

  Serial.println();
  */
  /*
  digitalWrite(releElektromagnet,HIGH);
  delay(500);
  digitalWrite(releElektromagnet,LOW);
  delay(500);
  */

  /*
  MyUserServoPtr->DjigDjig();

  MyRelaysPtr->OnElektromagnetRele();
  MyRelaysPtr->OnrelePitDvig();
  */

     //Serial.print(" GetPitDvigReleEnd():");
    //Serial.print(MyRelaysPtr->GetPitDvigReleEnd() );

    //Serial.print(" pinIrBallDetector:");
    //Serial.print(digitalRead(pinIrBallDetector));
    //Serial.println();