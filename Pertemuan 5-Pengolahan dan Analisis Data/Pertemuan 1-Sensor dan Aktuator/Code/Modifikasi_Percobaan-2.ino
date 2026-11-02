#include <DHT.h>

#define DHTPIN 4       // Pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22
#define RELAYPIN 26    // Pin kendali relay/LED indikator

DHT dht(DHTPIN, DHTTYPE);

// Menggunakan dua ambang batas (histerisis)
const float suhuBatasAtas = 30.0;  // Ambang batas atas (°C)
const float suhuBatasBawah = 28.0; // Ambang batas bawah (°C)

// Variabel untuk menyimpan status aktuator (false = OFF, true = ON)
bool statusAktuator = false;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); 
}

void loop() {
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Logika kontrol kendali histerisis
    if (suhu > suhuBatasAtas) {
      statusAktuator = true;
      digitalWrite(RELAYPIN, HIGH); // Aktifkan relay/LED
    } else if (suhu < suhuBatasBawah) {
      statusAktuator = false;
      digitalWrite(RELAYPIN, LOW);  // Matikan relay/LED
    }
    // Jika suhu berada di antara 28.0 °C dan 30.0 °C, 
    // kondisi relay tidak diubah (mempertahankan kondisi sebelumnya).

    if (statusAktuator) {
      Serial.println("Aktuator: ON");
    } else {
      Serial.println("Aktuator: OFF");
    }
  }

  delay(2000); // Jeda antar pembacaan data
}
