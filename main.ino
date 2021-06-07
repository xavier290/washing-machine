#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int velocidad = 5; // salida pwm para regular la velocidad del motor
int in1 = 2; 
int in2 = 3;

int rojo = 6;
int amarillo = 7;
int verde = 8;
int buzzer = 10;

int marcha = 9; // encendido y apagado de la lavadora
int espera = 2000;

float T;
float intervalo = 0; // control de los tiempos 
int i; // contador por bucle de lavado

void setup() {
  pinMode(velocidad, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(marcha, INPUT);
  pinMode(buzzer,OUTPUT);
  
  lcd.init();
  lcd.backlight();
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Presiona Inicio");
  lcd.setCursor(0,1);
  lcd.print("Para comenzar");

  if(digitalRead(marcha)== HIGH) 
  {
    digitalWrite(amarillo, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Lavado en curso ");
    Lavado();
    Centrifugado();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FIN!              ");
    digitalWrite(buzzer, HIGH);
    delay(espera);
    digitalWrite(buzzer, LOW);
  }
  digitalWrite(amarillo, LOW);
}

void Lavado() 
{
  // empieza la carga de agua
  analogWrite(velocidad, 100); //velocidad baja
  digitalWrite(in1, HIGH);    
  digitalWrite(in2, LOW);
  digitalWrite(verde, HIGH);
  delay(espera);             // tiempo de carga de agua
  digitalWrite(verde, LOW); 

  T = millis()/1000;
  intervalo = 0;            // reiniciamos la cuenta de tiempo
  while (intervalo < 20) {
    intervalo = millis()/1000-T;
  }

  digitalWrite(rojo,HIGH);
  delay(espera);
  digitalWrite(rojo,LOW);

  // vaciado de agua
  digitalWrite(verde, HIGH);
  delay(espera);
  digitalWrite(verde, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);

  T = millis()/1000;
  intervalo = 0;
  while(intervalo < 20) {
    intervalo = millis()/1000 - T;
  }

  digitalWrite(rojo,HIGH);
  delay(espera);
  digitalWrite(rojo, LOW);

  // empieza la seguna carga de agua
  digitalWrite(verde, HIGH);
  delay(espera);
  digitalWrite(verde, LOW);
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  T = millis()/1000;
  intervalo = 0;
  while(intervalo < 20) {
    intervalo = millis()/1000 -T; 
  }

  digitalWrite(rojo, HIGH);
  delay(espera);
  digitalWrite(rojo, LOW);

  analogWrite(velocidad, 0);
}

void Centrifugado() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Centrifugando   ");

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(rojo, HIGH);
  for (i=1;i<=10;i++) {
    analogWrite(velocidad,25*i); //Vamos incrementando la velocidad
    delay(1000);
  }
  for (i=10;i>=0;i--) {
    analogWrite(velocidad,25*i); //Para no frenar en seco
    delay(100);
  }
  
  digitalWrite(in2,HIGH);   //
  digitalWrite(in1,LOW);    //Cambiamos el sentido de giro
  
  //Segunda pasada
  for (i=1;i<=10;i++)
  {
    analogWrite(velocidad,25*i); //Vamos incrementando la velocidad
    delay(1000);
  }
  for (i=10;i>=0;i--)
  {
    analogWrite(velocidad,25*i); //Para no frenar en seco
    delay(100);
  }
  digitalWrite(rojo,LOW);  //Paramos el vaciado
}