#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

String message = "";
StaticJsonDocument<512> doc; 

#define PIN 8
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  if (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');
    DeserializationError erro = deserializeJson(doc, message);

    if (!erro) {
      int tempo = doc["tempo"];
      Serial.print("Tempo: ");
      Serial.println(tempo);

      JsonArray cores = doc["cores"].as<JsonArray>();
      for (JsonObject cor : cores) {
        byte r = cor["r"];
        byte g = cor["g"];
        byte b = cor["b"];

        setColor(r, g, b);
        pixels.show();
        delay(tempo); 
      }
    } else {
      Serial.println("Erro no JSON recebido!");
    }
  }
}

void setColor(byte red, byte green, byte blue) {
  pixels.setPixelColor(0, pixels.Color(red, green, blue));
}
