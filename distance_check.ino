#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int trigPin = A1;
const int echoPin = A0;
long duration, distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  lcd.init();
  lcd.backlight();

}

void loop() {
  //digitalWrite(2, LOW);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  Serial.println(distance);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("distanceincm:");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  delay(100);


// speaker
if (distance < 10){
   digitalWrite(2, HIGH);
   }
else{
   digitalWrite(2, LOW);
  }

//led 1 bottom
if (distance < 80){
   digitalWrite(10, HIGH);
   }
else{
   digitalWrite(10, LOW);
}

//led 2 botoom to top
if (distance < 60){
   digitalWrite(11, HIGH);
   }
else{
   digitalWrite(11, LOW);
  }

///led 3 down to top
if (distance < 40){
   digitalWrite(12, HIGH);
   }
else{
   digitalWrite(12, LOW);
  }

///led 4 top
if (distance < 20){
   digitalWrite(13, HIGH);
   }
else{
   digitalWrite(13, LOW);
  }

}
