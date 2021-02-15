#include <Arduino.h>
#include "MyUserServo.h"

const int8_t pinButtonStart = 4;


const int8_t pinIrBallDetector = 8;

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

  bool rele2WorkEnd = false;
  // Переменные реле Питания Двигателя #2

  // Переменные реле MoreSpeedRele #3
  int delayMoreSpeedReleWork= 30000;
  bool canOnMoreSpeedRele = true;
  bool firsTimeZamerMoreSpeedRele=false;
  unsigned long timingMoreSpeedRele;
  // Переменные реле MoreSpeedRele #3
 
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
  void OnPitDvigRele(int delayPitDvigReleWork){
    if(canOnPitDvigRele == true){
      if(firsTimeZamerPitDvigRele == false){ // Захватили момет старт отсчёта для 500мсек
        digitalWrite(relePitDvig,HIGH);
        timingPitDvigRele = millis();
        firsTimeZamerPitDvigRele = true;
      }  
      if(millis() - timingPitDvigRele > delayPitDvigReleWork){
        //digitalWrite(relePitDvig,LOW);
        canOnPitDvigRele = false;
        rele2WorkEnd = true;
      }
    }
  }
  bool GetPitDvigReleEnd(){
    if(rele2WorkEnd == true){
      return true;
    }
    else{
      return false;
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
  void ResetRelaysSetting(){
    canOnElektromagnetRele = true;
    canOnPitDvigRele = true;
    canOnMoreSpeedRele = true;

    rele2WorkEnd = false;
  }
};
Relays *MyRelaysPtr = nullptr; // Cоздали указатель

void setup() {
     //pinMode(10, OUTPUT);  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)

   
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinButtonStart,INPUT_PULLUP);
 

  pinMode(pinIrBallDetector,INPUT);

  //InitMyServo(); // Создали обьект в области оперативной памяти куча heap
  MyUserServoPtr = new MyUserServo_Gercon(); // Создали обьект в области оперативной памяти куча heap
  MyRelaysPtr = new Relays(); // Создали обьект в области оперативной памяти куча heap

 
}
 
bool start = false;
int sharCount = 0;
void loop() {
  /*
  Serial.print("Button:");
  Serial.print(digitalRead(pinButtonStart));

  Serial.print(" pinIrBallDetector:");
  Serial.print(digitalRead(pinIrBallDetector));
*/

 // Serial.print(" Gerkon:");  Serial.print(digitalRead(pinGerkon));
  MyUserServoPtr->ReadGerkon();
  MyUserServoPtr->DjigDjig();



  if(digitalRead(pinIrBallDetector)==0){
    sharCount++;  //Serial.print(" sharCount:");  Serial.print(sharCount);  Serial.println();
  }




  if(digitalRead(pinButtonStart)==0){
    start = true;
  }
  if(start == true){
      MyRelaysPtr->OnElektromagnetRele(500);
      MyRelaysPtr->OnPitDvigRele(20000);
      if(MyRelaysPtr->GetPitDvigReleEnd() == true){
          MyRelaysPtr->OnMoreSpeedRele(30000);
      }
    Serial.print(" GetPitDvigReleEnd():");
    Serial.print(MyRelaysPtr->GetPitDvigReleEnd() );

    Serial.print(" pinIrBallDetector:");
    Serial.print(digitalRead(pinIrBallDetector));
    Serial.println();
  }
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

}