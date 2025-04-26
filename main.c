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

bool motorStarted = false;
bool is_motor_button_enabled = false;
bool is_wear_belt = false;
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
  BeltControl();
  doorControl();
  temperatureControl();
  lightControl();
  
  if(motorButtonState == HIGH && is_motor_button_enabled == true && is_fuel == true){
      BeltSituation();
      delay(200);
  }

  if(kemerkontrol == 1 && digitalRead(motorButton) == HIGH && is_motor_button_enabled == true && is_fuel == true){
     StartMotor();
  }

  
}

void StopMotor(){
  digitalWrite(motorPin,LOW);
  motorStarted = false;
}

void StartMotor(){
   digitalWrite(motorPin,HIGH);
   motorStarted = true;
}

void BeltControl() {
    static int lastButtonState = LOW;
    int buttonState = digitalRead(beltButton);

    if (buttonState != lastButtonState) { // Durum değiştiyse
        delay(50); // Titremeyi önlemek için kısa bir bekleme
        buttonState = digitalRead(beltButton); // Durumu tekrar oku
        if (buttonState == HIGH) {
            is_wear_belt = true; 
            delay(20);
        }
        else{
            is_wear_belt = false; 
            delay(20);
        }
        if(is_motor_button_enabled == true && is_fuel == true){
            BeltSituation();
        }
    }
    lastButtonState = buttonState;
}

void BeltSituation(){
    if(is_wear_belt == false){
        digitalWrite(buzzer,HIGH);
        digitalWrite(redLED,HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("KemerTakiliDegil");
        lcd.setCursor(0, 1);
        lcd.print("Motor Calismaz!");
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
    lcd.clear();
    sprintf(buffer, "Sicaklik: %d C",sicaklik);
    lcd.print(buffer);
    delay(100);
    if(sicaklik > 25){
        lcd.clear();
        sprintf(buffer, "Sicaklik: %d C",sicaklik);
        lcd.setCursor(0,0);
        lcd.print(buffer);
        lcd.setCursor(0,1);
        lcd.print("Klima Acildi!");
        delay(100);
        digitalWrite(klimaPin,HIGH);
    }
    else{
        digitalWrite(klimaPin,LOW);
        lcd.clear();
    }
}

void lightControl(){
    int light_value = analogRead(lightPin); 
    if(light_value <= 250 && is_fuel == true){
        digitalWrite(blueLED,HIGH);
        lcd.clear();
        lcd.print("Farlar Acik");
        delay(100);
    }
    else{
        digitalWrite(blueLED,LOW);
        char buffer[32];
        sprintf(buffer, "isik degeri:%d",light_value);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Farlar Kapandi");
        lcd.setCursor(0,1);
        lcd.print(buffer);
        delay(100);
    }
}

void doorControl(){
      int is_door_open = digitalRead(cardoorSwitch);
      if(is_door_open == HIGH && is_fuel == true){
        is_motor_button_enabled = false;
        digitalWrite(pinkLED,LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Uyari: Kapi Acik");
        lcd.setCursor(0, 1);
        lcd.print("Motor Calismaz!");
      }
      else{
        digitalWrite(pinkLED,HIGH);
        lcd.clear();
        is_motor_button_enabled = true;
      }
      delay(100);
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
      lcd.print("Yakıt Bitti");
      lcd.setCursor(0,1);
      lcd.print("Motor Durdu");
      delay(100);
      digitalWrite(redLED,LOW);
      digitalWrite(blueLED,LOW);
      digitalWrite(yellowLED,LOW);
      digitalWrite(pinkLED,HIGH);
      digitalWrite(buzzer,LOW);
  }
  else if(fuelLevel < 5.0){
      digitalWrite(yellowLED,HIGH);
      delay(200);
      digitalWrite(yellowLED,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Kritik:Yakit");
      lcd.setCursor(0,1);
      lcd.print(fuelLevel);
      delay(100);
  }
  else if(fuelLevel < 10.0){
      digitalWrite(yellowLED,HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Uyari:DusukYakit");
      lcd.setCursor(0,1);
      lcd.print(fuelLevel);
      delay(100);
  }
  else{
      digitalWrite(yellowLED,LOW);
  }
}