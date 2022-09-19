/*
 * Este código tem como objetivo testar o aumento e a diminuição da velocidade de dois motores
 * conectados a ponte H DRV8833 com o uso da modulação PWM e com o auxilio de um loop de repetição.
 * Ambos os motores são acionados juntos, porém cada pino possui um canal PWM que é definido pelo usuário.
 *
 */
 
// Pinos da ponte H que serão conectados ao ESP32
#define AIN1 23
#define AIN2 22
#define BIN1 21
#define BIN2 19

// Configurando o PWM
const int freq = 50;      // Frequência do PWM, definida em 50Hz
const int resolucao = 12; // Resolução do PWM, 12 bits, de 0 a 4095.

// Estabelecendo os canais PWM que serão utilizados
const int chAIN1 = 0;   // Canal PWM 0 que irá ser vinculado ao pino da ponte H  
const int chAIN2 = 1;   // Canal PWM 1 que irá ser vinculado ao pino da ponte H 
const int chBIN1 = 2;   // Canal PWM 2 que irá ser vinculado ao pino da ponte H 
const int chBIN2 = 3;   // Canal PWM 3 que irá ser vinculado ao pino da ponte H 

void setup() {
  Serial.begin(115200);	// Iniciando o monitor serial
  
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
	
  // Acelera os dois motores no sentido horário, com a velocidade aumentando a cada 1ms
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN2,LOW);
  
  for (int i = 0; i < 4096; i++)
  {
    ledcWrite(chAIN1, i); // Escrevemos no canal chAIN1, o duty cycle "i".
    ledcWrite(chBIN1, i); // Escrevemos no canal chBIN1, o duty cycle "i".
    delay(1);
  }
  
  // Desacelera os dois motores, com a velocidade diminuindo a cada 1ms
  for (int i = 4095; i > 0; i--)
  {
    ledcWrite(chAIN1, i); // Escrevemos no canal chAIN1, o duty cycle "i".
    ledcWrite(chBIN1, i); // Escrevemos no canal chBIN1, o duty cycle "i".
    delay(1);
  }

  // Acelera os dois motores no sentido anti-horário, com a velocidade aumentando a cada 1ms
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, LOW);

  for (int i = 0; i < 4096; i++)
  {
    ledcWrite(chAIN2, i); // Escrevemos no canal chAIN2, o duty cycle "i".
    ledcWrite(chBIN2, i); // Escrevemos no canal chBIN2, o duty cycle "i".
    delay(1);
  }
  
  // Desacelera os dois motores, com a velocidade diminuindo a cada 1ms
  for (int i = 4095; i > 0; i--)
  {
    ledcWrite(chAIN2, i); // Escrevemos no canal chAIN2, o duty cycle "i".
    ledcWrite(chBIN2, i); // Escrevemos no canal chBIN2, o duty cycle "i".
    delay(1);
  } 
}
