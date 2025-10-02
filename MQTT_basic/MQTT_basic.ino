#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "Iphone do pavarin";
const String PSWD = "pavarini";

const char* brokerUrl = "test.mosquitto.org";
const int port = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void connectToWifi();
void connectToMqtt();

void setup() {
  Serial.begin(115200);
  connectToWifi();
  connectToMqtt();   // agora é função
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado! Tentando reconectar...");
    connectToWifi();
  }

  if (!mqttClient.connected()) {
    Serial.println("MQTT desconectado! Tentando reconectar...");
    connectToMqtt();
  }

  mqttClient.loop(); // mantém a conexão viva
}

void connectToWifi() {
  Serial.println("Iniciando conexão com rede WiFi...");
  WiFi.begin(SSID.c_str(), PSWD.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado ao WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectToMqtt() {
  Serial.println("Conectando ao broker MQTT...");
  mqttClient.setServer(brokerUrl, port);

  String clientId = "ESP-Pavarin-";
  clientId += String(random(0xffff), HEX);  // gera ID único

  while (!mqttClient.connected()) {
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Conectado ao broker com sucesso!");
    } else {
      Serial.print("Falha na conexão MQTT, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando novamente em 5s...");
      delay(5000);
    }
  }
}

void scanLocalNetworks() {
  Serial.println("Iniciando Scan de Redes Wi-Fi");
  int number = WiFi.scanNetworks();
  delay(500);

  if (number == -1) {
    Serial.println("ERRO! Não foi possível escanear redes.");
  } else {
    Serial.printf("Número de redes encontradas: %d\n", number);
    for (int net = 0; net < number; net++) {
      Serial.printf("%d - %s | %d dBm\n", net, WiFi.SSID(net).c_str(), WiFi.RSSI(net));
    }
  }
}
