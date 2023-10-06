#include "RTClib.h"
#include <Wire.h>

const int ledPin =  2;
const int errorLedPin = LED_BUILTIN;
int color = 0;
enum colors {Off, White, Red, Mixed};

RTC_DS3231 rtc;

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

void onChangeColor(int target) {
  while(color != target){
    Serial.println("Apagando");
    digitalWrite(ledPin, HIGH);
    delay(2000);
    Serial.println("Acendendo");
    digitalWrite(ledPin, LOW);
    if (color == 3){
      color = 1;
    }else{
      color++;
    }
    delay(1500);
  }
}

void off() {
  Serial.println("Encerrando o dia!");
  digitalWrite(ledPin, HIGH);
  color = Off;
}

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(errorLedPin, OUTPUT);

  Serial.begin(9600); //INICIALIZA A SERIAL
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    digitalWrite(errorLedPin, HIGH);
    while(1); //SEMPRE ENTRE NO LOOP
  }
  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    rtc.adjust(DateTime(2022, 7, 12, 7, 31, 50)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  digitalWrite(errorLedPin, LOW);
  off();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //Serial.println("Hora ajustada!");
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
}

void loop() {
  
  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
    Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    Serial.print(" / Dia: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print(" / Horas: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    Serial.println(); //QUEBRA DE LINHA NA SERIAL
    delay(10000); //INTERVALO DE 1 SEGUNDO
  
    // if the LED is off turn it on and vice-versa:
    if ((now.hour() == 7 || now.hour() == 16) && color != Red ) {
      Serial.println("Mudando para vermelho");
      onChangeColor(Red);
    } else if(((now.hour() >= 8 && now.hour() < 10) || (now.hour() >= 14 && now.hour() < 16)) && color != Mixed){
      onChangeColor(Mixed);
    } else if((now.hour() >= 10 && now.hour() < 14) && color != White){
      onChangeColor(White);
    } else if(now.hour() == 17 && color != Off){
      off();
    }
}
