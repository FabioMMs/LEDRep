// Biblioteca e Constantes LCD 
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);

// Biblioteca SPI
#include "SPI.h"
volatile boolean received;
volatile int Slavereceived;

// Constantes
int led = A5;
volatile int SENSORES = 2;
volatile int APAGA_TUDO = 3;
float rawVal = 0,tempVal1 = 0,rawValT = 0,tenVal = 0,rawValC = 0.0,corVal = 0.0;

//-----------------------------CONFIGURAÇÕES:(LCD,INT0,INT1,I/O)--------------------------------
void setup()
{
  Serial.begin(19200);
  pinMode(led, OUTPUT); // Configura o pino do led (digital) como saída
  pinMode(APAGA_TUDO, INPUT_PULLUP);
  pinMode(SENSORES, INPUT);
  delay(100);

  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  received = false;
  SPI.attachInterrupt();
  delay(100);

  attachInterrupt(digitalPinToInterrupt(APAGA_TUDO), interrupcao_button, LOW); // INT 1 (Botão)
  attachInterrupt(digitalPinToInterrupt(SENSORES), interrupcao, CHANGE); //  INT 0  (Sensores)
  delay(100);
  lcd.begin(16,4);
  lcd.print("Poste 1");

}

ISR (SPI_STC_vect)
{
  Slavereceived = SPDR;
  received = true;
}

void loop()
{
//--------------RELÉ FOTOELETRICO PARA LIGAR LUZ-----------------------------
  if((digitalRead(SENSORES) == LOW) && (digitalRead(APAGA_TUDO) == HIGH) && (Slavereceived == 0))
  {
      digitalWrite(led, HIGH);    
  } 
//-------------------------SENSORES------------------------------------------
  // Sensor de Temperatura
  rawVal = analogRead(A0);
  tempVal1 = map(rawVal, 0, 1023, 0, 500);

  // Sensor de Tensão
  rawValT = analogRead(A1);
  tenVal = map(rawValT, 0, 1023, 0, 500);
  tenVal = tenVal / 100;

  // Sensor de Corrente
  rawValC = analogRead(A2) * (5.0 / 1023.0);
  corVal = (rawValC - 2.5) / 0.1;
//------------------------------DISPLAY------------------------------------------
  // Display Temperatura
  lcd.setCursor(0, 1);
  lcd.print("T(C):");
  lcd.setCursor(6, 1);
  lcd.print(tempVal1);

  // Display Tensão
  lcd.setCursor(0, 2);
  lcd.print("T(mV):");
  lcd.setCursor(6, 2);
  lcd.print(tenVal);

  // Display Corrente
  lcd.setCursor(0, 3);
  lcd.print("C(A):");
  lcd.setCursor(6, 3);
  lcd.print(corVal);

  delay(100);

  if(received)
  {
    if (Slavereceived == 1)
    {
      digitalWrite(led, LOW);
    }
  }
}
//---------------------------------INTERRUPÇÕES-----------------------------------
void interrupcao()
{
    if((digitalRead(SENSORES) == LOW))
    {
      digitalWrite(led, HIGH);    
    }

    else
    {
    digitalWrite(led, LOW); 
    delay(500);
    }
}

void interrupcao_button()
{
    if(digitalRead(APAGA_TUDO) == LOW)
    {
      digitalWrite(led, LOW); // desligar o LED (low = nível lógico alto)
      delay(250);
    }

    else
    {
      digitalWrite(led, HIGH); // Ligar o LED (HIGH = nível lógico alto)
      delay(100);
    }
}

  //---------------------INTERRUPÇÃO  DOS SENSORES -------------------------

