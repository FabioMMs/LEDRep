// Biblioteca e Constantes LCD 
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);

// Biblioteca SPI
volatile boolean received;
volatile int Slavereceived;
byte mensagem;

// Constantes
long int cent;
long int cem,botVal;
int led = A5;
int foto = 0;
volatile int SENSORES = 2;
volatile int APAGA_TUDO = 3;
float rawVal = 0,tempVal = 0,rawValT = 0,tenVal = 0,rawValC = 0.0,corVal = 0.0;

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
  SPCR |= _BV(SPIE);
  received = false;

  attachInterrupt(digitalPinToInterrupt(APAGA_TUDO), interrupcao_button, LOW); // INT 1 (Botão)
  attachInterrupt(digitalPinToInterrupt(SENSORES), interrupcao, CHANGE); //  INT 0  (Sensores)
  delay(100);
  lcd.begin(16,4);
  lcd.print("Poste 1");

}

void loop()
{
//--------------RELÉ FOTOELETRICO PARA LIGAR LUZ-----------------------------
  if((digitalRead(SENSORES) == LOW) && (digitalRead(APAGA_TUDO) == HIGH) && (received == false))
  { 
      digitalWrite(led, HIGH);    
  } 
//-------------------------SENSORES------------------------------------------
  // Sensor de Temperatura
  rawVal = analogRead(A0);
  tempVal = map(rawVal, 0, 1023, 0, 500);

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
  lcd.print(tempVal);

  // Display Tensão
  lcd.setCursor(0, 2);
  lcd.print("T(mV):");
  lcd.setCursor(6, 2);
  lcd.print(tenVal);
  cem=tenVal*10;
  cem=(tenVal*10-cem)*100;

  // Display Corrente
  lcd.setCursor(0, 3);
  lcd.print("C(A):");
  lcd.setCursor(6, 3);
  lcd.print(corVal);
  cent=corVal*10;
  cent=(corVal*10-cent)*100;

  delay(100);
  
  if(digitalRead(foto)==LOW){
    botVal=0;
  }else{
    botVal=1;
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

ISR (SPI_STC_vect)
{
  mensagem = SPI_SlaveReception();

  if (mensagem == 0x0AB)
  {
    received = true;
    digitalWrite(led, LOW);
  }
  else if (mensagem == 0x0CD)
  {
    received = false;
  }
  else if ((mensagem == 0x020) || (mensagem == 0xFF))
  {
    SPI_SlaveTransmission(tempVal);//[0]
    delay(2);
    SPI_SlaveTransmission(tenVal*10);//[1]
    delay(2);
    SPI_SlaveTransmission(cem);//[2]
    delay(2);
    SPI_SlaveTransmission(corVal*10);//[3]
    delay(2);
    SPI_SlaveTransmission(cent);//[4]
    delay(2);
    SPI_SlaveTransmission(botVal);//[5]
    delay(2);
    SPI_SlaveTransmission((digitalRead(APAGA_TUDO)));//[6]
    delay(2);
  }
}

byte SPI_SlaveReception()
{
  while (!(SPSR & (1 << SPIF)));

  return SPDR;
}

void SPI_SlaveTransmission(byte dado)
{
  SPDR = dado;

  delay(10);
  while(!(SPSR & (1 << SPIF)));
}