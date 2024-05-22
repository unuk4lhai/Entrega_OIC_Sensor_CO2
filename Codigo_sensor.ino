#include <ESP8266WiFi.h>
#include <EspMQTTClient.h>
#include "MQ135.h"

//Definicoes e constantes
char SSIDName[] = "SUELI_61 - 2.4G"; //nome da rede WiFi
char SSIDPass[] = "@c3ssando3"; //senha da rede WiFI

char BrokerURL[] = "broker.hivemq.com"; //URL do broker MQTT
char BrokerUserName[] = ""; //nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; //senha para autenticar no broker MQTT
char MQTTClientName[] = "mqtt-mack-pub"; //nome do cliente MQTT
int BrokerPort = 1883; //porta do broker MQTT

String TopicoPrefixo = "TESTMACK1165744"; //prefixo do topico
String Topico_01 = TopicoPrefixo+"/CO2"; //nome do topico 01


EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); //inicializa o cliente MQTT

MQ135 gasSensor = MQ135(A0, 218);

//Este prototipo de funcao deve estar sempre presente
void onConnectionEstablished() {
}


void setup() {

  Serial.begin(9600);
  clienteMQTT.enableDebuggingMessages(); //habilita mensagens de debug no monitor serial

}

void loop() {

  //clienteMQTT.enableMQTTPersistence(); //estabelece uma conexao persistente
  clienteMQTT.loop(); //funcao necessaria para manter a conexao com o broker MQTT ativa e coletar as mensagens recebidas

  if (clienteMQTT.isWifiConnected() == 1) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected() == 1) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

  float avg = 0;
  for (int i = 0; i <= 100; i++) {
    float air_quality = gasSensor.getPPM();
    avg += air_quality;
    }
  avg = avg/100;
  Serial.println(avg);

  clienteMQTT.publish(Topico_01, String(avg, 2)); //funcao para publicar os dados no broker MQTT



  delay(1000);

}
