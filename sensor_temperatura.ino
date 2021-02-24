#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "AAPM";  // Rede WiFi
const char* password = "";  //Senha da Rede WiFi

#define DHTPIN D5 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();

  Serial.println("Conectando a Rede: "); //Imprime na serial a mensagem
  Serial.println(ssid); //Imprime na serial o nome da Rede Wi-Fi
  WiFi.begin(ssid, password); //Inicialização da comunicação Wi-Fi
  
  //Verificação da conexão
  while(WiFi.status() != WL_CONNECTED) { //Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print("."); //Imprime pontos
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP
  }

void loop()
{
  delay(60 * 1000);
  float umi = dht.readHumidity();
  float temp = dht.readTemperature();

  
  String dados = "{\"umidade\": " + String(umi, 2) + ", \"temperatura\": " + String(temp, 2) + ", \"nome_sensor\": \"C\"}";
  
   if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    http.begin("http://senai-metrologia.brazilsouth.cloudapp.azure.com/api/medidas");
    http.addHeader("Content-Type", "application/json");
     
    int codigo_resposta = http.POST(dados);
     
    if(codigo_resposta>0){
      
      Serial.println("Código HTTP : " + String(codigo_resposta));
      Serial.println("Dados: " + dados);

      if(codigo_resposta == 201){
        String resposta = http.getString();
        Serial.println("O servidor respondeu : ");
        Serial.println(resposta);

      }

    }else{
     Serial.print("Erro enviando POST, código: ");
     Serial.println(codigo_resposta);
    }

    http.end();

  }else{
     Serial.println("Erro na conexão WIFI");

  }
}
