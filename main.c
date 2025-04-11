// Pin tanımlamaları (global kapsamda)

//FOR INPUT 
int engineButton = 2; // DC motor (araç motoru)
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


int buzzer = 9; // Sesli uyarı için buzzer 
int motorPin = 10; // DC motor (araç motoru)
int klimaPin = 11; // DC motor (klima fanı)

// LCD pinleri (örnek olarak)
int lcdRS = 12;
int lcdEN = 13;
int lcdD4 = 14;
int lcdD5 = 15;
int lcdD6 = 16;
int lcdD7 = 17;

// Kütüphane ve nesne tanımlama
#include <LiquidCrystal.h>
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

bool motorStarted = false;
bool is_motor_button_enabled = true;

void setup() {
  // Pin modlarını ayarlama
  pinMode(engineButton, INPUT);
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

  lcd.begin(16, 2); // LCD’yi başlat
}

void loop() {
  int is_motor_button_pressed = digitalRead(engineButton);
  doorControl();
  if(is_motor_button_enabled && is_motor_button_pressed == HIGH){
    BeltControl();
  }
  else {
    if (motorStarted){
        StopMotor();
    }
  }

  temperatureControl();
  lightControl();
  fuelControl();
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

void temperatureControl(){
    int temperature = analogRead(temperaturePin); //
    if(temperature > 25){
        lcd.print("Sıcaklık: %d°C - Klima Açıldı",temperature);
        digitalWrite(klimaPin,HIGH);
    }
    else{
        digitalWrite(klimaPin,LOW);
        lcd.clear();
    }
}

void lightControl(){
    int light_value = analogRead(lightPin); //
    if(light_value <= 250){
        digitalWrite(blueLED,HIGH);
        lcd.print("Farlar Açık");
    }
    else{
        digitalWrite(blueLED,LOW);
        lcd.print("Farlar Kapandı");
    }
}

void doorControl(){
      int is_door_open = digitalRead(cardoorSwitch);
      if(is_door_open){
        digitalWrite(pinkLED,HIGH);
        lcd.print("Uyarı: Kapı Açık - Motor Çalışmaz");
        is_motor_button_enabled = false;
      }
      else{
        digitalWrite(pinkLED,LOW);
        lcd.clear();
        is_motor_button_enabled = true;
      }
}

void fuelControl(){
  int fuelValue = analogRead(fuelPin); // Potansiyometreden analog değeri oku (0-1023)

  float fuelLevel = ( fuelValue / 1023.0) * 100.0; // % cinsinden yakıt seviyesi

  if(fuelLevel == 0){
      if(motorStarted){
        StopMotor();
      }
      lcd.print("Yakıt Bitti - Motor Durdu");
      digitalWrite(redLED,LOW);
      digitalWrite(blueLED,LOW);
      digitalWrite(yellowLED,LOW);
      digitalWrite(pinkLED,LOW);
  }
  else if(fuelLevel < 5){
      digitalWrite(yellowLED,HIGH);
      digitalWrite(yellowLED,LOW);

      lcd.print("Kritik: Yakıt Çok Az - \%%d",fuelLevel);
  }
  else if(fuelLevel < 10){
      digitalWrite(yellowLED,HIGH);
      lcd.print("Uyarı: Yakıt Seviyesi Düşük - \%%d",fuelLevel);
  }

}