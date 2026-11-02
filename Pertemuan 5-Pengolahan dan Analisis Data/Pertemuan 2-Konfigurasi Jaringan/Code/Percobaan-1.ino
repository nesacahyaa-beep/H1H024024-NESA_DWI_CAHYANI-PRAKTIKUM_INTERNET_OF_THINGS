#include <ESP8266WiFi.h>

const char* ssid = "myminetae";
const char* password = "12345678";

const int ledPin = 2; // LED indikator status koneksi

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Set mode WiFi menjadi Station
  WiFi.mode(WIFI_STA);

  // Memulai koneksi ke jaringan WiFi
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  digitalWrite(ledPin, HIGH); // nyalakan LED sebagai indikator
}

void loop() {

  // Mengecek apakah koneksi WiFi terputus
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi terputus!");
    Serial.println("Mencoba menghubungkan kembali...");

    digitalWrite(ledPin, LOW);

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi berhasil terhubung kembali!");

    digitalWrite(ledPin, HIGH);
  }

  // Menampilkan informasi koneksi setiap 5 detik
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
  } else {
    Serial.println("Status: Terputus");
  }

  delay(5000);
}