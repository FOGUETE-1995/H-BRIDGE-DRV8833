#include <WiFi.h> //Inclui a biblioteca necessária para habilitar o Wifi do ESP32

void setup(){
	
  Serial.begin(115200); // Inicia o monitor Serial
  
  Serial.print("ESP Board MAC Address:  ");
  
  Serial.println(WiFi.macAddress()); //Exibe o endereço MAC do ESP32 no monitor serial
}
 
void loop(){

}
