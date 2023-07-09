// Bibliotecas 
#include <LiquidCrystal.h>
#include <TimerOne.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);
float corr,corr2,corr3,ten,ten2,ten3;
int temp,temp2,temp3,turno,turno2,turno3,botVal,botVal2,botVal3;
const int SS1 = 10;
const int SS2 = A4;
const int SS3 = A5;
int OFFM = 2;
int valor_botao, x;

float infos_poste1[7], infos_poste2[7], infos_poste3[7];

void setup() 
{
  // Debbuging
  Serial.begin(1800);

  // Interrupção Botão
  pinMode(OFFM, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(OFFM), interrupcaoBotao, CHANGE); //  INT 0

  // LCD
  lcd.begin(20,4);
  lcd.setCursor(7,0);
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
  // ESTADO DAS VARIAVEIS 
  // Parte Decimal dos Valores Float de Corrente e Tensão
  corr=(infos_poste1[3]/10)+(infos_poste1[4]/1000);
  corr2=(infos_poste2[3]/10)+(infos_poste2[4]/1000);
  corr3=(infos_poste3[3]/10)+(infos_poste3[4]/1000);
  ten=(infos_poste1[1]/10)+(infos_poste1[2]/1000);
  ten2=(infos_poste2[1]/10)+(infos_poste2[2]/1000);
  ten3=(infos_poste3[1]/10)+(infos_poste3[2]/1000);

  turno=infos_poste1[5];
  turno2=infos_poste2[5];
  turno3=infos_poste3[5];
  botVal=infos_poste1[6];
  botVal2=infos_poste2[6];
  botVal3=infos_poste3[6];
  temp=infos_poste1[0];
  temp2=infos_poste2[0];
  temp3=infos_poste3[0];

  // DISPLAY poste 1
  lcd.setCursor(0,1);
  lcd.print("P1:");
  lcd.setCursor(3,1);
  lcd.println("T:");
  lcd.setCursor(5,1);
  lcd.println(temp);
  lcd.setCursor(7,1);
  lcd.print("V:");
  lcd.setCursor(9,1);
  lcd.println(ten);
  lcd.setCursor(13,1);
  lcd.print("A:");
  lcd.setCursor(15,1);
  lcd.println(corr);  
  delay(10);
  // Display poste 2
  lcd.setCursor(0,2);
  lcd.print("P2:");
  lcd.setCursor(3,2);
  lcd.println("T:");
  lcd.setCursor(5,2);
  lcd.println(temp2);
  lcd.setCursor(7,2);
  lcd.print("V:");
  lcd.setCursor(9,2);
  lcd.println(ten2);
  lcd.setCursor(13,2);
  lcd.print("A:");
  lcd.setCursor(15,2);
  lcd.println(corr2); 
  delay(10);
  //Display Poste 3;
  lcd.setCursor(0,3);
  lcd.print("P3:");
  lcd.setCursor(3,3);
  lcd.println("T:");
  lcd.setCursor(5,3);
  lcd.println(temp3);
  lcd.setCursor(7,3);
  lcd.print("V:");
  lcd.setCursor(9,3);
  lcd.println(ten3);
  lcd.setCursor(13,3);
  lcd.print("A:");
  lcd.setCursor(15,3);
  lcd.println(corr3); 
  delay(10);


  //Mostra no Serial o Sinal do Botão e do Sensor Relé Fotoeletrico
  //POSTE1///
  Serial.print("P1: ");
  if(turno==0){
    Serial.print("Noite");
  }else{
    Serial.print("Dia");
  }
  Serial.print(" | ");
  Serial.print("BOTAO: ");
  if (botVal == 1)
  {
    Serial.println("Nao Acionado");
  }
  else
  {
    Serial.println("Acionado");
  }
  //POSTE2///
  Serial.print("P2: ");
  if(turno2==0){
    Serial.print("Noite");
  }else{
    Serial.print("Dia");
  }
  Serial.print(" | ");
  Serial.print("BOTAO: ");
  if (botVal2 == 1)
  {
    Serial.println("Nao Acionado");
  }
  else
  {
    Serial.println("Acionado");
  }
  //POSTE3///
  Serial.print("P3: ");
  if(turno3==0){
    Serial.print("Noite");
  }else{
    Serial.print("Dia");
  }
  Serial.print(" | ");
  Serial.print("BOTAO: ");
  if (botVal3 == 1)
  {
    Serial.println("Nao Acionado");
  }
  else
  {
    Serial.println("Acionado");
  }
  Serial.println("");
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

  infos_poste1[5] = SPI_MasterReception();
  delay(2);

  infos_poste1[6] = SPI_MasterReception();
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
  
  infos_poste2[5] = SPI_MasterReception();
  delay(2);

  infos_poste2[6] = SPI_MasterReception();
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

  infos_poste3[5] = SPI_MasterReception();
  delay(2);

  infos_poste3[6] = SPI_MasterReception();
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

