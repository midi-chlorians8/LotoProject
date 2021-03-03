#pragma once
#include <Arduino.h>



struct T_time{
       int8_t sec = 0; 
       int8_t min = 0;
       int8_t hour = 0;
       //int AverageMin = 0; // Всё время прошедшее в минутах
};

class Timer{
private:
    unsigned long timing; // Для подсчёта времени таймером

   unsigned long timingPrint = 0;
public:

    bool OneRazCopSEWorkTime = false; 
    T_time TimerTime;
    
    void CountTime() //Cчитаю время с нажатия кнопки
        {

            //if(letsBeginOtvhet == true){
            //   waitOneMin = waitOneMinGlobal; 
                    if (millis() - timing > 1000){ // Вместо 10000 подставьте нужное вам значение паузы 
                        TimerTime.sec+=1; //Serial.println ("10 seconds");
                        

                        //impuls =true;
                        // Перевод в минуты и часы
                        if (TimerTime.sec>59){
                            TimerTime.sec = 0;
                            TimerTime.min +=1;
                            //TimerTime.AverageMin +=1; // Пользуемся этим в таймере save eyes
                            if (TimerTime.min>59){ //Cпециально вложил чтоб меньше проверок было за цикл. Быстрее работало
                                TimerTime.min = 0;
                                TimerTime.hour +=1;
                            }
                        }
                        // Перевод в минуты и часы
                        //Serial.print(TimerTime.hour);Serial.print(":");Serial.print(TimerTime.min); Serial.print(":");Serial.print(TimerTime.sec); Serial.println();
                        timing = millis();                      
                    }
                   
       //     }


    }
    T_time GetTimerTime() const {    //Отдаю время кто попоросит
        return TimerTime;
    }

    void SbrosTimer(){
                TimerTime.sec=0;
                TimerTime.min=0;
                TimerTime.hour=0;

                //TimerTime.AverageMin=0;
        }

    void Print(){   
     
            if (millis() - timingPrint > 1000){ 

                Serial.print(TimerTime.hour);Serial.print(":");
                Serial.print(TimerTime.min); Serial.print(":");
                Serial.print(TimerTime.sec);  Serial.println(" ");

                timingPrint = millis(); 
     
        }
    }
};
Timer *MyTimerPtr = nullptr; // Cоздали указатель