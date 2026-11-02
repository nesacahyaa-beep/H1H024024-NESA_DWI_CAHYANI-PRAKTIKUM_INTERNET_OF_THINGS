#include <ESP8266WiFi.h>

const char* ssid = "vivo";
const char* password = "12345678";

const char* ap_ssid = "ESP8266-PraktikumIoT";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Set mode WiFi menjadi AP + Station
  WiFi.mode(WIFI_AP_STA);

  // Menghubungkan ESP8266 ke WiFi utama
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
  Serial.print("IP Station : ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");
  Serial.print("SSID : ");
  Serial.println(ap_ssid);
  Serial.print("IP Access Point : ");
  Serial.println(apIP);
}

void loop() {
  // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}