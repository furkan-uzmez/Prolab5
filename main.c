// Pin tanımlamaları (global kapsamda)
int motorButton = 2;      // Motor başlat butonu
int kemerButton = 3;      // Emniyet kemeri butonu
int kirmiziLED = 4;       // Emniyet kemeri uyarısı LED’i
int buzzer = 5;           // Sesli uyarı için buzzer
int maviLED = 6;          // Far durumu LED’i
int sariLED = 7;          // Yakıt seviyesi uyarısı LED’i
int rgbRed = 9;           // Kapı durumu için RGB LED (kırmızı pin)
int rgbGreen = 10;        // RGB LED (yeşil pin)
int rgbBlue = 11;         // RGB LED (mavi pin)
int motorPin = 13;        // DC motor (araç motoru ve klima fanı)
int lm35Pin = A0;         // Sıcaklık sensörü (analog)
int ldrPin = A1;          // Işık sensörü (analog)
int potPin = A2;          // Potansiyometre (yakıt seviyesi, analog)
int kapiAnahtar = 8;      // Kapı durumu anahtarı

// LCD pinleri (örnek olarak)
int lcdRS = 12;
int lcdEN = 11;
int lcdD4 = 5;
int lcdD5 = 4;
int lcdD6 = 3;
int lcdD7 = 2;

// Kütüphane ve nesne tanımlama
#include <LiquidCrystal.h>
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

void setup() {
  // Pin modlarını ayarlama
  pinMode(motorButton, INPUT);
  pinMode(kemerButton, INPUT);
  pinMode(kirmiziLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(maviLED, OUTPUT);
  pinMode(sariLED, OUTPUT);
  pinMode(rgbRed, OUTPUT);
  pinMode(rgbGreen, OUTPUT);
  pinMode(rgbBlue, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(kapiAnahtar, INPUT);
  pinMode(lm35Pin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(potPin, INPUT);
  lcd.begin(16, 2); // LCD’yi başlat
}

void loop() {
  // İşlevler burada çalışacak
}