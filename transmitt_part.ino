#include<LiquidCrystal.h> //библиотека для ЖК дисплея
#include<iarduino_RTC.h> //библиотека для часов реального времени 
#include<RCSwitch.h>

#define button_up 4 // кнопка увеличения 
#define button_down 3 // кнопка уменьшеня 
#define button_ok 5 // кнопка подтверждения
#define button_off 35 // кнопка выключения 
#define button_fastSPIN 42  //кнопка режима "проветривания"
#define button_commonSPIN 44 //кнопка обычного режима
#define commonLED 22 //
#define fastLED 24 //
#define nightLED 23 //

LiquidCrystal lcd(13, 12, 11, 10, 9, 8); // (RS, E, DB4, DB5, DB6, DB7)
iarduino_RTC time(RTC_DS1307);
RCSwitch RFmod = RCSwitch();

int mainState_display = 0;
int modeSET_display = 0;
int modeSET_night = 0;

int buttonState_up = 0;
int buttonState_down = 0;
int buttonState_ok = 0;
int buttonState_off = 0;
int buttonState_fastSPIN = 0;
int flag_long = 0;
int count_long = 0;
int flag_ok = 0;
int count_ok = 0;
int flag_up = 0;
int count_up = 0;
int flag_down = 0;
int count_down = 0;

int minutes_nightStart = 0;
int hours_nigtStart = 22;
int minutes_nightFinish = 0;
int hours_nigtFinish = 8;
int hour_compare = 0;
int minute_compare = 0;
int nightState = 0;

int transTime = 0; // переменная для отчета времени в обычном режиме 
int fastspinState = 0;
int commspinState = 1;
int flag_fast = 0;
int count_fast = 0;
int flag_com = 0;
int count_com = 0;
int backTOcom = 0;
int nightTime = 0;

void setup() {

Serial.begin(115200); // последовательный порт 
Serial.println();

lcd.begin(16, 2);
time.begin();                              // инициируем RTC модуль

  RFmod.enableTransmit(2);

  pinMode(button_up, INPUT); // инициализация кнопки увеличинения 
  pinMode(button_down, INPUT); // инициализация кнопки уменьшения 
  pinMode(button_ok, INPUT); // инициализация кнопки подтверждения 
  pinMode(button_off, INPUT); // инициализация кнопки выключения 
  pinMode(button_fastSPIN, INPUT); // инициализация кнопки режима "проветривания" 
  pinMode(button_commonSPIN, INPUT);    
  pinMode(commonLED, OUTPUT);
  pinMode(fastLED, OUTPUT);
  pinMode(nightLED, OUTPUT);

}

void loop() {


  buttonState_off = digitalRead(button_off);
  buttonState_fastSPIN = digitalRead(button_fastSPIN);
  
  Serial.println(nightTime);
  
  hour_compare = time.Hours;
  minute_compare = time.minutes; 

  
  switch(mainState_display){
    case 0:
      lcd.setCursor(3,1);              // Установка курсора на 3 столб. второй строки
      lcd.print(time.gettime("H:i:s"));       // вывод времени 
      lcd.setCursor(3,0);
      lcd.print("TEMP:");
      delay (10);
    break;
    case 1: 
      lcd.setCursor(3,1);              // Установка курсора на 3 столб. первая строка
      lcd.print(time.gettime("H:i:s"));       // вывод времени 
      lcd.setCursor(0,0);
      lcd.print(" TIME SETTINGS: ");
      delay (5);
    break;
    case 2:
      lcd.clear();
      mainState_display = 3;
      delay(5);
    break;
    case 3:       //режим выставления ночного режима 
      lcd.setCursor(2,0);
      lcd.print("NM starts at:");
      lcd.setCursor(5,1);
      lcd.print(hours_nigtStart);
      lcd.print(":");
      lcd.print(minutes_nightStart);
      lcd.print(":00");
    break;
    case 4:
      lcd.clear();
      mainState_display = 5;
      delay(5);
    break;
    case 5:       //режим выставления ночного режима 
      lcd.setCursor(2,0);
      lcd.print("NM finishes at:");
      lcd.setCursor(5,1);
      lcd.print(hours_nigtFinish);
      lcd.print(":");
      lcd.print(minutes_nightFinish);
      lcd.print(":00");
    break;
    case 6: 
      lcd.clear();
      mainState_display = 0;
      delay (10);
    break;
  }

  buttons_control (); // вызов функции проверки кнопок 
  nightMode();
  communication_command();

}

void buttons_control(){
 
   time.blinktime(modeSET_display); 
   
  
   if (digitalRead(button_fastSPIN) == 1  && flag_fast == 0){
      count_fast++;
      if (count_fast == 1) {
        fastspinState ++;
        flag_fast = 1;}
    } else if (digitalRead(button_fastSPIN) == 0 && flag_fast == 1){
       if(fastspinState > 1){fastspinState == 0;} // возвращаемся к первому устанавливаемому параметру
       count_fast = 0;
       flag_fast = 0;
    }

   if (digitalRead(button_commonSPIN) == 1  && flag_com == 0){
    count_com++;
    if (count_com == 1) {
      commspinState ++;
      flag_com = 1;}
    } else if (digitalRead(button_commonSPIN) == 0 && flag_com == 1){
       count_com = 0;
       flag_com = 0;
    }

  if (commspinState > 1) {
    commspinState = 0;
    }
  if (fastspinState > 1) {
    fastspinState = 0;
    }
    
  if (digitalRead(button_ok) == 1  && flag_long == 0){
    count_long++;
    if (count_long == 3) {
      mainState_display ++;
      flag_long = 1;
      } 
  } else if (digitalRead(button_ok) == 0 && flag_long == 1){
      count_long = 0;
      flag_long = 0;
  }
     
  if (digitalRead(button_ok) == 1  && flag_ok == 0){
    count_ok++;
    if (count_ok == 1) {
      if(mainState_display == 1){
      modeSET_display ++;
           if(mainState_display==1 && modeSET_display>3){modeSET_display=1;}  // возвращаемся к первому устанавливаемому параметру
      }
      if(mainState_display == 3 || mainState_display == 5){
        modeSET_night ++;
        if(mainState_display == 3 && modeSET_night > 2){modeSET_night = 1;}
        if(mainState_display == 5 && modeSET_night > 2){modeSET_night = 1;}
        }
      
      flag_ok = 1;}
  } else if (digitalRead(button_ok) == 0 && flag_ok == 1){
    if (mainState_display == 0){modeSET_display = 0;}
     count_long = 0;
     count_ok = 0;
     flag_ok = 0;
  }
    
    if (digitalRead(button_up) == 1  && flag_up == 0){
      count_up++;
      if (count_up == 1) {
        if (mainState_display == 1){
    switch (modeSET_display){                                     // инкремент (увеличение) устанавливаемого значения
          case 1: time.settime(0,                                   -1, -1, -1, -1, -1, -1); break;
          case 2: time.settime(-1, (time.minutes==59?0:time.minutes+1), -1, -1, -1, -1, -1); break;
          case 3: time.settime(-1, -1, (time.Hours==23?0:time.Hours+1),     -1, -1, -1, -1); break;
          }}
          else if (mainState_display == 3){
            switch(modeSET_night){
              case 1: minutes_nightStart ++; if(minutes_nightStart == 60){minutes_nightStart = 0; lcd.clear();} break;
              case 2: hours_nigtStart ++; if(hours_nigtStart == 24){hours_nigtStart = 0; lcd.clear();} break;
              }
            }
          else if (mainState_display == 5){
            switch(modeSET_night){
              case 1: minutes_nightFinish ++; if(minutes_nightFinish == 60){minutes_nightFinish = 0; lcd.clear();} break;
              case 2: hours_nigtFinish ++; if(hours_nigtFinish == 24){hours_nigtFinish = 0; lcd.clear();} break;
              }
            } 
        flag_up = 1;}
    } else if (digitalRead(button_up) == 0 && flag_up == 1){
       count_up = 0;
       flag_up = 0;
    }
  
    if (digitalRead(button_down) == 1  && flag_down == 0){
      count_down++;
      if (count_down == 1) {
        if (mainState_display == 1){
    switch (modeSET_display){                                     // инкремент (увеличение) устанавливаемого значения
          case 1: time.settime(0,                                   -1, -1, -1, -1, -1, -1); break;
          case 2: time.settime(-1, (time.minutes==0?59:time.minutes-1), -1, -1, -1, -1, -1); break;
          case 3: time.settime(-1, -1, (time.Hours==0?23:time.Hours-1),     -1, -1, -1, -1); break;
          }}
          else if (mainState_display == 3){
            switch(modeSET_night){
              case 1: minutes_nightStart --; if(minutes_nightStart == -1){minutes_nightStart = 59;} break;
              case 2: hours_nigtStart --; if(hours_nigtStart == -1){hours_nigtStart = 23;} break;
              }
            }
          else if (mainState_display == 5){
            switch(modeSET_night){
              case 1: minutes_nightFinish --; if(minutes_nightFinish == -1){minutes_nightFinish = 59;} break;
              case 2: hours_nigtFinish --; if(hours_nigtFinish == -1){hours_nigtFinish = 23;} break;
              }
            }        
        flag_down = 1;}
    } else if (digitalRead(button_down) == 0 && flag_down == 1){
       count_down = 0;
       flag_down = 0;
    } 
}

void communication_command(){
  if(commspinState == 1 && fastspinState == 0){ //условие обычного режима
    if (backTOcom != 0){
       backTOcom = 0;
    }
    digitalWrite(commonLED, 1);
    digitalWrite(fastLED, 0); //
    transTime += 1; // отчет времени для обычного режима 
    if(transTime < 3500){ //услови для запуска вентиляторов 
      RFmod.send(B0010, 4);}
      else if(transTime > 3500 && transTime < 7000){ //услови для запуска вентиляторов 
      RFmod.send(B0101, 4);}//запуск вентиляторов 
    else if(transTime > 7000 && transTime < 21000){ //условие для прекращения работы рекупиратора 
      RFmod.send(B0001, 4);} //прекращение работы рекупиратора 
     else if(transTime > 21000){transTime = 0;} //условие для сброса таймера 
  }else if(commspinState == 1 && fastspinState == 1 && backTOcom == 0){ //
     digitalWrite(fastLED, 1); //
     transTime = 0; //
     commspinState = 0;} //
  else if(commspinState == 1 && fastspinState == 1 && backTOcom == 1){ //
     digitalWrite(commonLED, 1); //
     transTime = 0; //
     fastspinState = 0;}
  else if(commspinState == 0 && fastspinState == 1 && backTOcom == 0){
     backTOcom = 1;
     digitalWrite(commonLED, 0);//
     digitalWrite(fastLED, 1); //
     transTime ++; //
     if(transTime < 3500){ //услови для запуска вентиляторов 
        RFmod.send(B0011, 4);} //    
     else if(transTime >= 3500){ //
        fastspinState = 0,  commspinState == 1;} //
  }else if(fastspinState == 0 && commspinState == 0 && nightState == 0){
     transTime = 0;
     digitalWrite(commonLED, 0);//
     digitalWrite(fastLED, 0);//
     RFmod.send(B0001, 4);
  }
  }

void nightMode(){

  if(hour_compare == hours_nigtStart || hour_compare == hours_nigtFinish){
    if(minute_compare >= minutes_nightStart || minute_compare <= minutes_nightFinish){
      digitalWrite(commonLED, 0);//
      digitalWrite(fastLED, 0);//
      nightState = 1;
      nightTime ++;
      commspinState = 0;
      if (nightTime < 3500){
      RFmod.send(B0100, 4);}
      else if (nightTime > 3500 && nightTime < 7000){
        RFmod.send(B0110, 4);}
        else if (nightTime > 7000 && nightTime < 21000){
          RFmod.send(B0001, 4);}
          else if (nightTime > 21000){nightTime = 0;}
      digitalWrite(nightLED, 1);}
   else if(minute_compare <= minutes_nightStart || minute_compare >= minutes_nightFinish){
      digitalWrite(nightLED, 0);
      nightState = 0;
      communication_command ();
      }
  } else if(hour_compare > hours_nigtStart || hour_compare < hours_nigtFinish){
      digitalWrite(commonLED, 0);//
      digitalWrite(fastLED, 0);//
      nightState = 1;
      commspinState = 0;
      nightTime ++;
      if (nightTime < 3500){
      RFmod.send(B0100, 4);}
      else if (nightTime > 3500 && nightTime < 7000){
        RFmod.send(B0110, 4);}
        else if (nightTime > 7000 && nightTime < 21000){
          RFmod.send(B0001, 4);}
          else if (nightTime > 21000){nightTime = 0;}
      digitalWrite(nightLED, 1);
    } else if(hour_compare < hours_nigtStart || hour_compare > hours_nigtFinish){
      digitalWrite(nightLED, 0);
      nightState = 0;
      communication_command ();
      }
  }






  
