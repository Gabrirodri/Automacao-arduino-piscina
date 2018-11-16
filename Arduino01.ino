/****************************************************************************************
  TCC - AUTOMAÇÃO DE PISCINA
*****************************************************************************************/ 

/****************************************************************************************
  Bibliotecas
*****************************************************************************************/ 
#include "Wire.h"              // Biblioteca - I2C – Protocolo de Comunicação
#include <LiquidCrystal_I2C.h> //Biblioteca do visor LCD i2C
#include <TimeLib.h>           //Biblioteca do RTC 
#include <TimerOne.h>    
#include <EEPROM.h>  //Importar a EEPROM library.
#include <DS1307RTC.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//********************** PARAMETRIZAÇÃO DO PROJETO ***********************
#define nome "TCC - automacao piscina"

#define DS1307_ADDRESS 0x68    // Modulo RTC no endereco 0x68
#define tempoContador  50;

//********************** FINAL DA PARAMETRIZAÇÃO DO PROJETO ***********************

//CONFIGURACAO DO DISPLAY LCD com modulo I2C
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE); // Modulo I2C display no endereco 0x27
byte zero = 0x00; 

//********************************************************************************


//Parametros de temperatura e ph
const int pinPo = A0;; //pino recebe valor de  PO - PH
int H, M, S;  //Variáveis para alteração da hora do relógio.
int temp_sensor = 10; // pino onde se encontra o sensor de temperatura
int contador = 0; //contador - usado pelo timer1
float temperature = 25;// para nao ligar aquecedor - temperatura padrão... espera até receber sinal do sensor para atualizar o valor
OneWire oneWirepin(temp_sensor);

DallasTemperature sensors(&oneWirepin);

// funções do programa
void principal();
void PHTemp();
void SelecionaDataeHora();
void contaTempo();
void testerele();

void setup()           // configuração inicial
{ 
  Wire.begin();       //inicie a biblioteca Wire e barramento I2C
 lcd.backlight();     //Inicia o visor LCD com a luz ligada
 lcd.begin (16,2); 
 lcd.clear();         //apaga LCD
 Serial.begin(9600);  //Inicia a porta serial

     //A linha de comando abaixo serve para que você possa configurar a hora e data do seu modulo RTC
     //SelecionaDataeHora(); 
  
//colocando pinos digitais em configuração de saida de sinal
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(5, OUTPUT);
 pinMode(6, OUTPUT);
 pinMode(7, OUTPUT);

 //neste codigo a logica  das saidas estao invertidas 
 
// mudar estados dos pinos para desligado em 0 ou ligado em 1
 digitalWrite(2, 1); // Cloração
 digitalWrite(3, 1); // Filtração
 digitalWrite(4, 0); // INDICA SISTEMA LIGADO
 digitalWrite(5, 1); // AQUECEDOR
 digitalWrite(6, 1); //PH ALTO
 digitalWrite(7, 1); //PH BAIXO
  
  //INICIO DO TIMER
  Timer1.initialize(1000000);         //TIMER IRA EXECUTAR A CADA 1 SEGUNDO (PARAMETRO EM MICROSEGUNDOS)
  Timer1.attachInterrupt(contaTempo); //aponta para função de interrupção por timer1
  contador = tempoContador;           //contador recebe parametros de tempo pré-definidos no inicio
  Timer1.start();
  Timer1.attachInterrupt(contaTempo); //aponta para função de interrupção por timer1
  
setSyncProvider(RTC.get);     // sincronização do RTC

if (timeStatus() != timeSet) { // caso nao sincronizar ele executa esse if
    //Escreve no LCD:
    lcd.print("BAT. do RTC INOP"); // Diz que a bateria esta inoperante
    //Atraso para o próximo passo:
    delay(2000);
    //Apaga a informação do LCD.
    lcd.clear();
    //Alteração dos dados de hora e data.
    H = 1;
    M = 1;
    S = 1;
    //Grava no RTC uma nova hora e data.
    setTime(H, M , S , 1 , 10 , 2001 ); // grava essa data e essa hora no RTC... indicando que precisa ser ajustado novamente
    RTC.set(now());
  }
  else {
    //Escreve no LCD:
    lcd.print("RTC OK");    
    //Atraso para o próximo passo:
    delay(1000);
    //Apaga a informação do LCD.
    lcd.clear();
}
}

void contaTempo() 
 {
  Serial.println(" Tmp: ");
  Serial.println(contador);
  contador--;  
 
  /*Se temperatura acima de 25° = temperatura ideal
  Se estiver abaixo de 24º entao ligar aquecedor(LED)*/
  if (temperature <= 24)
 {
 digitalWrite(5, 0); //LIGAR AQUECEDOR
 }
 
  if(temperature >=25)
 {
 digitalWrite(5, 1); //DESLIGAR AQUECEDOR
 }

  
  if (contador >= 40 && contador <= 41)
  {
   digitalWrite(2, 0); // Cloração
   digitalWrite(3, 0); // Filtração  
  }
  if (contador >= 30 && contador <= 31)
  {
   digitalWrite(2, 1); // Cloração
  }
  if (contador >= 20 && contador <= 21)
  {
   digitalWrite(2, 0); // Cloração
  }
  if (contador >= 10 && contador <= 11)
  {
   digitalWrite(2, 1); // Cloração
   digitalWrite(3, 1); // Filtração  
  }
  if (contador >= 0 && contador <= 1)
  {
  contador=0;
  Serial.println("RESET");
  contador = tempoContador;
  Serial.println(contador); 
  Timer1.start();
  Timer1.attachInterrupt(contaTempo);  
  }
}

void loop()
{
 int i=0;
 for (i=0;i<5;i++) //este for serve para prender a tela inicial em laço de repetição, fazendo mudar apos 5 segundos... nao alterando a contagem de tempo...
 {
  principal();
  delay(1000);
 }

lcd.clear(); 
PHTemp();
delay(300);
PHTemp();
delay(300);
PHTemp();
delay(300);
PHTemp();
delay(300);
PHTemp();
delay(300);
PHTemp();
delay(300);
PHTemp();
delay(500);
i=0;
}


void principal()
{
 lcd.clear(); // apagar tela anterior
 // Le os valores (data e hora) do modulo DS1307
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero);
 Wire.endTransmission();
 Wire.requestFrom(DS1307_ADDRESS, 7);
 int segundos = ConverteparaDecimal(Wire.read());
 int minutos = ConverteparaDecimal(Wire.read());
 int horas = ConverteparaDecimal(Wire.read() & 0b111111);
 int diadasemana = ConverteparaDecimal(Wire.read()); 
 int diadomes = ConverteparaDecimal(Wire.read());
 int mes = ConverteparaDecimal(Wire.read());
 int ano = ConverteparaDecimal(Wire.read());
 
 // Mostra os dados no display
 lcd.setCursor(0,0);
 //lcd.print(" ");
 // Acrescenta o 0 (zero) se a hora for menor do que 10
 if (horas <10)
 lcd.print("0");
 lcd.print(horas);
 lcd.print(":");
 // Acrescenta o 0 (zero) se minutos for menor do que 10
 if (minutos < 10)
 lcd.print("0");
 lcd.print(minutos);
 lcd.print(":");
 // Acrescenta o 0 (zero) se minutos for menor do que 10
 if (segundos < 10)
 lcd.print("0");
 lcd.print(segundos);
 lcd.setCursor(0,1);

 // Mostra o dia da semana
 switch(diadasemana)
 {
 case 0:lcd.print("Dom");
 break;
 case 1:lcd.print("Seg");
 break;
 case 2:lcd.print("Ter");
 break;
 case 3:lcd.print("Qua");
 break;
 case 4:lcd.print("Qui");
 break;
 case 5:lcd.print("Sex");
 break;
 case 6:lcd.print("Sab");
 }
 
 lcd.setCursor(4,1);
 // Acrescenta o 0 (zero) se dia do mes for menor do que 10
 if (diadomes < 10)
 lcd.print("0");
 lcd.print(diadomes);
 lcd.print("/");
 // Acrescenta o 0 (zero) se mes for menor do que 10
 if (mes < 10)
 lcd.print("0");
 lcd.print(mes);
 lcd.print("/");
 lcd.print(ano);
}

void PHTemp()
{ 

  lcd.setCursor(0,0);
  lcd.print("PH:");
  lcd.setCursor(0,1);


 // cria variáveis ​​auxiliares
 
  int campo[10];
  int aux1;
  unsigned long int medicao = 0;
  
// Lendo dez amostras por 10 ms

for (int i = 0; i < 10; i++) {
    campo[i] = analogRead(pinPo);
    delay(10);
  }
  // Classificar membros do campo de resultados medidos por tamanho
  
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (campo[i] > campo[j]) {
        aux1 = campo[i];
        campo[i] = campo[j];
        campo[j] = aux1;
      }
    }
  }
  
  // Salva do 2º ao 8º resultado
  // variável a partir da qual a média é calculada
  // (pule dois membros do campo no início
  // e final para melhor precisão)
  
  for (int i = 2; i < 8; i++) {
    medicao += campo[i];
  }

// Calcula o valor do pH da média
  // Medição e conversão para faixa de pH de 0 a 14
  
  float medicaoPH = (float) medicao * 5.0 / 1024 / 6;
  float valorPH = -4.70 * medicaoPH + 21.34;
// imprime os resultados na linha serial

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  
  Serial.print(temperature);
  
  delay(10); 
  //Serial.print(" pH:");
 // Serial.print(valorPH);
  
  lcd.setCursor(4,0);
  lcd.print(valorPH);
  lcd.setCursor(9,0);
  lcd.print("Temp:");
  lcd.setCursor(14,0);
  lcd.print(temperature);
  
    /*Se Ph estiver entre 7 e 7,8 = ph ideal
  Se o ph estiver acima de 7,8 = ligar bomba (ph diminuir)
  Se o ph estiver abaixo de 7 = ligar bomba de (ph aumentar)*/
  
  if(valorPH >= 7 && valorPH <=7.8)
 {
  lcd.setCursor(0,2);
  lcd.print("PH IDEAL"); 
 digitalWrite(6, 1); //MENOS GENCO
 digitalWrite(7, 1); //MAIS GENCO
 }
 if (valorPH >= 7.9)
 {
 lcd.setCursor(0,2);
 lcd.print("PH ALTO  ");  
 digitalWrite(6, 0); //MENOS GENCO
 digitalWrite(7, 1); //MAIS GENCO
 }
 if(valorPH <= 6.9)
 {
 lcd.setCursor(0,2);
 lcd.print("PH BAIXO");  
 digitalWrite(6, 1); //MENOS GENCO
 digitalWrite(7, 0); //MAIS GENCO
 }
  

}
 
void SelecionaDataeHora() //Seta a data e a hora do DS1307
{
 byte segundos = 30; // Valores de 0 a 59
 byte minutos = 25; //Valores de 0 a 59
 byte horas = 11; //Valores de 0 a 23
 byte diadasemana = 0; //Valores de 0 a 6 - 0=Domingo, 1 = Segunda, etc.
 byte diadomes = 11; //Valores de 1 a 31
 byte mes = 11; //Valores de 1 a 12
 byte ano = 18; //Valores de 0 a 99
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero); //Stop no CI para que o mesmo possa receber os dados
 
 //As linhas abaixo escrevem no CI os valores de 
 //data e hora que foram colocados nas variaveis acima
 Wire.write(ConverteParaBCD(segundos));
 Wire.write(ConverteParaBCD(minutos));
 Wire.write(ConverteParaBCD(horas));
 Wire.write(ConverteParaBCD(diadasemana));
 Wire.write(ConverteParaBCD(diadomes));
 Wire.write(ConverteParaBCD(mes));
 Wire.write(ConverteParaBCD(ano));
 Wire.write(zero);
 Wire.endTransmission(); 
}

byte ConverteParaBCD(byte val)
{ 
 //Converte o número de decimal para BCD
 return ( (val/10*16) + (val%10) );
}
 
byte ConverteparaDecimal(byte val) 
{ 
 //Converte de BCD para decimal
 return ( (val/16*10) + (val%16) );
}

