## Percobaan 4B: Pertukaran Data Dua Arah (Publish dan Subscribe Secara Bersamaan) 
Dokumentasi ini memuat detail pelaksanaan Percobaan 4B mengenai komunikasi data dua arah (*full-duplex*) secara *real-time* antara mikrokontroler ESP8266 NodeMCU dengan *broker* MQTT (`broker.hivemq.com`) menggunakan protokol MQTT, teknik *non-blocking timer*, dan enkapsulasi *payload* JSON.

## Tujuan
1. Mempelajari konsep komunikasi dua arah (full-duplex) pada mikrokontroler dengan memanfaatkan protokol MQTT, yaitu mekanisme publish dan subscribe.
2. Menerapkan pustaka PubSubClient dan ArduinoJson untuk mengirim data telemetri (publish) sekaligus menerima perintah kontrol (subscribe).
3. Menggunakan teknik non-blocking timer berbasis millis() supaya proses penerimaan pesan dapat berjalan secara real-time dan tidak tertunda oleh delay().
4. Mengkaji cara kerja penyambungan ulang otomatis (auto-reconnect) serta pengolahan payload berformat JSON dalam sistem IoT.

## Spesifikasi yang diharapkan
1. ESP8266 NodeMCU sukses tersambung ke jaringan WiFi dan tercatat sebagai klien yang terhubung (connected) pada broker MQTT broker.hivemq.com.
2. Mikrokontroler mengirimkan (publish) data telemetri atau status perangkat dalam format JSON ke broker secara periodik.
3. Mikrokontroler mampu menerima (subscribe) perintah kontrol {"perintah": "ON"} atau {"perintah": "OFF"} dari broker secara real-time untuk menyalakan dan mematikan LED pada pin D4.
4. Sistem bekerja secara full-duplex tanpa keterlambatan (latency) maupun terputusnya koneksi yang disebabkan oleh penggunaan fungsi blocking.

## Alat dan Bahan
1. Board ESP8266 DevKit (1 buah)
2. LED (1 buah) sebagai simulasi aktuator, dan Resistor 220 Ohm (1 buah)
3. Sensor DHT22
4.Breadboard dan Kabel Jumper (secukupnya)
5. Kabel USB (Micro-USB/USB-C sesuai board)
6. Laptop/PC dengan Arduino IDE (sudah terpasang board manager ESP8266, pustaka DHT sensor library, PubSubClient, dan ArduinoJson)
7. Jaringan WiFi yang terhubung ke internet
8. Aplikasi client MQTT (MQTT Explorer/HiveMQ WebSocket Client) untuk mengirim perintah kendali dan memantau data
9. Broker MQTT publik (port 1883)

## Rangkaian Percobaan

Pada percobaan ini, kaki anoda LED terhubung ke pin **GPIO2 (D4)** pada ESP8266 NodeMCU melalui resistor pembatas arus 220Ω, dan kaki katoda terhubung ke pin **GND**. ESP8266 dikonfigurasi untuk terhubung ke jaringan WiFi eksternal dan bertindak sebagai klien MQTT *full-duplex* yang mempublikasikan data telemetry sekaligus mendengarkan perintah kontrol.

## Gambar Percobaan 4B
Kondisi Saat Menerima {"perintah": "OFF"}

![Percobaan 4A Bagian 1](<percobaan 4B-1.jpeg>)

Kondisi Saat Menerima {"perintah": "ON"}

![Percobaan 4A Bagian 2](<percobaan 4B-2.jpeg>)

## Source dan Penjelasan Program
Library/Dependencies:
- `<ESP8266WiFi.h>`: Library utama untuk mengelola antarmuka nirkabel pada ESP8266 agar terhubung ke titik akses (*access point*).
- `<PubSubClient.h>`: Library untuk mengimplementasikan protokol MQTT (*Publish* dan *Subscribe*) pada mikrokontroler.
- `<ArduinoJson.h>`: Library untuk melakukan pembuatan (*serialization*) dan penguraian (*deserialization*) data berformat JSON.

## Penjelasan Logika Program
Alur Kerja Program
1. **Inisialisasi (`setup`)**: Program mengaktifkan Serial Monitor (115200 bps), mengatur pin D4 sebagai `OUTPUT` dengan kondisi awal `LOW`, mengisikan kredensial WiFi, serta mengatur konfigurasi server broker MQTT dan fungsi *callback*.
2. **Pendaftaran & Re-koneksi MQTT**: Fungsi `reconnect()` dipanggil untuk memastikan ESP8266 terhubung ke broker MQTT dan mendaftarkan diri (*subscribe*) ke *topic* kontrol target.
3. **Monitoring & Callback Subscriber**: Saat pesan masuk dari broker pada *topic* kontrol, fungsi `callback()` mendeserialisasi *payload* JSON dan langsung mengubah status LED (ON/OFF/BLINK).
4. **Non-Blocking Telemetry Loop (`loop`)**: Di dalam `loop()`, fungsi `client.loop()` dijalankan terus-menerus. Setiap selisih waktu `millis()` memenuhi interval tertentu, program melakukan serialisasi status perangkat ke format JSON lalu mempublikasikannya (*publish*) ke *topic* telemetry tanpa membekukan alur utama program.

## Penjelasan Fungsi Utama
1. `client.setServer(broker, port)`: Menentukan alamat host broker MQTT dan nomor port komunikasi (default: 1883).
2. `client.setCallback(callback)`: Mengatur fungsi *callback* yang dipanggil otomatis saat ada pesan masuk pada *topic subscribe*.
3. `client.subscribe(topic)`: Mendaftarkan mikrokontroler sebagai *subscriber* pada *topic* tertentu untuk menerima pesan kontrol.
4. `client.publish(topic, payload)`: Mengirimkan *payload* data (teks/JSON) ke *topic* tertentu pada broker MQTT.
5. `client.loop()`: Menjaga sinyal *keep-alive*, memproses *buffer* data masuk, dan mengelola pertukaran pesan MQTT.
6. `deserializeJson(doc, payload)`: Mengurai *payload* string JSON mentah dari broker menjadi objek C++.
7. `serializeJson(doc, buffer)`: Mengonversi objek JSON di memori menjadi format string/buffer untuk dipublikasikan.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)
1. `if (!client.connected())`: Percabangan untuk mendeteksi apabila koneksi ke broker MQTT terputus, sehingga program memicu fungsi `reconnect()`.
2. `if (millis() - lastMsg > interval)`: Percabangan pewaktuan *non-blocking* yang mengeksekusi pengiriman data telemetry secara berkala tanpa menghentikan pemrosesan rutin `client.loop()`.
3. `if (error)`: Percabangan pada fungsi *callback* untuk mendeteksi apakah *payload* JSON yang diterima dari broker valid atau mengalami kegagalan ekstraksi.

## Konfigurasi Pin
| Komponen       | Pin NodeMCU | Pin GPIO | Modus  | Keterangan                                 |
| :------------- | :---------: | :------: | :----: | :----------------------------------------- |
| Anoda LED (+)  |   Pin D4    |  GPIO2   | OUTPUT | Terhubung ke kaki anoda LED (sisi +)       |
| Katoda LED (-) |   Pin GND   |    -     |   -    | Terhubung ke Ground NodeMCU                |
| Resistor 220Ω  |  Antara D4  |    -     |   -    | Pembatas arus listrik untuk LED (opsional) |
| Built-in LED   |   Pin D4    |  GPIO2   | OUTPUT | LED internal bawaan modul ESP8266          |

## Source Code
**Bagian 1**

![Percobaan4A](<Sourcecode_Percobaan-4B-1.png>)

**Bagian 2**

![Percobaan4A](<Sourcecode_Percobaan-4B-2.png>)

## Pertanyaan Praktikum
1. Mengapa penggunaan delay() yang lama sebaiknya dihindari pada program yang menggabungkan proses publish dan subscribe secara bersamaan? \
**Jawab :**
Fungsi delay() bersifat blocking karena menghentikan seluruh proses eksekusi
mikrokontroler. Penggunaannya menyebabkan client.loop() terhenti, sehingga perangkat
tidak dapat menerima perintah kendali (subscribe) secara langsung dan berisiko terputus
dari broker MQTT akibat timeout.

2. Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi millis() pada program diatas! \
**Jawab:** 
Penggunaan millis() memanfaatkan pencatat waktu internal board. Program membandingkan selisih waktu saat ini dengan waktuTerakhirPublish (millis() -waktuTerakhirPublish > 5000), sehingga data sensor hanya dikirim saat interval 5 detik tercapai. Di luar interval tersebut, loop() tetap berjalan sehingga client.loop() dapat dieksekusi tanpa terhambat.

3. Apa yang akan terjadi apabila fungsi client.loop() jarang dipanggil (misalnya hanya sekali setiap 10 detik)? \
**Jawab:**
Jika delay() (misalnya 10 detik) digunakan pada loop(), respons aktuator terhadap perintah mengalami keterlambatan hingga 10 detik. Selain itu, buffer pesan MQTT berisiko overflow, dan koneksi ke broker dapat terputus karena perangkat gagal membalas sinyal keep-alive (ping) tepat waktu.

4. Modifikasi program agar menambahkan satu topic perintah baru untuk mengendalikan aktuator kedua (misalnya buzzer), dengan fungsi callback yang dapat membedakan topic mana yang menerima pesan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md! \
**Jawab:**
```cpp
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