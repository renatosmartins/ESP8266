// IRremoteESP8266
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

//BIBLIOTECA LCD
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
#include <DigoleSerial.h>

//PORTA ESP LED
#define IR_LED_Ar1 4  // 4 (D2)
#define IR_LED_Ar2 12 // 12 (D6)

//MODULO RTC
#include <Wire.h>
#include <SPI.h>

#include "RTClib.h"
RTC_DS1307 RTC;
//DateTime now = RTC.now();

//Verificar se o Ar está ligado - se false, desligado
boolean Ar1 = false;
boolean Ar2 = false; 
int count = 0;

//COMANDO CONTROLE REMOTO
IRsend irsend_Ar1(IR_LED_Ar1);  // Set the GPIO to be used to sending the message.
IRsend irsend_Ar2(IR_LED_Ar2);  //PORTA DE ENVIO A SER DEFINIDA D6 - SUGESTÃO

//INICIALIZAR A FUNÇÃO SETUP DO LCD
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 115200); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif

const unsigned char welcomeimage[] PROGMEM = {
    0, 0, 0, 0, 0, 127
    , 0, 8, 1, 2, 0, 127
    , 0, 8, 0, 148, 0, 127
    , 0, 16, 0, 89, 112, 127
    , 3, 144, 0, 16, 144, 127
    , 28, 145, 192, 16, 144, 127
    , 1, 30, 128, 80, 144, 127
    , 9, 49, 3, 208, 144, 127
    , 5, 72, 0, 80, 144, 127
    , 2, 72, 0, 150, 144, 127
    , 3, 8, 0, 152, 208, 127
    , 5, 24, 0, 64, 160, 127
    , 4, 148, 0, 64, 128, 127
    , 8, 36, 0, 128, 128, 127
    , 16, 34, 3, 240, 128, 127
    , 32, 65, 0, 14, 0, 127
    , 0, 129, 128, 1, 252, 127
    , 3, 0, 64, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
};

const unsigned char fonts[] = {6, 10, 18, 51, 120, 123};
const char *fontdir[] = {"0\xb0", "90\xb0", "180\xb0", "270\xb0"};

//COMANDO QUE LIGA O AR CONDICIONADO 1
uint16_t rawLigaAr1[231] = {3020, 9006,  522, 524,  508, 1548,  522, 524,  518, 548, 
                          492, 552,  494, 550,  520, 524,  520, 526,  518, 548,  520, 1514, 
                          520, 548,  486, 558,  520, 1540,  466, 578,  490, 552,  522, 1566, 
                          520, 1516,  518, 1540,  520, 1566,  522, 1514,  522, 548,  490, 552,
                          520, 522,  520, 526,  518, 548,  518, 550,  522, 524,  520, 522,  520,
                          548,  492, 552,  520, 522,  520, 526,  516, 550,  492, 552,  520, 554,
                          512, 524,  506, 564,  546, 498,  520, 524,  520, 524,  518, 548,  490, 552, 
                          520, 522,  520, 526,  518, 548,  492, 552,  520, 524,  520, 524,  542, 552, 
                          492, 552,  520, 522,  522, 526,  510, 1546,  522, 1538,  496, 1540,  520, 1540,  
                          468, 1648,  3000, 9028,  522, 1516,  538, 556,  520, 522,  520, 522,  520, 526,  
                          518, 548,  492, 550,  520, 524,  520, 524,  518, 1540,  520, 524,  518, 526,  508,
                          1550,  520, 524,  520, 1568,  520, 1540,  494, 548,  520, 1540,  520, 1518,  518, 1540, 
                          522, 1540,  490, 1570,  518, 1542,  546, 1516,  494, 1566,  464, 606,  492, 550,  522, 526, 
                          518, 1540,  520, 1516,  510, 1548,  520, 526,  518, 1540,  520, 522,  572, 522,  520, 526,  518, 
                          548,  490, 554,  520, 522,  520, 526,  518, 1540,  520, 524,  516, 1542,  522, 524,  546, 1540,  
                          520, 522,  520, 576,  488, 554,  494, 550,  520, 522,  520, 524,  512, 556,  520, 1516,  586, 1498, 
                          522, 1540,  520, 1540,  494};  // UNKNOWN 969EB59

//COMANDO QUE LIGA O AR CONDICIONADO 2
uint16_t rawLigaAr2[] = {};

//COMANDO QUE DESLIGA O AR CONDICIONADO 1
uint16_t rawDesligaAr1[] = {};

//COMANDO QUE DESLIGA O AR CONDICIONADO 2
uint16_t rawDesligaAr2[] = {};

//Sensor de temperatura usando o LM35
const int LM35 = A0; // Define o pino que lera a saída do LM35
int temperatura; // Variável que armazenará a temperatura medida

//SETUP RTC
void setupRTC(){
    Wire.begin(4,5);
    RTC.begin();    
    delay(500);
    RTC.adjust(DateTime(__DATE__, __TIME__)); 
    delay(1000);
    //Serial.begin(115200); // Khoi dong serial port de lay du lieu
}

//SETUP LCD
void setupLCD() {
    mydisp.begin();
    mydisp.setColor(1);
    mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
    mydisp.clearScreen(); //CLear screen
}
 
//Função que será executada uma vez quando ligar ou resetar o Arduino
void setup() {
   //iniciando as portas 
   irsend_Ar1.begin();
   irsend_Ar2.begin();
   setupRTC();
   setupLCD();
   Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void panico(){
  
    Serial.println(" ");
    Serial.println("Modo pânico. Temperatura acima de 35ºC");
    Serial.println("Envio de comando ligar, cool e tempetura adequada!");
    ligar_Ar1();
    delay(2000);
    ligar_Ar2();
    
    DateTime now = RTC.now(); 
    
    //intervalo de 10 minutos para checar novamente a temperatura
    int tempoInicial = now.hour();
    int tempoFinal = now.hour() - tempoInicial; 

    resetpos();
    mydisp.setPrintPos(0, 0, _TEXT_);
    mydisp.print("Temperatura:"); // display a char array
    mydisp.print(temp());
    mydisp.print("C");    
      
    while(tempoFinal < 2){
      tempoFinal = now.hour() - tempoInicial;
      delay(100);
    }

    if(temp() <= 28){
      Serial.println(" ");
      Serial.println("Saiu com a temperatura inferior a 28ºC - Desativa um dos aparelhos");
      alterna();      
    }
}

int temp(){
  temperatura = (float(analogRead(LM35))*3.3/(1024.0))/0.01;
  return temperatura;      
  delay(100);
}

//LIGAR AR 1
void ligar_Ar1(){
    //comando para ligar o ar condicionado
    Serial.println("Ar 1 em funcionamento");
    irsend_Ar1.sendRaw(rawLigaAr1, 231, 38);
    Ar1 = true;
    digitalClockDisplay();  
    Serial.print("Temperatura: ");
    Serial.print(temp());
    Serial.println(" ºC");
}

//DESLIGA AR 1
void desliga_Ar1(){
    irsend_Ar1.sendRaw(rawDesligaAr1, 300, 38);
    Ar1 = false;       
    Serial.println("Ar 1 desligado");    
    digitalClockDisplay();  
}


//LIGA AR 2
void ligar_Ar2(){
    Serial.println("Ar 2 em funcionamento");
    irsend_Ar2.sendRaw(rawLigaAr2, 231, 38);
    Ar2 = true;
    digitalClockDisplay();  
    Serial.print("Temperatura: ");
    Serial.print(temp());
    Serial.println(" ºC");
}

//DESLIGA AR_2
void desliga_Ar2(){
      irsend_Ar2.sendRaw(rawDesligaAr1, 300, 38);
      Ar2 = false;       
      Serial.println("Ar 2 desligado");
      digitalClockDisplay();  
}

//ALTERAR O AR
void alterna(){
  Serial.println(" ");
  Serial.println("Função Switch");
  if(!Ar1 && Ar2){
      //LIGA AR 1 E DESLIGA O 2
      Serial.println("Ar 1 ligado - Ar 2 Desligado!");
      Serial.println("Data e hora de ligação: ");
      digitalClockDisplay();
      irsend_Ar1.sendRaw(rawLigaAr1, 300, 38);   
      Ar1 = true;
      delay(1000);
      irsend_Ar2.sendRaw(rawDesligaAr2, 300, 38);
      Ar2 = false;
  }
  else if(Ar1 && !Ar2){
      //LIGA AR 2 E DESLIGA O 1 - TANTO SE OS DOIS FOREM TRUE OU O Ar1 FOR TRUE E O DOIS FALSE
      Serial.println("Ar 2 ligado - Ar 1 Desligado!");
      Serial.println("Data e hora de ligação: ");
      digitalClockDisplay();
      irsend_Ar2.sendRaw(rawLigaAr2, 300, 38);
      Ar2 = true;
      delay(1000);
      irsend_Ar1.sendRaw(rawDesligaAr1, 300, 38);
      Ar1 = false;        
  }
  else{
      //SÓ SERVE QUANDO OS DOIS APARELHOS ESTIVEREM LIGADOS
      //USA 2 VEZES O AR 1 E DEPOIS DUAS VEZES O AR 2
      //ZERA O CONTADOR E VOLTA A ULTILIZAR O AR 1 E ASSIM SUCESSIVAMENTE...
      //PARA ALTERNAR QUANDO ESTIVER COM TEMPERATURA SUPERIOR A 30º
      //PODE-SE FAZER UM RADOM PARA SORTEIO DO AR A SER LIGADO (rand() % 2) - '0' PARA O AR 1 E '1' PARA O AR 2;
      if(count <= 2){
        //LIGA AR 2 E DESLIGA O 1 - SE OS DOIS ESTIVEREM LIGADOS
        Serial.println("Ar 2 ligado - Ar 1 Desligado!");
        Serial.println("Data e hora de ligação: ");
        digitalClockDisplay();
        irsend_Ar2.sendRaw(rawLigaAr2, 300, 38);
        Ar2 = true;
        delay(1000);
        irsend_Ar1.sendRaw(rawDesligaAr1, 300, 38);
        Ar1 = false;
        count++;
      }
      else if(count > 2 && count <= 4){
        //LIGA AR 1 E DESLIGA O 2 - SE OS DOIS ESTIVEREM LIGADOS
        Serial.println("Ar 1 ligado - Ar 2 Desligado!");
        Serial.println("Data e hora de ligação: ");
        digitalClockDisplay();
        irsend_Ar1.sendRaw(rawLigaAr1, 300, 38);
        Ar1 = true;
        delay(1000);
        irsend_Ar2.sendRaw(rawDesligaAr2, 300, 38);
        Ar2 = false;
      }
      else{
        count = 0;
        alterna();         
      }
  }
}

void digitalClockDisplay(){
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC); // Năm
  Serial.print('/');
  Serial.print(now.month(), DEC); // Tháng
  Serial.print('/');
  Serial.print(now.day(), DEC); // Ngày
  Serial.print(' ');
  Serial.print(now.hour(), DEC); // Giờ
  Serial.print(':');
  Serial.print(now.minute(), DEC); // Phút
  Serial.print(':');
  Serial.print(now.second(), DEC); // Giây
  Serial.println();
}

//FUNÇÃO PARA CHAVEAR AR CONDICONADO A CADA 6 HORAS
int timer(){
  DateTime now = RTC.now();
  //if(now.hour() % 6 == 0 && now.now.hour() == 00 && second() == 00){ - CONDIÇÃO VÁLIDA TROCA A CADA 6 HORAS
  //################### FUNÇÃO TESTE - FAZ A TROCA A CADA 2 MINUTOS 
  if(now.hour() % 5 == 0 && now.second() == 00 ){ 
      alterna();
  }
}

//########################RESETAR POSIÇÃO CURSOR#######################################
void resetpos(void) //for demo use, reset display position and clean the demo line
{
    mydisp.setPrintPos(0, 0, _TEXT_);
    delay(2000); //delay 2 seconds
    mydisp.println("                "); //display space, use to clear the demo line
    mydisp.setPrintPos(0, 0, _TEXT_);
}

void resetpos1(void) //for demo use, reset display position and clean the demo line
{
    mydisp.setPrintPos(0, 1, _TEXT_);
    delay(2000); //delay 2 seconds
    mydisp.println("                "); //display space, use to clear the demo line
    mydisp.setPrintPos(0, 1, _TEXT_);
}

void LCD(){
    
    //JOGA NO DISPLAY LCD A TEMPERATURA POSIÇÃO 1
    //COLUNA X LINHA X CONTEÚDO
    //JOGA NO DISPLAY LCD A TEMPERATURA POSIÇÃO 1
    resetpos();
    mydisp.setPrintPos(0, 0, _TEXT_);
    mydisp.print("Temperatura:"); // display a char array
    mydisp.print(temp());
    mydisp.print("C");
    
    //JOGA NO DISPLAY DATA + HORA POSIÇÃO 2
    //2º linha
    resetpos1();
    mydisp.setPrintPos(1, 1, _TEXT_);    
    DateTime now = RTC.now();
    mydisp.print(now.day(),DEC);
    mydisp.print("/");
    mydisp.print(now.month(),DEC);
    mydisp.print("/");
    mydisp.print(now.year(),DEC);
    mydisp.print("-");
    mydisp.print(now.hour(),DEC);
    mydisp.print(":");
    mydisp.print(now.minute(),DEC);
    delay(1000);
}

void loop(){
  //LIGAR AR 1 E AR 2 CASO A TEMPERATURA SEJA SUPERIOR A 26
  //COMANDO DE INCIALIZAÇÃO DO SISTEMA
  if(!Ar1){
    Serial.println(" ");
    ligar_Ar1();
  }
  else if(temp() >= 29 && !Ar2){
    Serial.println(" ");
    Serial.println("Temperatura superior a 29ºC - Ativação automática Ar 2");
    Serial.println(" ");
    ligar_Ar2();
  }
  else{
    Serial.println("Os dois 'Aparelhos' estão ligados");
  }

   mydisp.disableCursor(); //DISABLE cursor
   mydisp.clearScreen(); //CLear screen
    
  //VERIFICA TEMPERATURA
  while(true){
    //SE A TEMPERAUTRA FOR MAIOR QUE 29ºC E O AR 2 ESTIVER DESLIGADO, LIGAR 

    //JOGA TEMP E DATA + HORA PARA O DISPLAY 16X2
    
    
    DateTime now = RTC.now();
    LCD();
    Serial.print(now.day(),DEC);
    Serial.print("/");
    Serial.print(now.month(),DEC);
    Serial.print("/");
    Serial.print(now.year(),DEC);
    Serial.print("-");
    Serial.print(now.hour(),DEC);
    Serial.print(":");
    Serial.print(now.minute(),DEC);
    
    if(temp() >= 29 && !Ar2){
      Serial.println(" ");
      Serial.println("Temperatura superior a 29ºC - Ativação automática Ar 2");
      Serial.println(" ");
      ligar_Ar2();
    }

    //SE A TEMPERAUTRA FOR MAIOR QUE 35ºC REENVIA O COMANDO DE LIGAR,
    //GELAR E ESCOLHE NOVAMENTE A TEMPERATURA DOS DOIS 'AR' CONDICIONADOS
    
    if(temp() > 35){
    resetpos1();
    mydisp.setPrintPos(1, 1, _TEXT_);
    mydisp.print("MODO PANICO!!!");
      panico();
    }

    //FUNÇÃO RESPONSÁVEL POR VERIFICAR O TEMPO PARA TROCA DO AR
    timer();
  }
}
