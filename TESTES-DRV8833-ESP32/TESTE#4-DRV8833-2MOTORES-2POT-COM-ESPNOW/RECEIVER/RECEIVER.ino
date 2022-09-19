#include <esp_now.h>
#include <WiFi.h>

// Estrutura damensagem que será enviada
// DEVE SER A MESMA ESTRUTURA NO EMISSOR
typedef struct struct_message {
  int rightSpd;     // Recebe o valor da velocidade da direita
  int leftSpd;      // Recebe o valor da velocidade da esquerda
  int sentido;      // Recebe o valor dO sentido de rotação dos motores
} struct_message;

struct_message myData;    // Cria um objeto chamado myData

// Função de callback chamada ao receber algum dado
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print("\t");
  Serial.print("VD: ");
  Serial.print(myData.rightSpd);
  Serial.print("\t");
  Serial.print("VE: ");
  Serial.print(myData.leftSpd);
  Serial.print("\t");
  Serial.print("Sentido: ");
  Serial.print(myData.sentido);
  Serial.println();
}

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
  Serial.begin(115200); //Inicia o monitor serial

  // Configura o ESP32 como um Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Configura a função de callback que será chamada ao enviar algum dado
  esp_now_register_recv_cb(OnDataRecv);
  
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
  /*
   * O que irá definir se o motor irá girar para frente ou para trás é determinado pela
   * variável "myData.sentido". 
   * Para cada sentido de rotação, devemos enviar o sinal de PWM para o pino correspondente
   * Isso implica que apenas dois pinos terão sinal PWM e os outros dois estarão em LOW
   *
   */
  if (myData.sentido > 0){
    // Sentido horário
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(chAIN1, myData.rightSpd);	// Escrevemos no canal chAIN1, o duty cycle myData.rightSpd.
    ledcWrite(chBIN1, myData.leftSpd);	// Escrevemos no canal chBIN1, o duty cycle myData.leftSpd.
  }
  else if(myData.sentido < 0){
    // Sentido anti-horário
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, LOW);
    ledcWrite(chAIN2, myData.rightSpd);	// Escrevemos no canal chAIN2, o duty cycle myData.rightSpd.
    ledcWrite(chBIN2, myData.leftSpd);	// Escrevemos no canal chBIN2, o duty cycle myData.leftSpd.
  }
  else{ 
    // Freia os dois motores
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN2, HIGH);
  }
  
}
