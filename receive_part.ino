#include <RCSwitch.h>

#define voltIN 5 // вентилятор на выдув
#define voltOUT 4 // вентилятор на вдув 
#define buttonMODE 3


RCSwitch RFmod = RCSwitch();

int command;  // переменная для хранения полученой команды
int ventMode = 0; // переменная режима (вдув: 1, выдув: 0)


void setup() {
  Serial.begin(115200); // последовательный порт 
  Serial.println();

  RFmod.enableReceive(0);
  

  pinMode(voltIN, OUTPUT); //реле для выдува
  pinMode(voltOUT, OUTPUT); //реле для вдува
  pinMode(buttonMODE, INPUT); //состояние работы вентилятора
}

void loop() {

  ventMode = digitalRead(buttonMODE);

  Serial.println(command);

  if(RFmod.available()){
    
    command = RFmod.getReceivedValue();
    
  Serial.println(command);
    if(ventMode == 0){
      
    switch(command){
      case B0001: //выключено 
        analogWrite(voltIN, 0);
        analogWrite(voltOUT, 0);
      break;
      case B0010: //обычный режим
        analogWrite(voltIN, 0);  
        analogWrite(voltOUT, 500);
      break;
      case B0101: //обычный режим
        analogWrite(voltIN, 500);  
        analogWrite(voltOUT, 0);
      break;
      case B0011: //проветривание 
        analogWrite(voltIN, 1023);  
        analogWrite(voltOUT, 0);
      break;
      case B0100: //ночной режим
        analogWrite(voltIN, 0);  
        analogWrite(voltOUT, 300);
       break;
       case B0110: //ночной режим 
        analogWrite(voltIN, 500);  
        analogWrite(voltOUT, 0);
        break;
      }
    }

    if(ventMode == 1){
    switch(command){
      case B0001:
        analogWrite(voltIN, 0);
        analogWrite(voltOUT, 0);
      break;
      case B0010: //обычный режим
        analogWrite(voltIN, 0);  // Доделать шим
        analogWrite(voltOUT, 500);
      break;
      case B0101: //обычный режим
        analogWrite(voltIN, 500);  // Доделать шим
        analogWrite(voltOUT, 0);
      break;
      case B0011: //проветривание 
        analogWrite(voltIN, 0);  // Доделать шим
        analogWrite(voltOUT, 1023);
      break;
      case B0100: //ночной режим
        analogWrite(voltIN, 0);  
        analogWrite(voltOUT, 300);
      break;
      case B0110: //ночной режим 
        analogWrite(voltIN, 500);  
        analogWrite(voltOUT, 0);
        break;
        
      }
    }
    RFmod.resetAvailable();
   }

}
