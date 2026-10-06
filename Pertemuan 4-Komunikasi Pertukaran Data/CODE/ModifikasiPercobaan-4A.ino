#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topicData = "unsoed/tk245004/kelompokAnda/data";
const char* topicPerintah = "unsoed/tk245004/kelompokAnda/perintah";
const char* topicBuzzer = "unsoed/tk245004/kelompokAnda/buzzer";

#define DHTPIN 4
#define DHTTYPE DHT11
const int ledPin = 2;
const int buzzerPin = 5;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000; // publish data setiap 5 detik (non-blocking)

void callback(char* topic, byte* payload, unsigned int length) {
  String pesan;
  for (unsigned int i = 0; i < length; i++) pesan += (char)payload[i];

  JsonDocument doc;
  if (deserializeJson(doc, pesan)) return; // abaikan jika parsing gagal

  const char* perintah = doc["perintah"];

  if (String(topic) == topicPerintah) {
    digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW);
    Serial.print("Perintah diterima -> Aktuator LED: ");
    Serial.println(perintah);
  } 
  else if (String(topic) == topicBuzzer) {
    digitalWrite(buzzerPin, String(perintah) == "ON" ? HIGH : LOW);
    Serial.print("Perintah diterima -> Aktuator Buzzer: ");
    Serial.println(perintah);
  }
}

void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi berhasil terhubung!");
}

void hubungkanMQTT() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      client.subscribe(topicPerintah);
      client.subscribe(topicBuzzer);
      Serial.println("Terhubung dan subscribe topic perintah");
      Serial.println("Subscribe topic buzzer");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  dht.begin();
  hubungkanWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) hubungkanMQTT();

  client.loop(); // memproses pesan masuk secara terus-menerus

  // Publish data sensor secara berkala tanpa memblokir proses subscribe
  if (millis() - waktuTerakhirPublish > intervalPublish) {
    waktuTerakhirPublish = millis();

    float suhu = dht.readTemperature();

    if (!isnan(suhu)) {
      JsonDocument doc;
      doc["suhu"] = suhu;

      char buffer[128];
      serializeJson(doc, buffer);

      client.publish(topicData, buffer);

      Serial.print("Data terkirim: ");
      Serial.println(buffer);
    }
  }
}