// Bibliotecas 
#include <LiquidCrystal.h>
#include <TimerOne.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);

const int SS1 = 10;
const int SS2 = A4;
const int SS3 = A5;
int OFFM = 2;
int valor_botao, x;

int infos_poste1[5], infos_poste2[5], infos_poste3[5];

void setup() 
{
  // Debbuging
  Serial.begin(19200);

  // Interrupção Botão
  pinMode(OFFM, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(OFFM), interrupcaoBotao, CHANGE); //  INT 0

  // LCD
  lcd.begin(16,4);
  lcd.setCursor(0,0);
  lcd.print("MESTRE");

  // Comunicação SPI
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);

  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

  pinMode(SS1, OUTPUT);
  digitalWrite(SS1, HIGH);

  pinMode(SS2, OUTPUT);
  digitalWrite(SS2, HIGH);

  pinMode(SS3, OUTPUT);
  digitalWrite(SS3, HIGH);

  // Timer
  Timer1.initialize(3000000);
  Timer1.attachInterrupt(timerEscape);
}

void loop()
{
  lcd.setCursor(0,1);
  lcd.println(infos_poste1[2]);
  delay(10);

  lcd.setCursor(0,2);
  lcd.println(infos_poste2[2]);
  delay(10);

  lcd.setCursor(0,3);
  lcd.println(infos_poste3[2]);
  delay(100);
}

void timerEscape()
{
  infoPoste1();
  infoPoste2();
  infoPoste3();
}

void infoPoste1()
{
  digitalWrite(SS1, LOW);
  SPI_MasterTransfer(0x020);
  delay(2);

  infos_poste1[0] = SPI_MasterReception();
  delay(2);

  infos_poste1[1] = SPI_MasterReception();
  delay(2);

  infos_poste1[2] = SPI_MasterReception();
  delay(2);

  infos_poste1[3] = SPI_MasterReception();
  delay(2);

  infos_poste1[4] = SPI_MasterReception();
  delay(2);

  digitalWrite(SS1, HIGH);
}

void infoPoste2()
{
  digitalWrite(SS2, LOW);
  SPI_MasterTransfer(0x020);
  delay(2);

  infos_poste2[0] = SPI_MasterReception();
  delay(2);

  infos_poste2[1] = SPI_MasterReception();
  delay(2);

  infos_poste2[2] = SPI_MasterReception();
  delay(2);

  infos_poste2[3] = SPI_MasterReception();
  delay(2);

  infos_poste2[4] = SPI_MasterReception();
  delay(2);

  digitalWrite(SS2, HIGH);
}

void infoPoste3()
{
  digitalWrite(SS3, LOW);
  SPI_MasterTransfer(0x020);
  delay(2);

  infos_poste3[0] = SPI_MasterReception();
  delay(2);

  infos_poste3[1] = SPI_MasterReception();
  delay(2);

  infos_poste3[2] = SPI_MasterReception();
  delay(2);

  infos_poste3[3] = SPI_MasterReception();
  delay(2);

  infos_poste3[4] = SPI_MasterReception();
  delay(2);

  digitalWrite(SS3, HIGH);
}

void interrupcaoBotao()
{
  valor_botao = digitalRead(OFFM);

  if(valor_botao == LOW)
  {
    digitalWrite(SS1, LOW);
    digitalWrite(SS2, LOW);
    digitalWrite(SS3, LOW);

    SPI_MasterTransfer(0x0AB);

    digitalWrite(SS1, HIGH);
    digitalWrite(SS2, HIGH);
    digitalWrite(SS3, HIGH);
  }

  else
  {
    digitalWrite(SS1, LOW);
    digitalWrite(SS2, LOW);
    digitalWrite(SS3, LOW);

    SPI_MasterTransfer(0x0CD);

    digitalWrite(SS1, HIGH);
    digitalWrite(SS2, HIGH);
    digitalWrite(SS3, HIGH);
  }
}

void SPI_MasterTransfer(byte dado)
{
  SPDR = dado;
  
  while(!(SPSR & (1 << SPIF)));
}

byte SPI_MasterReception()
{
  SPDR = 0xFF;
  while (!(SPSR & (1 << SPIF)));

  delay(10);
  return SPDR;
}

