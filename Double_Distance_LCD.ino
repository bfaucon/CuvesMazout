/*
** Example Arduino sketch for SainSmart I2C LCD Screen 16x2
** based on https://bitbucket.org/celem/sainsmart-i2c-lcd/src/3adf8e0d2443/sainlcdtest.ino
** by
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)
　
** This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
** https://bitbucket.org/fmalpartida/new-liquidcrystal 
　
** Modified Ã¢â‚¬â€œ Ian Brennan ianbren at hotmail.com 23-10-2012 to support Tutorial posted to Arduino.cc
　
** Written for and tested with Arduino 1.0
**
** NOTE: Tested on Arduino Uno whose I2C pins are A4==SDA, A5==SCL
　
*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
　
//distance1 sensor
#define trigPin1 10
#define echoPin1 9
//distance2 sensor
#define trigPin2 12
#define echoPin2 11
int maximumRange = 400; // distance1 Maximale acceptée (en cm)
int minimumRange = 0;   // distance1 Minimale acceptée (en cm)
int Haut1 = 154;   // distance1 Minimale acceptée (en cm)
int Haut2 = 154;   // distance1 Minimale acceptée (en cm)
long pi = 3.141516;
long r = 35;
long l = 115;
　
long duration1, distance1, duration2, distance2; // Durée utilisé pour calculer la distance1
long h1, h2, h3;
long vol1, vol2;
　
//Flashing LED on Arduino board
#define LEDPin 13
　
//LCD
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
　
int n = 1;
　
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
　
void setup()
{
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);  //The transmit pin of the ultrasonic sensor
  pinMode(echoPin1, INPUT);   //The receive pin of the ultrasonic sensor
  pinMode(trigPin2, OUTPUT);  //The transmit pin of the ultrasonic sensor
  pinMode(echoPin2, INPUT);   //The receive pin of the ultrasonic sensor
  pinMode(LEDPin, OUTPUT);   //The LED of the Arduinolcd.begin (16,2); //  <<----- My LCD was 16x2
　
  lcd.begin (16,2); //  <<----- My LCD was 16x2
    
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
　
  lcd.print("---F8ASB.COM---");  
  lcd.setCursor (0,1);
  lcd.print("    Le Blog   "); 
}
　
void loop()
{
  // Sonde 1
  // Envoi une impulsion de 10 micro seconde sur la broche "trigger" 
  digitalWrite(trigPin1, LOW); 
  delayMicroseconds(2); 
　
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin1, LOW);
  
  // Attend que la broche Echo passe au niveau HAUT 
  // retourne la durÃ©e
  duration1 = pulseIn(echoPin1, HIGH);
  //Serial.println("duration1");
  //Serial.println(duration1);
  //Calculer la distance1 (en cm, basÃ© sur la vitesse du son).
  distance1 = duration1/58.2;
  
  // Sonde 2
  // Envoi une impulsion de 10 micro seconde sur la broche "trigger" 
  digitalWrite(trigPin2, LOW); 
  delayMicroseconds(2); 
　
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin2, LOW);
  
  // Attend que la broche Echo passe au niveau HAUT 
  // retourne la durÃ©e
  duration2 = pulseIn(echoPin2, HIGH);
  //Serial.println("duration1");
  //Serial.println(duration1);
  //Calculer la distance1 (en cm, basÃ© sur la vitesse du son).
  distance2 = duration2/58.2;
  
  // Si la distance1 mesurÃ©e est HORS des valeurs acceptables
  lcd.clear();
  if (distance1 >= maximumRange || distance1 <= minimumRange)
    {
    /* Envoyer une valeur nÃ©gative sur la liaison sÃ©rie.
       Activer la LED pour indiquer que l'erreur */    
      lcd.setCursor(0,0);
      lcd.print(" STEP AWAY!!!");
    }
  else 
    {
   /* Envoyer la distance1 vers l'ordinateur via liaison sÃ©rie.
      Eteindre la LED pour indiquer une lecture correcte. */
      lcd.setCursor(0,0);
      lcd.print("C1:");
      h1 = (Haut1 - distance1);
      lcd.print(h1);
      lcd.print("cm");
      if (h1 < 35)
        {
          vol1 = (l*(r*r*atan(sqrt(2*r*h1-h1*h1)/(r-h1)) - (r-h1)* sqrt(2*r*h1-h1*h1)))/1000;
          //http://www.nongnu.org/avr-libc/user-manual/group__avr__math.html#ga98384ad60834911ec93ac5ae1af4cf0a
          //view-source:http://richard.poncet1.free.fr/VolumeCuve.php
        }
      else
        {
          if (h1 > 115)
          {
            h3= h1 - 115 + 35;
            vol1 = ((3.141516*r*r + (h3-r)*sqrt(2*r*h3-h3*h3) - r*r*atan(sqrt(2*r*h3-h3*h3)/(h3-r)))+(115-r)*2*r)*l/1000;
          }
          else
          {
            vol1 = ((3.141516*r*r/2)+(h1-r)*2*r)*l/1000;
          }
        }
        lcd.print(" ");
        lcd.print(vol1);
        lcd.print("L");
      }
            
    
  if (distance2 >= maximumRange || distance2 <= minimumRange)
    {
    /* Envoyer une valeur nÃ©gative sur la liaison sÃ©rie.
       Activer la LED pour indiquer que l'erreur */    
      lcd.setCursor(0,1);
      lcd.print(" STEP AWAY!!!");
    }
  else 
    {
   /* Envoyer la distance1 vers l'ordinateur via liaison sÃ©rie.
      Eteindre la LED pour indiquer une lecture correcte. */
      lcd.setCursor(0,1);
      lcd.print("C2:");
      h2 = Haut2 - distance2;
      lcd.print(h2);
      lcd.print("cm");   
      if (h2 < 35)
        {
          vol2 = (l*(r*r*atan(sqrt(2*r*h2-h2*h2)/(r-h2)) - (r-h2)* sqrt(2*r*h2-h2*h2)))/1000;
          //http://www.nongnu.org/avr-libc/user-manual/group__avr__math.html#ga98384ad60834911ec93ac5ae1af4cf0a
          //view-source:http://richard.poncet1.free.fr/VolumeCuve.php
        }
      else
        {
          if (h2 > 115)
          {
            h3= h2 - 115 + 35;
            vol2 = ((3.141516*r*r + (h3-r)*sqrt(2*r*h3-h3*h3) - r*r*atan(sqrt(2*r*h3-h3*h3)/(h3-r)))+(115-r)*2*r)*l/1000;
          }
          else
          {
            vol2 = ((3.141516*r*r/2)+(h2-r)*2*r)*l/1000;
          }
        }
        lcd.print(" ");
        lcd.print(vol2);
        lcd.print("L");       
    }
 
 //Attendre 50ms avant d'effectuer la lecture suivante.
 delay(500);
  
}
　
