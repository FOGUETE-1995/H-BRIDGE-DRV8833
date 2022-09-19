#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //COLOQUE os valores do endereço MAC do receptor

// Estrutura da mensagem que será enviada
// DEVE SER A MESMA ESTRUTURA NO RECEPTOR
typedef struct struct_message {
  int spdRight;    // Variável que receberá a velocidade do motor da direita
  int spdLeft;     // Variável que receberá a velocidade do motor da esquerda
  int sentido;     // Variável que receberá o sentido de rotação
} struct_message;

struct_message mySpd;      // Cria um objeto chamado mySpd

esp_now_peer_info_t peerInfo;  // Cria um objeto chamado peerInfo

// Função callback chamada ao enviar algum dado
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

#define potV 39 // Potenciômetro da velocidade
#define potD 32 // Potenciômetro da direção

// Estabelecendo algumas variáveis de auxílio
int dir = 0;      // Variável que vai receber o valor da direção
int spd = 0;      // Variável que vai receber o valor da velocidade
int spdAbs = 0;   // Variável que receberá o valor absoluto da velocidade

void setup() {
  Serial.begin(115200); // Inicia o monitor serial

  // Configura o ESP32 como um Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  // Configura a função de callback que será chamada ao enviar algum dado
  esp_now_register_send_cb(OnDataSent);
  
  // Registra o dispositivo que receberá os dados (peer)
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Adiciona o dispositivo que receberá os dados (peer)  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
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
   */

  spdAbs = abs(spd); // Atribuímos o valor de potência que será enviado aos motores
  
  /*
   * O que irá definir se o motor irá girar para frente ou para trás é determinado pelo
   * sinal (-) ou (+) de "spd". 
   *
   */
  mySpd.sentido = spd;

  // Estabelecemos uma margem para centralizar o potenciômetro, isso pode mudar de acordo com o joystick.
  if (dir >= -10 && dir <= 10){
    dir = 0;
  }

  // Estabelecendo os valores de velocidade de cada motor
  if (dir > 0){
      // Curva para a direita
      mySpd.spdLeft = spdAbs;
      mySpd.spdRight = spdAbs - spdAbs*dir/100;
  }else if(dir < 0){
      // Curva para a esquerda
      mySpd.spdLeft = spdAbs + spdAbs*dir/100;
      mySpd.spdRight = spdAbs;
  }else{
      // Vai para frente ou para trás de acordo com o sentido escolhido
      mySpd.spdLeft = spdAbs;
      mySpd.spdRight = spdAbs;
  }

  // Conversão dos valores das velocidades
  mySpd.spdLeft = map(mySpd.spdLeft, 0, 100, 0, 4095);
  mySpd.spdRight = map(mySpd.spdRight, 0, 100, 0, 4095);

  // Envia os dados via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &mySpd, sizeof(mySpd));
   
  // Exibe uma mensagem na tela se a mensagem foi enviada ou não.
  if (result == ESP_OK) {
    Serial.println("Success"); 	// Mensagem enviada
  }
  else {
    Serial.println("Error");	// Mensagem não enviada
  }
}
