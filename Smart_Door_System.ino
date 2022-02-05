#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = A1;
const int echoPin = A0;
long duration, distance;

#define SS_PIN 10
#define RST_PIN 9

#define LED_G 4 //define green LED pin
#define LED_R 3 //define red LED

#define LED_RB 8
#define LED_Y  7
#define LED_RD 6
#define LED_YD 1

#define PB_1 A2
#define PB_2 A3
int buttonState1 = 0; 
int buttonState2 = 0; 

int counter = 0; 
int newcounter = 0; 

#define BUZZER 5 //buzzer pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(2); //servo pin
  myServo.write(180); //servo start position
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_RB, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_YD, OUTPUT);
  pinMode(LED_RD, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(1, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(PB_1, INPUT);
  pinMode(PB_2, INPUT);

  lcd.init();
  lcd.backlight();
  
  Serial.println("Put your card to the reader...");
  Serial.println();

}
void loop() 
{
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(newcounter);
 // lcd.setCursor(0, 0);
 // lcd.print("Door Closed!");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  delay(50);
    
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  //Serial.println(distance);
  delay(50);

  int buttonState1 = digitalRead(PB_1);
  int buttonState2 = digitalRead(PB_2);

   if (buttonState1 == HIGH) {
    
    digitalWrite(BUZZER, HIGH);
    delay(100);
    
  } else {
    
    digitalWrite(BUZZER, LOW);
  }
  
  if (buttonState2 == HIGH) {

   lcd.clear();
   lcd.setCursor(0, 0);
   newcounter = counter + 1;
   lcd.print(newcounter);
   Serial.println(newcounter);
   counter = newcounter; 
   
   myServo.write(0);
   digitalWrite(BUZZER, HIGH);

   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Door Open!");
   lcd.setCursor(0, 0);
   lcd.print(newcounter);
   lcd.setCursor(0, 1);
   lcd.print(distance);
   delay(1500);
    
  } else {
    
   myServo.write(180);
   digitalWrite(BUZZER, LOW);
   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Door Closed!");
   lcd.setCursor(0, 1);
    lcd.print(distance);
    
  }
  
  if (distance < 80){
   digitalWrite(LED_RD, HIGH);
   }
else{
   digitalWrite(LED_RD, LOW);
}

if (distance < 40){
   digitalWrite(LED_Y, HIGH);
   }
else{
   digitalWrite(LED_Y, LOW);
  }


if (distance < 20){
   digitalWrite(LED_RB, HIGH);
   digitalWrite(BUZZER, HIGH);
   delay(100);
   }
else{
   digitalWrite(LED_RB, LOW);
   digitalWrite(BUZZER, LOW);
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "EA A9 4F 2E") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    newcounter = counter + 1;
    Serial.println(newcounter);
    counter = newcounter;
    myServo.write(0);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, LOW);
    digitalWrite(BUZZER, HIGH);
    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Door Open!");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    
    delay(1500);
    
    myServo.write(180);
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Door Closed!");
   lcd.setCursor(0, 0);
   lcd.print(newcounter);
    lcd.setCursor(0, 1);
    lcd.print(distance);
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);
  }
}
