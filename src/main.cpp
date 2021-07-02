// 5 sec litle sped ,15 sec speed fast, 5 sec little

//01.03.2021+
//03.03.2021 Добавляю софт перезагрузку после каждой игры
//+
#include <Arduino.h>
#include "MyUserServo.h"
#include "ButtonStart.h"
#include "Relays.h"
#include "Timer.h"

void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0

class SensorShare{
private:
  const int8_t pinIrBallDetector = 8;
  const int8_t pinLaser = 3;

  int8_t sharCount = 0;
  bool canRead=true; // Будет ли читатся следующий шар
  bool isBeTimeZaxvat = false;
  unsigned long timingSensorWait=0;
public:
  SensorShare(){
      pinMode(pinIrBallDetector,INPUT); // pinMode(pinIrBallDetector,INPUT_PULLUP);
      pinMode(pinLaser,OUTPUT);
      digitalWrite(pinLaser,LOW);//Выключить лазер
  }
  void IrSensorRead(){
    //Если прочитан шар перед датчиком
    //Прибавить к счётчику шаров +1
 // Serial.print(F("digitalRead(pinIrBallDetector):")); Serial.println(digitalRead(pinIrBallDetector));

    if(canRead == true){
      if( digitalRead(pinIrBallDetector)== 1){
            //digitalWrite(pinLaser,LOW);//Выключить лазер
            sharCount++;
            Serial.print(F("sharCount:")); Serial.println(sharCount);

            //Один раз захват текущее время
            if(isBeTimeZaxvat == false){
                  timingSensorWait = millis(); canRead = false;
                  Serial.println(F("I not read other shars 2sec:"));
              isBeTimeZaxvat = true;
            }
            //Один раз захват текущее время            //
            if(sharCount!=4){
              //delay(1500);
            }
            else{
              delay(500);
            }
      }


    }
    if(millis() - timingSensorWait > 2000 and canRead == false){ // Если прошло уже 2сек и луч был прерван
      canRead = true;
      isBeTimeZaxvat = false;
      digitalWrite(pinLaser,HIGH);//Включить лазер
      Serial.println(F("I read other shars:"));
    }
  }
  int GetShareCount(){
    return sharCount;
  }
  void ResetShareCount(){
    sharCount=0;
    digitalWrite(pinLaser,LOW);//Выключить лазер
  }
};
SensorShare *MySensorSharePtr = nullptr; // Cоздали указатель




void setup() {

  Serial.begin(115200);

  delay(50);
  Serial.println(F("Progaramm started\n"));//Serial.println();

  //InitMyServo(); // Создали обьект в области оперативной памяти куча heap
  MyUserServoPtr = new MyUserServo_Gercon(); // Создали обьект в области оперативной памяти куча heap
  MyRelaysPtr = new Relays(); // Создали обьект в области оперативной памяти куча heap
  MySensorSharePtr = new SensorShare(); // Создали обьект в области оперативной памяти куча heap
  MyButtonStartPtr = new ButtonStart(20);

  MyTimerPtr = new Timer();

  pinMode(3,OUTPUT);
  // digitalWrite(3,HIGH); // Лазер
}

bool oneRazOnLaser = false; // Чтоб включить один раз лазер когда отыграют реле

void loop() {

  //Serial.print("digitalRead(pinIrBallDetector):");
  //Serial.println( digitalRead(8) );
if(MyButtonStartPtr->GetIsPressed() == false){//Если кнопка не была нажата то
    MyButtonStartPtr->Read(); //Читаем постоянно кнопку старт
}

if(MyButtonStartPtr->GetIsPressed() == true){ //Если состояние кнопки старт = НАЖАТА

    // Таймер считает время (Просто информирует)
    MyTimerPtr->CountTime();
    MyTimerPtr->Print();
    // Таймер считает время (Просто информирует)


    // Включаются - выключаются реле
      MyRelaysPtr->OnElektromagnetRele(500); //Вкл реле электромагнит на 500мсек

      MyRelaysPtr->OnPitDvigRele(); //Вкл реле до конца текущей игры
      //MyRelaysPtr->CountTimeAftherOnPitDvigRele(20000);// !! должно работать без этой строки!! Начать считать время от запуска реле реле питания двигателя (20сек)

      if(MyRelaysPtr->CountTimeAftherOnPitDvigRele(5000) == EndedReleTime){  // If time afther
          MyRelaysPtr->OnMoreSpeedRele(15000); //Вкл реле быстрого вращения на 30сек
      }
    // Включаются - выключаются реле

    // Если прошло 5 сек после откл реле высокой скорости то играем. Включается чтение датчиков. Считаются шары до 4х штук
      if(MyRelaysPtr->OnMoreSpeedReleWorkEnd()==true){
          if(MyRelaysPtr->CountTimeAftherRele3Activated(5000) == EndedReleTime){
              //Serial.print(F(" I read sensors ")); Serial.print(F( " GetShareCount():")); Serial.print( MySensorSharePtr->GetShareCount() ); Serial.println();

              // Один раз включить лазер и сделать задержку
              if (oneRazOnLaser == false){
                  digitalWrite(3,HIGH);//Включить лазер
                  delay(20);
                  oneRazOnLaser = true;
              }
              // Один раз включить лазер и сделать задержку

              MySensorSharePtr->IrSensorRead();
              MyUserServoPtr->ReadGerkon();
              MyUserServoPtr->DjigDjig();



              if(MySensorSharePtr->GetShareCount() == 4){

                   if(MyUserServoPtr->GetServoMooveEnded()==true){ // Если серва вернулась в исходное положение
                        //Всё ресет
                        MySensorSharePtr->ResetShareCount();
                        MyRelaysPtr->ResetRelaysSetting();
                        MyUserServoPtr->ResetMooveServo();
                        MyButtonStartPtr->SetResetButton();
                        oneRazOnLaser = false;
                        digitalWrite(3,LOW); //Лазер выкл
                        delay(10);
/*
                        delete MyUserServoPtr;
                        delete MyRelaysPtr;
                        delete MySensorSharePtr;
                        delete MyButtonStartPtr;
                        delete MyTimerPtr;
                        */
                        resetFunc(); //вызываем reset
                   }
              }
          }
      }
    // Если прошло 10сек после откл реле высокой скорости то играем. Включается чтение датчиков. Считаются шары до 4х штук

  }
//Serial.print( " MyButtonStartPtr->GetIsPressed():"); Serial.print(MyButtonStartPtr->GetIsPressed()  );
//Serial.print( " digitalRead(pinIrBallDetector):"); Serial.print( digitalRead(pinIrBallDetector) );

//Serial.print( " GetShareCount():"); Serial.print( MySensorSharePtr->GetShareCount() ); Serial.println();
}
