#pragma once
#include <Arduino.h>
#include <Servo.h>


class MyUserServo_Gercon{
private:
  const int8_t pinServo = 6;
  Servo myservo;  // create servo object to control a servo  myservo.write(pos);
  bool canMoove = false;
  unsigned long timingServo; // Переменная для хранения точки отсчета
  int8_t stepServoMoove=0;

const int8_t pinGerkon = 2;
bool pressedGercon = false;
public:
  MyUserServo_Gercon(){ // Конструктор по умолчанию
    myservo.attach(pinServo);  // attaches the servo on pin 9 to the servo object
    SetPrimaryPos();

    pinMode(pinGerkon,INPUT_PULLUP);
  }
  void SetPrimaryPos(){
      myservo.write(5);
      delay(20);
  }
  void ResetMooveServo(){ //Разрешить сделать ещё один раз джик-джик
      canMoove = true;
  }
  void DjigDjig(){ // 0-180 и обратно

        if(canMoove == true){
                if ( (millis() - timingServo > 500) and stepServoMoove ==0){ // Вместо 10000 подставьте нужное вам значение паузы 
                    myservo.write(5); 
                    stepServoMoove =1;
                    Serial.println(F("stepWork = 1"));
                    timingServo = millis(); 
                }
                if ( (millis() - timingServo > 500) and stepServoMoove ==1){ // Вместо 10000 подставьте нужное вам значение паузы 
                    myservo.write(175); 
                    stepServoMoove =2;
                    Serial.println(F("stepWork = 2"));
                    timingServo = millis(); 
                }
                if ( (millis() - timingServo > 500) and stepServoMoove ==2){ // Вместо 10000 подставьте нужное вам значение паузы 
                    myservo.write(5); 
                    stepServoMoove =0;
                    Serial.println(F("stepWork = 3 ComeBack"));
                    timingServo = millis(); 
                    canMoove = false;
                }    
        }
  }
  
  void ReadGerkon(){
    if(digitalRead(pinGerkon) == 0){ //Если геркон "нажат"
      canMoove = true;
    }
  }
};
MyUserServo_Gercon *MyUserServoPtr = nullptr; // Cоздали указатель

void InitMyServo(){
    MyUserServoPtr = new MyUserServo_Gercon(); // Создали обьект в области оперативной памяти куча heap
}