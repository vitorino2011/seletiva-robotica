#include <Wire.h> // Aqui fiz a inclusão da biblioteca de comunicação I2C.
#include <LiquidCrystal_I2C.h> // Aqui fiz a inclusão da biblioteca do display LCD com interface I2C.

// Aqui fiz a configuração do display LCD no endereço 0x27, com 16 colunas e 2 linhas.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Aqui fiz a definição dos pinos utilizados no Arduino.
const int PINO_SENSOR_PH = A0; // Aqui fiz a definição do pino analógico do sensor de pH.
const int LED_VERDE = 7; // Aqui fiz a definição do pino digital do LED verde.
const int LED_VERMELHO = 8; // Aqui fiz a definição do pino digital do LED vermelho.

// Aqui fiz a definição dos parâmetros usados no cálculo da tensão.
const float TENSAO_REFERENCIA = 5.0; // Aqui considerei a tensão de referência do Arduino.
const int RESOLUCAO_ADC = 1023; // Aqui considerei a resolução do conversor analógico-digital de 10 bits.

// Aqui fiz a definição da faixa segura do pH.
const float LIMITE_INFERIOR = 6.5; // Aqui fixei o limite inferior da faixa segura.
const float LIMITE_SUPERIOR = 8.5; // Aqui fixei o limite superior da faixa segura.

float lerTensaoPH() { // Aqui fiz a criação da função que lê a tensão média do sensor.
  long soma = 0; // Aqui fiz a variável que acumula as leituras analógicas.
  const int amostras = 10; // Aqui defini a quantidade de amostras para calcular a média.

  for (int i = 0; i < amostras; i++) { // Aqui fiz um laço para repetir a leitura do sensor.
    soma += analogRead(PINO_SENSOR_PH); // Aqui fiz a leitura analógica e somei o resultado.
    delay(20); // Aqui fiz uma pequena pausa entre as leituras para estabilização.
  }

  float media = soma / (float)amostras; // Aqui fiz o cálculo da média das leituras.
  float tensao = media * TENSAO_REFERENCIA / RESOLUCAO_ADC; // Aqui fiz a conversão do valor analógico em tensão.
  return tensao; // Aqui fiz o retorno da tensão calculada.
}

float converterParaPH(float tensao) { // Aqui fiz a criação da função que converte tensão em pH.
  float ph = 7.0 + ((2.5 - tensao) / 0.18); // Aqui fiz o cálculo do pH com base na calibração adotada.

  /*
    Aqui fiz a explicação da fórmula:
    parti do pH neutro, que é 7,0, e somei ou subtraí a variação
    causada pela diferença entre a tensão medida e 2,5 V.
    Depois, dividi essa diferença por 0,18, que foi o fator de conversão do sensor.
  */

  return ph; // Aqui fiz o retorno do valor de pH calculado.
}

String classificarPH(float ph) { // Aqui fiz a criação da função que classifica o pH.
  if (ph < LIMITE_INFERIOR) { // Aqui verifiquei se o valor estava abaixo da faixa segura.
    return "ACIDO"; // Aqui retornei a classificação ácida.
  } else if (ph > LIMITE_SUPERIOR) { // Aqui verifiquei se o valor estava acima da faixa segura.
    return "ALCALINO"; // Aqui retornei a classificação alcalina.
  } else { // Aqui tratei a condição intermediária.
    return "SEGURO"; // Aqui retornei a classificação segura.
  }
}

void setup() { // Aqui fiz a função de inicialização, executada uma única vez.
  Serial.begin(9600); // Aqui fiz o início da comunicação serial.

  pinMode(LED_VERDE, OUTPUT); // Aqui configurei o LED verde como saída.
  pinMode(LED_VERMELHO, OUTPUT); // Aqui configurei o LED vermelho como saída.

  lcd.init(); // Aqui fiz a inicialização do display LCD.
  lcd.backlight(); // Aqui fiz o acionamento da luz de fundo do display.

  lcd.setCursor(0, 0); // Aqui posicionei o cursor na primeira linha.
  lcd.print("Sistema de pH"); // Aqui mostrei a mensagem inicial.
  lcd.setCursor(0, 1); // Aqui posicionei o cursor na segunda linha.
  lcd.print("Iniciando..."); // Aqui mostrei a mensagem de inicialização.
  delay(2000); // Aqui mantive a mensagem por dois segundos.

  lcd.clear(); // Aqui limpei o display após a mensagem inicial.
}

void loop() { // Aqui fiz a função principal, executada continuamente.
  float tensao = lerTensaoPH(); // Aqui fiz a leitura da tensão do sensor.
  float ph = converterParaPH(tensao); // Aqui fiz a conversão da tensão para pH.
  String status = classificarPH(ph); // Aqui fiz a classificação do valor encontrado.

  if (status == "SEGURO") { // Aqui verifiquei se a água estava na faixa segura.
    digitalWrite(LED_VERDE, HIGH); // Aqui liguei o LED verde.
    digitalWrite(LED_VERMELHO, LOW); // Aqui desliguei o LED vermelho.
  } else { // Aqui tratei os casos fora da faixa segura.
    digitalWrite(LED_VERDE, LOW); // Aqui desliguei o LED verde.
    digitalWrite(LED_VERMELHO, HIGH); // Aqui liguei o LED vermelho.
  }

  lcd.clear(); // Aqui limpei o display antes de atualizar as informações.
  lcd.setCursor(0, 0); // Aqui posicionei o cursor na primeira linha.
  lcd.print("pH: "); // Aqui mostrei o rótulo do valor de pH.
  lcd.print(ph, 2); // Aqui mostrei o valor do pH com duas casas decimais.

  lcd.setCursor(0, 1); // Aqui posicionei o cursor na segunda linha.
  lcd.print(status); // Aqui mostrei a classificação da água.

  Serial.print("Tensao: "); // Aqui iniciei a exibição da tensão no monitor serial.
  Serial.print(tensao); // Aqui mostrei o valor da tensão.
  Serial.print(" | pH: "); // Aqui mostrei o separador e o rótulo do pH.
  Serial.print(ph); // Aqui mostrei o valor do pH.
  Serial.print(" | Status: "); // Aqui mostrei o rótulo da classificação.
  Serial.println(status); // Aqui mostrei a classificação final e quebrei a linha.

  delay(1000); // Aqui esperei um segundo antes da próxima leitura.
}
