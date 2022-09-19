/*
 *
 * Esse código tem como objetivo girar dois motores conectados a ponte H DRV8833
 * em dois sentidos com velocidade máxima, de acordo com a alimentação do circuito.
 * O tempo de duração é de 1 segundo em cada etapa.
 *
 */


// Pinos da ponte H que serão conectados ao ESP32
#define AIN1 23
#define AIN2 22
#define BIN1 21
#define BIN2 19

void setup() {
  Serial.begin(115200); // Iniciando o monitor serial
  
  // Definindo as portas conectadas á ponte H como OUTPUTS
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  
}
 
void loop() {
  
  // Gira os dois motores no sentido horário
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);

  delay(1000); // Espera 1 segundo
  
  // Desliga os dois motores
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);
  
  delay(1000); // Espera 1 segundo
  
  // Gira os dois motores no sentido anti-horário
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);

  delay(1000); // Espera 1 segundo
  
  // Desliga os dois motores
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);
  
  delay(1000); // Espera 1 segundo
   
}
