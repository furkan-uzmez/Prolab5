// Pin tanımlamaları (global kapsamda)

//FOR INPUT 
int motorButton = 2; // DC motor (araç motoru)
int beltButton = 3; // Emniyet kemeri butonu
int cardoorSwitch = 4; // Kapı durumu anahtarı

int temperaturePin = A0;  // Sıcaklık sensörü (analog)
int lightPin = A1;          // Işık sensörü (analog)
int fuelPin = A2;          // Potansiyometre (yakıt seviyesi, analog)


//FOR OUTPUT
int redLED = 5; // Emniyet kemeri uyarısı LED’i
int blueLED = 6; // Far durumu LED’i
int yellowLED = 7; // Yakıt seviyesi uyarısı LED’i
int pinkLED = 8; // Kapaının açık olduğunu gösteren RGB LED 


int buzzer = 12; // Sesli uyarı için buzzer 
int motorPin = 10; // DC motor (araç motoru)
int klimaPin = 11; // DC motor (klima fanı)

// LCD pinleri 
int lcdRS = 22; // PA0
int lcdEN = 23; // PA1
int lcdD4 = 24; // PA2
int lcdD5 = 25; // PA3
int lcdD6 = 26; // PA4
int lcdD7 = 27; // PA5

// Kütüphane ve nesne tanımlama
#include <LiquidCrystal.h>
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

bool is_motor_button_enabled = false;
bool motorButtonPressed = false;
int kemerkontrol = 0;
bool is_fuel = false;


void setup() {
  // Pin modlarını ayarlama
  pinMode(motorButton, INPUT);
  pinMode(beltButton, INPUT);
  pinMode(cardoorSwitch, INPUT_PULLUP);

  pinMode(temperaturePin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(fuelPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(pinkLED, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(klimaPin, OUTPUT);
  Serial.begin(9600);

  analogReference(DEFAULT);

  lcd.begin(16, 2); // LCD’yi başlat
}

void loop() {
  int motorButtonState = digitalRead(motorButton);
  fuelControl();
  
  if(is_fuel == true){
      doorControl();
      BeltSituation();
      lightControl();
      temperatureControl();
      delay(20);
      if(kemerkontrol == 1 && motorButtonState == HIGH && is_motor_button_enabled == true){
         StartMotor();
      }
      else{
         StopMotor();
      }
  }

}

void StopMotor(){
  digitalWrite(motorPin,LOW);
}

void StartMotor(){
   digitalWrite(motorPin,HIGH);
}

void BeltSituation(){
    int buttonState = digitalRead(beltButton);
    if(buttonState == LOW){
        digitalWrite(buzzer,HIGH);
        digitalWrite(redLED,HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("KemerTakiliDegil");
        lcd.setCursor(0, 1);
        lcd.print("Motor Calismaz!");
        delay(300);
        kemerkontrol = 0;
    }
    else{
        digitalWrite(buzzer,LOW);
        digitalWrite(redLED,LOW);
        lcd.clear();  
        kemerkontrol = 1;
    }
}

void temperatureControl(){
    char buffer[32];
    int temperature = analogRead(temperaturePin); 
    int sicaklik = ((temperature * 5.0) / 1023.0) * 100.0; // °C’ye çevir
    if(sicaklik > 25){
        lcd.clear();
        sprintf(buffer, "Sicaklik: %d C",sicaklik);
        lcd.setCursor(0,0);
        lcd.print(buffer);
        lcd.setCursor(0,1);
        lcd.print("Klima Acildi!");
        delay(300);
        digitalWrite(klimaPin,HIGH);
    }
    else{
        digitalWrite(klimaPin,LOW);
        lcd.clear();
    }
}

void lightControl(){
    int light_value = analogRead(lightPin); 
    char buffer[32];
    sprintf(buffer, "isik degeri:%d",light_value);
    if(light_value <= 250){
        digitalWrite(blueLED,HIGH);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Farlar Acik");
        lcd.setCursor(0,1);
        lcd.print(buffer);
        delay(300);
    }
    else{
        digitalWrite(blueLED,LOW);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Farlar Kapandi");
        lcd.setCursor(0,1);
        lcd.print(buffer);
        delay(300);
    }
}

void doorControl(){
      int is_door_open = digitalRead(cardoorSwitch);
      if(is_door_open == HIGH){
        is_motor_button_enabled = false;
        digitalWrite(pinkLED,LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Uyari: Kapi Acik");
        lcd.setCursor(0, 1);
        lcd.print("Motor Calismaz!");
        delay(300);
      }
      else{
        digitalWrite(pinkLED,HIGH);
        lcd.clear();
        is_motor_button_enabled = true;
      }
      
}

void fuelControl(){
  int fuelValue = analogRead(fuelPin); // Potansiyometreden analog değeri oku (0-1023)
  float fuelLevel = ( fuelValue / 1023.0) * 100.0; // % cinsinden yakıt seviyesi
  is_fuel = true;
  if(fuelLevel == 0.0){
      is_fuel = false;
      StopMotor();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Yakit Bitti");
      lcd.setCursor(0,1);
      lcd.print("Motor Durdu");
      delay(300);
      digitalWrite(redLED,LOW);
      digitalWrite(blueLED,LOW);
      digitalWrite(yellowLED,LOW);
      digitalWrite(pinkLED,HIGH);
      digitalWrite(buzzer,LOW);
      digitalWrite(klimaPin,LOW);
  }
  else if(fuelLevel < 5.0){
      digitalWrite(yellowLED,HIGH);
      delay(200);
      digitalWrite(yellowLED,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Kritik:YakitCokAz");
      lcd.setCursor(0,1);
      lcd.print(fuelLevel);
      delay(300);
  }
  else if(fuelLevel < 10.0){
      digitalWrite(yellowLED,HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Uyari:DusukYakit");
      lcd.setCursor(0,1);
      lcd.print(fuelLevel);
      delay(300);
  }
  else{
      digitalWrite(yellowLED,LOW);
  }
}