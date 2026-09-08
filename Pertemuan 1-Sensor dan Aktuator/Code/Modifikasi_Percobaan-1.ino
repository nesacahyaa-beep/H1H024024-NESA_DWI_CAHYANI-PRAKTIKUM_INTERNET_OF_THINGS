#include <DHT.h>

#define DHTPIN D4      
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Memulai akuisisi data sensor DHT11...");
}

void loop() {
  float totalSuhu = 0.0;
  float totalKelembaban = 0.0;
  int sampelValid = 0;
  int jumlahSampel = 5; // Jumlah pengambilan data untuk dirata-ratakan

  for (int i = 0; i < jumlahSampel; i++) {
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();

    // Memeriksa apakah hasil pembacaan sensor valid
    if (isnan(kelembaban) || isnan(suhu)) {
      Serial.println("Gagal membaca data dari sensor DHT11!");
    } else {
      totalSuhu += suhu;
      totalKelembaban += kelembaban;
      sampelValid++;
    }

    delay(2000);  // Jeda 2 detik antar pembacaan sampel
  }

  if (sampelValid > 0) {
    float rataSuhu = totalSuhu / sampelValid;
    float rataKelembaban = totalKelembaban / sampelValid;

    Serial.print("Rata-rata Suhu (5 sampel): ");
    Serial.print(rataSuhu);
    Serial.print(" °C, Rata-rata Kelembaban: ");
    Serial.print(rataKelembaban);
    Serial.println(" %");
  } else {
    Serial.println("Pengambilan 5 sampel gagal sepenuhnya!");
  }
}