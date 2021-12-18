#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
HUSKYLENS huskylens;


int R1 = 22;       // 신호등 1 - 빨 , 노 , 초 
int Y1 = 24;
int G1 = 26;

int R2 = 30;        // 신호등 2 - 빨 , 노 , 초 
int Y2 = 32;
int G2 = 34;

int R3 = 38;        // 신호등 2 - 빨 , 노 , 초 
int Y3 = 40;
int G3 = 42;

int R4 = 14;
int G4 = 15;

int R5 = 16;
int G5 = 17; 


// =====================================================

void base()
{

  digitalWrite(R1,0);
  digitalWrite(Y1,0);
  digitalWrite(G1,1);
  
  digitalWrite(R2,0);
  digitalWrite(Y2,0);
  digitalWrite(G2,1);

  digitalWrite(R3,1);
  digitalWrite(Y3,0);
  digitalWrite(G3,0);

  digitalWrite(R4,0);
  digitalWrite(G4,1);''
  8
  digitalWrite(R5,1);
  digitalWrite(G5,0);

   if (!huskylens.request())       // 물체 인식 했음? 
      Serial.println(F("연결 불가 "));
  else if(!huskylens.isLearned()) 
      Serial.println(F("배운 물체 아님"));
  else if(!huskylens.available()) 
      Serial.println(F("화면에 라인이 없음. "));
  else
  {
      Serial.println(F("인식 성공 "));
      while (huskylens.available())
      {
          HUSKYLENSResult result = huskylens.read();
          printResult(result);      //결과 출력     //result.ID 에 인식된 물체 번호 
          husky_T(result);      // 허스키 분류 
      }    
  }
}


void bohangza()
{
  digitalWrite(R1,0);
  digitalWrite(Y1,1);
  digitalWrite(G1,0);
  
  digitalWrite(R2,1);
  digitalWrite(Y2,0);
  digitalWrite(G2,0);

  digitalWrite(R3,1);
  digitalWrite(Y3,0);
  digitalWrite(G3,0);

  digitalWrite(R4,1);
  digitalWrite(G4,0);
  
  digitalWrite(R5,0);
  digitalWrite(G5,1);

  delay(4000);

  base();
  
}

void T_road()
{
  digitalWrite(R1,1);
  digitalWrite(Y1,0);
  digitalWrite(G1,0);
  
  digitalWrite(R2,1);
  digitalWrite(Y2,0);
  digitalWrite(G2,0);

  digitalWrite(R3,0);
  digitalWrite(Y3,1);
  digitalWrite(G3,0);

  digitalWrite(R4,0);
  digitalWrite(G4,1);
  
  digitalWrite(R5,1);
  digitalWrite(G5,0);

  delay(3000);

  base();
}


void left()
{
  digitalWrite(R1,0);
  digitalWrite(Y1,1);     //좌회전 신호  
  digitalWrite(G1,1);
  
  digitalWrite(R2,1);
  digitalWrite(Y2,0);
  digitalWrite(G2,0);

  digitalWrite(R3,1);
  digitalWrite(Y3,0);
  digitalWrite(G3,0);

  digitalWrite(R4,0);
  digitalWrite(G4,1);
  
  digitalWrite(R5,1);
  digitalWrite(G5,0);

  delay(3000);

  base();
  
}

void husky_T(HUSKYLENSResult result)    //허스키 분류 
{
  if ( ((result.xCenter > 0) && (result.xCenter < 160)) && ((result.yCenter > 0) && (result.yCenter < 120)) && (result.ID == 2))    // 보행자 인식 
  {
    bohangza();
  }
  else if ( ((result.xCenter > 160) && (result.xCenter < 320)) && ((result.yCenter > 0) && (result.yCenter < 120)) && (result.ID == 1))    //좌회전 2 인식 
  {
    left();
  }
  else if ( ((result.xCenter > 16) && (result.xCenter < 320)) && ((result.yCenter > 120) && (result.yCenter < 240)) && (result.ID == 1))    //좌회전 3 인식 
  {
    T_road();
  }

  
}

void printResult(HUSKYLENSResult result)        //의미 없음. 그냥 출력함. 
{
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}




// =======================================================


void setup() 
{
  // put your setup code here, to run once:

  pinMode(R1, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(G1, OUTPUT);

  pinMode(R2, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(G2, OUTPUT);

  pinMode(R3, OUTPUT);
  pinMode(Y3, OUTPUT);
  pinMode(G3, OUTPUT);

  pinMode(R4, OUTPUT);
  pinMode(G4, OUTPUT);

  pinMode(R5, OUTPUT);
  pinMode(G5, OUTPUT);

  

  Serial.begin(115200);
  Wire.begin();
  while (!huskylens.begin(Wire))              // 세팅 안될때 
  {
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }

  
}


void loop() {
  // put your main code here, to run repeatedly:

 base();
   
}
