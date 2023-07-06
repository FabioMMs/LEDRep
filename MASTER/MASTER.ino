// Biblioteca e Constantes LCD 
#include <LiquidCrystal.h>
#include "SPI.h"
//#include "PinChangeInterrupt.h"
//#include <Sodaq_PcInt.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);
int SS2 = A4;
int SS3 = A5;
int OFFM = 2;
int valor_botao, x;


void setup() {
  Serial.begin(19200);
  pinMode(OFFM, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(OFFM), interrupcao, CHANGE); //  INT 0
  pinMode(SS2, OUTPUT);
  pinMode(SS3, OUTPUT);
  SPI.begin(); 
  SPI.setClockDivider(SPI_CLOCK_DIV128);    
  digitalWrite(SS,HIGH); 
  digitalWrite(SS2,HIGH); 
  digitalWrite(SS3,HIGH); 
  delay(100);
  lcd.begin(16,4);
  lcd.print("MESTRE");
}

void loop() {
  float Mastersend, Mastereceive;   
  digitalWrite(SS, LOW);
  digitalWrite(SS2, LOW);
  digitalWrite(SS3, LOW); 
  Mastersend = x;
  Mastereceive=SPI.transfer(Mastersend); 
  delay(100);
  digitalWrite(SS, HIGH);
  digitalWrite(SS2, HIGH);
  digitalWrite(SS3, HIGH);
  delay(500);
}

void interrupcao()
{
  lcd.setCursor(0, 2);
  valor_botao = digitalRead(OFFM);
  
  if(valor_botao == HIGH)
  {
    x = 0;
  }
  else
  {
    x = 1;
  }
}
