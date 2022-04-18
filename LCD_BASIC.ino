#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

LiquidCrystal lcd(7, 6, 5, 4, 3,2);

#define RST_PIN 8
#define SS_PIN 10
#define sensorPin1 A2
#define red_light 9


int card1Balance = 5000;
int card2Balance = 300;

int senVal1;

//int red_light = 1;
int intervalRed = 20000;

unsigned long previousRed = 0;

bool redState = LOW;
bool yellowState = LOW;
bool greenState = LOW;

MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup() {
  // put your setup code here, to run once:
lcd.begin(16,2);
//Serial.begin(9600);
 pinMode(9,OUTPUT);
 pinMode(1,OUTPUT);
 pinMode(sensorPin1, INPUT);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.setCursor(0, 0);
  lcd.print(" Automatic Fine");
  lcd.setCursor(0, 1);
  lcd.print("Colection System");
  delay(3000);
  lcd.clear();
}

void loop() {
  unsigned long currentMills = millis();
  // put your main code here, to run repeatedly:





if(currentMills - previousRed >= intervalRed){
  previousRed = currentMills;

  if(redState == LOW){
    redState = HIGH;
  }else{
    redState = LOW;
  }


  
}

if(redState == HIGH){
  digitalWrite(9,HIGH);
  digitalWrite(1,LOW);
  lcd.setCursor(0,0);
lcd.print("STOP");

  sensorRead();

    if (senVal1 == 0)
    {
rfid();
    }
}else{
  digitalWrite(9,LOW);
  digitalWrite(1,HIGH);
     lcd.setCursor(0,0);
lcd.print("GO!!");

digitalWrite(0,HIGH);
}

}


void rfid(){
 if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }




  content.toUpperCase();

  if (content.substring(1) == "B6 49 AA 30")
  {



    if (card1Balance >= 500)
    {
      String card = content.substring(1);
      card1Balance = card1Balance - 500;
      lcd.setCursor(0, 0);
      lcd.print(card);
      lcd.setCursor(0, 1);
      lcd.print("Fined Paid!");
      senVal1 =!digitalRead(sensorPin1);
      delay(2000);
      lcd.clear();
    }
    else
    {
      String card = content.substring(1);
      lcd.setCursor(9, 1);
      lcd.print(card1Balance);
      lcd.print(" Tk");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Recharge"); 
      senVal1 =1;
      delay(1000);
      lcd.clear();
    }
  }
  else if (content.substring(1) == "92 92 49 1E")
  {
    if (card2Balance >= 500)
    {
      String card = content.substring(1);
      lcd.setCursor(0, 0);
      lcd.print(card);
      lcd.setCursor(0, 1);
      lcd.print("No Money");
     senVal1 =1;
       delay(1000);
      lcd.clear();
    }
    else
    {
      String card = content.substring(1);
      lcd.setCursor(0, 0);
      lcd.print(card);
      lcd.setCursor(0, 1);
      lcd.print( "No Money");
       senVal1 =1;




       delay(1000);
      lcd.clear();
    }
  }
  else   {
    lcd.setCursor(0, 0);
    lcd.print("Invalid Card");
     senVal1 = 1;
    delay(1500);
    lcd.clear();
  }


}

void sensorRead(){
  senVal1 = digitalRead(sensorPin1);  
}
