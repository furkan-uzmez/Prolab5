// Pin tanımlamaları (global kapsamda)

//FOR INPUT 
int engineButton = 2; // DC motor (araç motoru ve klima fanı)
int beltButton = 3; // Emniyet kemeri butonu
int cardoorSwitch = 4; // Kapı durumu anahtarı

int lm35Pin = A0;         // Sıcaklık sensörü (analog)
int ldrPin = A1;          // Işık sensörü (analog)
int potPin = A2;          // Potansiyometre (yakıt seviyesi, analog)


//FOR OUTPUT
int redLED = 5; // Emniyet kemeri uyarısı LED’i
int blueLED = 6; // Far durumu LED’i
int yellowLED = 7; // Yakıt seviyesi uyarısı LED’i
int pinkLED = 8; // Kapaının açık olduğunu gösteren RGB LED 


int buzzer = 9; // Sesli uyarı için buzzer 
int motorPin = 10; // DC motor (araç motoru ve klima fanı)

// LCD pinleri (örnek olarak)
int lcdRS = 11;
int lcdEN = 12;
int lcdD4 = 13;
int lcdD5 = 14;
int lcdD6 = 15;
int lcdD7 = 16;

// Kütüphane ve nesne tanımlama
#include <LiquidCrystal.h>
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

bool motorStarted = false;

void setup() {
  // Pin modlarını ayarlama
  pinMode(engineButton, INPUT);
  pinMode(beltButton, INPUT);
  pinMode(cardoorSwitch, INPUT);

  pinMode(lm35Pin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(potPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(pinkLED, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(motorPin, OUTPUT);
  
  lcd.begin(16, 2); // LCD’yi başlat
}

void loop() {
  int is_motor_button_pressed = digitalRead(engineButton);
  if(is_motor_button_pressed == HIGH){
    BeltControl();
  }
  else {
    if (motorStarted){
        StopMotor();
    }
  }

}

void StopMotor(){
  digitalWrite(motorPin,LOW);
  motorStarted = false;
}

void BeltControl(){
    int is_wear_belt = digitalRead(beltButton);
    if(is_wear_belt == LOW){
        digitalWrite(buzzer,HIGH);
        digitalWrite(redLED,HIGH);
        lcd.clear();
        lcd.print("Emniyet Kemeri Takılı Değil!");
        delay(1000);
        digitalWrite(buzzer,LOW);
    }
    else{
        digitalWrite(buzzer,LOW);
        digitalWrite(redLED,LOW);
        lcd.clear();
        digitalWrite(motorPin,HIGH);
        motorStarted = true;
    }
}