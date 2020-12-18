#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <DHTesp.h>

const char* ssid = "AAPM";  // Rede WiFi
const char* password = "";  //Senha da Rede WiFi

#define DHTpin 14    //D5 of NodeMCU is GPIO14

DHTesp dht;

void setup()
{
  Serial.begin(115200);

  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17

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
  delay(4 * 1000 * 1);
  float umi = dht.getHumidity();
  float temp = dht.getTemperature();

  
  String dados = "{\"umidade\": " + String(umi, 1) + ", \"temperatura\": " + String(temp, 1) + ", \"nome_sensor\": \"A\"}";
  
   if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    http.begin("http://senai-metrologia.brazilsouth.cloudapp.azure.com/api");
    http.addHeader("Content-Type", "application/json");
     
     int codigo_resposta = http.POST(dados);
     
    if(codigo_resposta>0){
      
      Serial.println("Código HTTP : " + String(codigo_resposta));

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
  Serial.println(dados);
}
