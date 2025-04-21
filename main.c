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
bool is_motor_button_enabled = true;
bool is_wear_belt = false;
bool motorButtonPressed = false;
int kemerkontrol = 0;

void setup() {
  // Pin modlarını ayarlama
  pinMode(motorButton, INPUT);
  pinMode(beltButton, INPUT);
  pinMode(cardoorSwitch, INPUT);

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
  volatile int motorButtonState = digitalRead(motorButton);
  BeltControl();
  doorControl();

  if(motorButtonState == HIGH && is_motor_button_enabled){
      BeltSituation();
      motorButtonState = LOW;
      delay(200);
  }

  //motorButtonState = digitalRead(motorButton);

  if(kemerkontrol == 1 && digitalRead(motorButton) == HIGH ){
     StartMotor();
  }

  temperatureControl();
  lightControl();
  fuelControl();
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
            is_wear_belt = !is_wear_belt; // Durumu tersine çevir
            delay(200);
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
        digitalWrite(buzzer,LOW);
        delay(100);
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
    int temperature = analogRead(temperaturePin); 
    //int sicaklik = temperature;
    int sicaklik = (temperature * 5 / 1023) * 100; // °C’ye çevir

    if(sicaklik > 25){
        char buffer[32];
        lcd.clear();
        sprintf(buffer, "Sicaklik: %d C",temperature);
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
    Serial.println(light_value);
    if(light_value <= 250){
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
      if(is_door_open == HIGH){
        digitalWrite(pinkLED,HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Uyari: Kapi Acik");
        lcd.setCursor(0, 1);
        lcd.print("Motor Calismaz!");
        delay(100);
        is_motor_button_enabled = false;
      }
      else{
        digitalWrite(pinkLED,LOW);
        lcd.clear();
        is_motor_button_enabled = true;
      }
}

void fuelControl(){
  char buffer[32];
  int fuelValue = analogRead(fuelPin); // Potansiyometreden analog değeri oku (0-1023)
  lcd.clear();
  lcd.print(fuelValue);
  delay(100);
  float fuelLevel = ( fuelValue / 1023.0) * 100.0; // % cinsinden yakıt seviyesi
  lcd.clear();
  lcd.print(fuelLevel);
  delay(100);

  if(fuelLevel == 0){
      if(motorStarted){
        StopMotor();
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Yakıt Bitti - Motor Durdu");
      lcd.setCursor(0,1);
      lcd.print("Motor Durdu");
      delay(100);
      digitalWrite(redLED,LOW);
      digitalWrite(blueLED,LOW);
      digitalWrite(yellowLED,LOW);
      digitalWrite(pinkLED,LOW);
  }
  else if(fuelLevel < 5){
      digitalWrite(yellowLED,HIGH);
      digitalWrite(yellowLED,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Kritik: Yakıt");
      lcd.setCursor(0,1);
      sprintf(buffer, "Çok Az - \%%d",fuelLevel);
      lcd.print(buffer);
  }
  else if(fuelLevel < 10){
      digitalWrite(yellowLED,HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Uyari: Dusuk");
      lcd.setCursor(0,1);
      sprintf(buffer,"Yakit\%%d",fuelLevel);
      lcd.print(buffer);
      delay(100);
  }
  /*else{
    lcd.clear();
    lcd.print("Yakıt yüksek");
    delay(100);
  }*/

}