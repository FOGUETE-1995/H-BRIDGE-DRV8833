/*
 * Este código tem como objetivo testar o acionamento de dois motores conectados
 * a ponte H DRV8833 com o auxilio de 2 potenciômetros.
 * Cada potenciômetro irá controlar duas variáveis, sendo um encarregado de 
 * controlar para qual lado o robô irá fazer a curva e quão fechada essa curva será.
 * E o segundo potenciômetro irá controlar a velocidade máxima que poderá ser fornecida
 * para os motores e o sentido de rotação deles.
 * 
 */

// Pinos da ponte H que serão conectados ao ESP32
#define AIN1 23
#define AIN2 22
#define BIN1 21
#define BIN2 19

#define potV 39 // Potenciômetro da velocidade
#define potD 32 // Potenciômetro da direção

// Configurando o PWM
const int freq = 50;      // Frequência do PWM, definida em 50Hz
const int resolucao = 12; // Resolução do PWM, 12 bits, de 0 a 4095.

// Estabelecendo os canais PWM que serão utilizados
const int chAIN1 = 0;   // Canal PWM 0 que irá ser vinculado ao pino da ponte H  
const int chAIN2 = 1;   // Canal PWM 1 que irá ser vinculado ao pino da ponte H 
const int chBIN1 = 2;   // Canal PWM 2 que irá ser vinculado ao pino da ponte H 
const int chBIN2 = 3;   // Canal PWM 3 que irá ser vinculado ao pino da ponte H 

// Estabelecendo algumas variáveis de auxílio
int dir = 0; 		// Variável que vai receber o valor da direção
int spd = 0; 		// Variável que vai receber o valor da velocidade
String text = "";	// Variável que vai receber o texto referente ao sentido de direção do robô

int spdRight = 0; 	// Variável que receberá a velocidade do motor da direita
int spdLeft = 0;  	// Variável que receberá a velocidade do motor da esquerda
int spdAbs = 0;   	// Variável que receberá o valor absoluto da velocidade

void setup() {
  Serial.begin(115200); // Inicia o monitor serial

  // Definindo as portas conectadas á ponte H como OUTPUTS
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);

  // Configurando a frequencia e a resolução de cada canal PWM criado
  ledcSetup(chAIN1, freq, resolucao);
  ledcSetup(chAIN2, freq, resolucao);
  ledcSetup(chBIN1, freq, resolucao);
  ledcSetup(chBIN2, freq, resolucao);

  // Atribuindo cada pino de saída do ESP32 ao seu respectivo canal de PWM criado
  ledcAttachPin(AIN1, chAIN1);
  ledcAttachPin(AIN2, chAIN2);
  ledcAttachPin(BIN1, chBIN1);
  ledcAttachPin(BIN2, chBIN2);
}
 
void loop() {
  // Leitura dos potenciômetros
  dir = analogRead(potD);
  spd = analogRead(potV);

  // Conversão dos valores
  dir = map(dir, 0, 4095, -100, 100);
  spd = map(spd, 0, 4095, -100, 100);
  
  /* 
   *  Os valores das variáveis "dir" e "spd" são convertidos para valores entre -100 e 100
   *  para poder saber quão acentuada será a curva, para qual lado será a curva, qual a velocidade 
   *  e qual será o sentido de rotação adotados pelo robô.
   *  
   *  Para saber a direção do robô, utilizamos a variável "dir".
   *  Quanto mais próximo de 100, mais acentuada será a curva do robô para aquele lado.
   *  O lado para o qual o robô fará a curva é determinado pelo sinal (-) ou (+) da variável "dir". 
   *  Assim, se o valor de "dir" é 100, significa que o robô fará uma curva para a direita. E se
   *  o valor de "dir" for -100, o robô fará uma curva para esquerda.
   *  
   *  Para saber o sentido de rotação e a velocidade do robô, utilizamos a variável "spd"
   *  Quanto mais próximo de 100, mais potência será transmitida para os pinos dos motores.
   *  O sentido de rotação e determindo pelo sinal (-) ou (+) da variável "spd".
   *  Ou seja, se o valor de "spd" for 100, o robô terá os motores girando para frente com 100% da potência,
   *  e se o valor de "spd" for -100, o sentido de rotação será o inverso, mas com a mesma potência.
   *
   */


  spdAbs = abs(spd); // Atribuímos o valor de potência que será enviado aos motores

  // Estabelecemos uma margem para centralizar o potenciômetro, isso pode mudar de acordo com o joystick.
  if (dir >= -10 && dir <= 10){
    dir = 0;
  }

  // Estabelecendo os valores de velocidade de cada motor
  if (dir > 0){       
    // Curva para a direita
    spdLeft = spdAbs;
    spdRight = spdAbs - spdAbs*dir/100;
  }
  else if (dir < 0){  
    // Curva para a esquerda
    spdLeft = spdAbs + spdAbs*dir/100;
    spdRight = spdAbs;
  }
  else{
    // Vai para frente ou trás, de acordo com o sentido escolhido
    spdLeft = spdAbs;
    spdRight = spdAbs;
  }

  // Conversão dos valores das velocidades
  spdLeft = map(spdLeft, 0, 100, 0, 4095);
  spdRight = map(spdRight, 0, 100, 0, 4095);


  /*
   * O que irá definir se o motor irá girar para frente ou para trás é determinado pelo
   * sinal (-) ou (+) de "spd". 
   * Para cada sentido de rotação, devemos enviar o sinal de PWM para o pino correspondente
   * Isso implica que apenas dois pinos terão sinal PWM e os outros dois estarão em LOW
   *
   */
   
  if (spd > 0){
    // Sentido horário
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(chAIN1, spdRight);	// Escrevemos no canal chAIN1, o duty cycle spdRight.
    ledcWrite(chBIN1, spdLeft);		// Escrevemos no canal chBIN1, o duty cycle spdLeft.
  }
  else if(spd < 0){
    // Sentido anti-horário
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, LOW);
    ledcWrite(chAIN2, spdRight);	// Escrevemos no canal chAIN2, o duty cycle spdRight.
    ledcWrite(chBIN2, spdLeft);		// Escrevemos no canal chBIN2, o duty cycle spdLeft.
  }
  else{ 
    // Freia os dois motores
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN2, HIGH);
  }
  
}
