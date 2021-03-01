//01.03.2021+
#include <Arduino.h>
#include "MyUserServo.h"
#include "ButtonStart.h"
#include "Relays.h"
#include "Timer.h"

class SensorShare{
private:
  const int8_t pinIrBallDetector = 8;
  const int8_t pinLaser = 3;

  int sharCount = -1;
  bool canRead=true; // Будет ли читатся следующий шар
  bool isBeTimeZaxvat = false;
  unsigned long timingSensorWait=0;
public:
  SensorShare(){
      pinMode(pinIrBallDetector,INPUT_PULLUP);
      pinMode(pinLaser,OUTPUT);
      digitalWrite(pinLaser,LOW);//Выключить лазер
  }
  void IrSensorRead(){
    //Если прочитан шар перед датчиком
    //Прибавить к счётчику шаров +1
 // Serial.print(F("digitalRead(pinIrBallDetector):")); Serial.println(digitalRead(pinIrBallDetector));

    if(canRead == true){
      if( digitalRead(pinIrBallDetector)== 0){ 
            digitalWrite(pinLaser,LOW);//Выключить лазер
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
              delay(270);
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

  Serial.begin(115200);delay(50);
  Serial.println(F("Progaramm started\n"));//Serial.println();

  //InitMyServo(); // Создали обьект в области оперативной памяти куча heap
  MyUserServoPtr = new MyUserServo_Gercon(); // Создали обьект в области оперативной памяти куча heap
  MyRelaysPtr = new Relays(); // Создали обьект в области оперативной памяти куча heap
  MySensorSharePtr = new SensorShare(); // Создали обьект в области оперативной памяти куча heap
  MyButtonStartPtr = new ButtonStart(20);

  MyTimerPtr = new Timer();
}
 

void loop() {
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

      if(MyRelaysPtr->CountTimeAftherOnPitDvigRele(2000) == EndedReleTime){
          MyRelaysPtr->OnMoreSpeedRele(3000); //Вкл реле быстрого вращения на 30сек
      }
    // Включаются - выключаются реле

    // Если прошло 10сек после откл реле высокой скорости то играем. Включается чтение датчиков. Считаются шары до 4х штук
      if(MyRelaysPtr->OnMoreSpeedReleWorkEnd()==true){
          if(MyRelaysPtr->CountTimeAftherRele3Activated(1000) == EndedReleTime){
              //Serial.print(F(" I read sensors ")); Serial.print(F( " GetShareCount():")); Serial.print( MySensorSharePtr->GetShareCount() ); Serial.println();

              MySensorSharePtr->IrSensorRead();
              MyUserServoPtr->ReadGerkon();
              MyUserServoPtr->DjigDjig();

             // digitalWrite(pinLaser,HIGH);//Включить лазер

              if(MySensorSharePtr->GetShareCount() == 4){

                   if(MyUserServoPtr->GetServoMooveEnded()==true){ // Если серва вернулась в исходное положение
                        //Всё ресет
                        MySensorSharePtr->ResetShareCount();
                        MyRelaysPtr->ResetRelaysSetting();
                        MyUserServoPtr->ResetMooveServo();               
                        MyButtonStartPtr->SetResetButton();
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
