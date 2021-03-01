#pragma once
#include <Arduino.h>

 enum StatesEndTime{
  NoEnded,
  EndedReleTime,
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

  bool oneRazSetAndPrint_OnPitDvigRele = false; // Для установки пина в высокий уровень и печати один раз что реле питания двигателя было включено
  bool oneRazPrintIReadSensors = false; // Для печати
public:
  Relays()
  {
      pinMode(releElektromagnet,OUTPUT);
      pinMode(relePitDvig,OUTPUT);
      pinMode(releMoreSpeed,OUTPUT);digitalWrite(releMoreSpeed,HIGH);
      
  }
  void OnElektromagnetRele(uint16_t delayElektromagnetReleWork){
    if(canOnElektromagnetRele == true){
      if(firsTimeZamerElektromagnetRele == false){ // Захватили момет старт отсчёта для 500мсек
          digitalWrite(releElektromagnet,HIGH);
          timingElektromagnetRele = millis();
          Serial.println(F("Electro Magnet Rele On"));
        firsTimeZamerElektromagnetRele = true;
      }  
      if(millis() - timingElektromagnetRele > delayElektromagnetReleWork){
        digitalWrite(releElektromagnet,LOW);
        Serial.println(F("Electro Magnet Rele Off"));
        canOnElektromagnetRele = false;
      }
    }
  }
  void OnPitDvigRele(){
    if(oneRazSetAndPrint_OnPitDvigRele == false){
        Serial.println(F("PitDvig Rele On"));
        digitalWrite(relePitDvig,HIGH);
      oneRazSetAndPrint_OnPitDvigRele = true;
    }
       
  }
  StatesEndTime CountTimeAftherOnPitDvigRele(uint16_t delayPitDvigReleWork){
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
      return EndedReleTime;
    }
    else{
      return NoEnded;
    }
  }

  void OnMoreSpeedRele(uint16_t delayMoreSpeedReleWork){
      if(canOnMoreSpeedRele == true){
          if(firsTimeZamerMoreSpeedRele == false){ // Захватили момет старт отсчёта 
                digitalWrite(releMoreSpeed,LOW);//Включить реле                                  digitalWrite(releMoreSpeed,HIGH);
                Serial.println(F("MoreSpeed Rele On"));
                timingMoreSpeedRele = millis();
            firsTimeZamerMoreSpeedRele = true;
          }  
          if(millis() - timingMoreSpeedRele > delayMoreSpeedReleWork){
            digitalWrite(releMoreSpeed,HIGH);//Выключить реле                                   //digitalWrite(releMoreSpeed,LOW);
            Serial.println(F("MoreSpeed Rele Off"));
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

  StatesEndTime CountTimeAftherRele3Activated(uint16_t delayCountTimeAfthRele3){
    if(canOnCountTimeAfthRele3 == true){
      if(firsTimeZamerCountTimeAfthRele3 == false){ // Захватили момет старт отсчёта для 500мсек

        timingCountTimeAfthRele3 = millis();
        firsTimeZamerCountTimeAfthRele3 = true;
        
      }  
      if(millis() - timingCountTimeAfthRele3 > delayCountTimeAfthRele3){
        //digitalWrite(relePitDvig,LOW);
        canOnCountTimeAfthRele3 = false;
        rele3WorkEnd = true;
        if(oneRazPrintIReadSensors == false){
            Serial.println(F(" I read sensors "));
            oneRazPrintIReadSensors = true;
        }
      }
    }
    if(rele3WorkEnd ==true){
      return EndedReleTime;
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

    digitalWrite(relePitDvig,LOW);Serial.println(F("\nPitDvigRele Rele Off\n"));
    oneRazSetAndPrint_OnPitDvigRele = false;

    oneRazPrintIReadSensors = false;
  }
};
Relays *MyRelaysPtr = nullptr; // Cоздали указатель