#include <Servo.h>      
#include <DHT.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 


//SENSOR TEMPERATURA
#define DHTPIN 4     // Define pin 4 para el sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);   // Initialize DHT sensor for normal 16mhz Arduino

//LCD
LiquidCrystal_I2C lcd(0x3F,16,2);

//SERVO
Servo servo1; 
int pos = 0;

int PINSERVO = 8;   // pin 8 conectado a señal del servo
int PULSOMIN = 1000;    // pulso minimo en microsegundos
int PULSOMAX = 2000;    // pulso maximo en microsegundos

unsigned long previousMillis = 0;
const long interval = 10000;    

//RELES
int RELESOL = 9;      // pin de modulo Rele Solido a pin digital 9 de Arduino
int RELEVAL = 10;      // pin de modulo Rele Valvula a pin digital 10 de Arduino
int RELELED = 11;      // pin de modulo Rele LED a pin digital 11 de Arduino
int Boton = 12;       // pin de Boton a pin digital 12 de Arduino

void setup()
{
  servo1.attach(PINSERVO, PULSOMIN, PULSOMAX);  // inicializacion de servo
  servo1.write(0);
  
  Serial.begin(9600);
  
  dht.begin();

  lcd.init();
  lcd.backlight();

 pinMode(RELESOL, OUTPUT);  // pin 9 como salida
 pinMode(RELEVAL, OUTPUT);  // pin 10 como salida
 pinMode(RELELED, OUTPUT);  // pin 11 como salida
 pinMode(Boton, INPUT);     // pin 12 como entrada
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Modifica el valor a este ultimo
    previousMillis = currentMillis;

    // Si la posicion es 0 se mueve a 180 y viceversa
    if (pos == 0) {
      servo1.write(180);
      pos = 180;
    } else {
      servo1.write(0);
      pos = 0;
    }
  }
  
  //Lee la humedad y la temperatura
  float hum = dht.readHumidity();
  float tem = dht.readTemperature();

  char temp = tem;
  char humi = hum;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp,1);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Hum:  ");
  lcd.print(humi,1);
  lcd.print(" %");

  Serial.print("Temp: ");
  Serial.print(tem,1);
  Serial.print(" C");
  Serial.print(" ");
  Serial.print("Hum:  ");
  Serial.print(hum,1);
  Serial.println(" %");

  //Activa la Resistencia si la temperatura es menor de 37,6
   if (tem < 37,6)
   digitalWrite(RELESOL, HIGH);
   else digitalWrite(RELESOL, LOW);

  //Activa la valvula si la humedad es menor de 50
   if (hum < 50)
   digitalWrite(RELEVAL, HIGH);
   else digitalWrite(RELEVAL, LOW);
 
  //Encender Led en función del valor leido
  if (Boton == HIGH) {
      digitalWrite(RELELED, HIGH);
  }
  else {
      digitalWrite(RELELED, LOW);
  } 
}
