#include <Servo.h>      // incluye libreria de Servo
#include <DHT.h> // sensor library using lib from https://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LCD library using from  https://www.ardumotive.com/i2clcden.html for the i2c LCD library  


//Constants
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//ALWAYS USE THIS WITH LCD I2C and Addres 0x3F
LiquidCrystal_I2C lcd(0x3F,16,2);

Servo servo1; 
int pos = 0;

int PINSERVO = 8;   // pin 9 conectado a señal del servo
int PULSOMIN = 1000;    // pulso minimo en microsegundos
int PULSOMAX = 2000;    // pulso maximo en microsegundos

unsigned long previousMillis = 0;
const long interval = 10000;    

int RELESOL = 9;      // pin IN de modulo Rele Solido a pin digital 4 de Arduino
int RELEVAL = 10;      // pin IN de modulo Rele Valvula a pin digital 5 de Arduino
int RELELED = 11;      // pin IN de modulo Rele LED a pin digital 6 de Arduino
int Boton = 12;

void setup()
{
  servo1.attach(PINSERVO, PULSOMIN, PULSOMAX);  // inicializacion de servo
  servo1.write(0);
  
  //Serial.begin(9600);
  
  dht.begin();

  lcd.init();
  lcd.backlight();

 pinMode(RELESOL, OUTPUT);  // pin 4 como salida
 pinMode(RELEVAL, OUTPUT);  // pin 5 como salida
 pinMode(RELELED, OUTPUT);  // pin 6 como salida
 pinMode(Boton, INPUT);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (pos == 0) {
      servo1.write(180);
      pos = 180;
    } else {
      servo1.write(0);
      pos = 0;
    }
  }
  
  //Read data and store it to variables hum and temp
  float hum = dht.readHumidity();
  float tem = dht.readTemperature();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(tem,1);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Hum:  ");
  lcd.print(hum,1);
  lcd.print(" %");

    if (tem < 37,5) digitalWrite(RELESOL, HIGH);
   else if (tem > 38) digitalWrite(RELESOL, LOW);

   if (hum < 55) digitalWrite(RELEVAL, HIGH);
   else digitalWrite(RELEVAL, LOW);
 
  //Encender Led en función del valor leido
  if (Boton == HIGH) {
      digitalWrite(RELELED, HIGH);
  }
  else {
      digitalWrite(RELELED, LOW);
  } 
}
