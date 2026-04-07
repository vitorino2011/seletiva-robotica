#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Eu estou utilizando um display LCD com comunicação I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Aqui eu defino os pinos do Arduino
const int PINO_SENSOR_PH = A0; // pino do sensor como A0 = pino analógico;
const int LED_VERDE = 7; //led verde como pino digital = 7
const int LED_VERMELHO = 8; //pino vermelho digital = 8

// Esses valores eu uso como referência para o cálculo do pH
const float TENSAO_REFERENCIA = 5.0; 
const int RESOLUCAO_ADC = 1023;

// Aqui eu defino a faixa segura da água
const float LIMITE_INFERIOR = 6.5;
const float LIMITE_SUPERIOR = 8.5;

// ------------------------------------------------------
// Função que eu criei para ler o sensor de pH
// Eu faço várias leituras e tiro uma média para evitar ruído
// ------------------------------------------------------
float lerTensaoPH() {
  long soma = 0;
  const int amostras = 10;

  for (int i = 0; i < amostras; i++) {
    soma += analogRead(PINO_SENSOR_PH);
    delay(20);
  }

  float media = soma / (float)amostras;

  // Aqui eu converto o valor analógico em tensão
  float tensao = media * TENSAO_REFERENCIA / RESOLUCAO_ADC;
  return tensao;
}

// ------------------------------------------------------
// Aqui eu converto a tensão em valor de pH
// Essa fórmula é uma aproximação comum em sensores de pH
// ------------------------------------------------------
float converter_Para_PH(float tensao) {
  float ph = 7.0 + ((2.5 - tensao) / 0.18);

  /*
  Expicação da fórmula:
  A fórmula calcula o pH somando 7 ao quanto a tensão medida se afasta de 2,5 V,
   dividido por 0,18 (fator de conversão do sensor).

   7 = ph neutro;
  */

  return ph;
}

// Eu classifico o pH em três estados

String classificar_PH (float ph) {
  if (ph < LIMITE_INFERIOR) {
    return "ACIDO";
  }
  
  else if (ph > LIMITE_SUPERIOR) {
    return "ALCALINO";
  }
  
  else {
    return "SEGURO";
  }

}


// SETUP - executa uma vez
void setup() {
  Serial.begin(9600);

  // Eu configuro os LEDs como saída
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Eu inicio o LCD
  lcd.init();
  lcd.backlight();

  // Mensagem inicial
  lcd.setCursor(0, 0);
  lcd.print("Sistema de pH");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);

  lcd.clear();
}

// LOOP - executa continuamente
void loop() {

  // Eu faço a leitura do sensor
  float tensao = lerTensaoPH();

  // Eu converto essa leitura para pH
  float ph = converterParaPH(tensao);

  // Eu classifico o resultado
  String status = classificarPH(ph);

  // Aqui eu controlo os LEDs
  if (status == "SEGURO") {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
  } 

  else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }

  // Aqui eu mostro as informações no LCD
  lcd.clear();

  lcd.setCursor(0, 0); // defini a posição das informações no LCD como 0
  lcd.print("pH: "); //mostra as informações do PH de forma adaptavel
  lcd.print(ph, 2);

  lcd.setCursor(0, 1);
  lcd.print(status);

  /*
   Também envio os dados para o monitor serial
   (eu uso isso para depuração)
  */

  Serial.print("Tensao: ");
  Serial.print(tensao);
  Serial.print(" | pH: ");
  Serial.print(ph);
  Serial.print(" | Status: ");
  Serial.println(status);

  delay(2000);
}
