// Biblioteca e Constantes LCD 
#include <LiquidCrystal.h>


const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);

const int SS1 = 10;
const int SS2 = A4;
const int SS3 = A5;
int OFFM = 2;
int valor_botao, x;


void setup() 
{
  // Debbuging
  Serial.begin(19200);

  // Interrupção Botão
  pinMode(OFFM, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(OFFM), interrupcaoBotao, CHANGE); //  INT 0

  // LCD
  lcd.begin(16,4);
  lcd.print("MESTRE");

  // Comunicação SPI
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);

  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

  pinMode(SS1, OUTPUT);
  digitalWrite(SS1, HIGH);

  // pinMode(SS2, OUTPUT);
  // digitalWrite(SS2, HIGH);

  // pinMode(SS3, OUTPUT);
  // digitalWrite(SS3, HIGH);
}

void loop()
{

}

void interrupcaoBotao()
{
  lcd.setCursor(0, 2);
  valor_botao = digitalRead(OFFM);

  if(valor_botao == LOW)
  {
    digitalWrite(SS1, LOW);
    SPI_MasterTransfer(0x0AB);
    digitalWrite(SS1, HIGH);
  }

  else
  {
    digitalWrite(SS1, LOW);
    SPI_MasterTransfer(0x0CD);
    digitalWrite(SS1, HIGH);
  }
}

void SPI_MasterTransfer(byte dado)
{
  SPDR = dado;
  
  while(!(SPSR & (1 << SPIF)));
}

// byte SPI_MasterReception()
// {
//   SPDR = 0xFF;
//   while (!(SPSR & (1 << SPIF)));

//   return SPDR;
// }

