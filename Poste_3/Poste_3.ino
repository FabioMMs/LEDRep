// Biblioteca e Constantes LCD 
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);
int led=A5;
int RELE_FOTO=3;
int APAGA_TUDO=2;
//---------------------------------INTERRUPÇÕES-----------------------------------
void interrupcao(){
 digitalWrite(led, LOW); // desligar o LED (low = nível lógico alto)
 delay(5000);
 }

void interrupcao_button(){
  digitalWrite(led, digitalRead(APAGA_TUDO)); // desligar o LED (low = nível lógico alto)
  delay(5000);
}

void setup()
{
  digitalWrite(led,HIGH);
  pinMode(led, OUTPUT); // Configura o pino do led (digital) como saída
  attachInterrupt(digitalPinToInterrupt(APAGA_TUDO),interrupcao_button,LOW);// INT 1
  attachInterrupt(digitalPinToInterrupt(RELE_FOTO),interrupcao,LOW);//  INT 0
  delay(100);
  lcd.begin(16,4);
  lcd.print("Poste 3");
}

void loop()
{
//--------------RELÉ FOTOELETRICO PARA LIGAR LUZ-----------------------------
  if((digitalRead(RELE_FOTO)==HIGH)){
      digitalWrite(led, (digitalRead(APAGA_TUDO)));
  }
//-------------------------SENSORES------------------------------------------
  // Sensor de Temperatura
  float rawVal = 0;
  float tempVal = 0;
  rawVal = analogRead(A0);
  tempVal =map(rawVal, 0, 1023, 0, 500);

  // Sensor de Tensão
  float rawValT = 0;
  float tenVal = 0;
  rawValT = analogRead(A1);
  tenVal = map(rawValT, 0, 1023, 0, 500);
  tenVal = tenVal / 100;

  // Sensor de Corrente
  float rawValC = 0.0;
  float corVal = 0.0;
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

  // Display Corrente
  lcd.setCursor(0, 3);
  lcd.print("C(A):");
  lcd.setCursor(6, 3);
  lcd.print(corVal);

  delay(100);
}