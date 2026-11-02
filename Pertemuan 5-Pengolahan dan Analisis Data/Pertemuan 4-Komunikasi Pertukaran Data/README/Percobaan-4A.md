## Percobaan 4A: Subscribe dan Deserialisasi Data JSON untuk Kendali Aktuator 
Dokumentasi ini memuat detail pelaksanaan Percobaan 4A mengenai komunikasi data dua arah antara mikrokontroler ESP8266 NodeMCU dengan broker MQTT menggunakan fungsi callback (Subscribe) dan deserialisasi format data JSON untuk mengendalikan aktuator (LED).

## Tujuan
1. Memahami konsep komunikasi data berbasis protokol MQTT dengan pola Publish/Subscribe pada mikrokontroler.
2. Mengonfigurasi pustaka `PubSubClient` dan fungsi `callback()` untuk menerima pesan terenkapsulasi secara real-time.
3. Menyusun dan melakukan deserialisasi (parsing) payload berformat JSON (JavaScript Object Notation) menggunakan pustaka ArduinoJson.
4. Mengendalikan status aktuator (LED) berdasarkan nilai variabel yang diekstrak dari data JSON serta melakukan penanganan kesalahan (error handling).

## Spesifikasi yang diharapkan
1. ESP8266 NodeMCU berhasil terhubung ke jaringan WiFi eksternal dan terhubung aktif ke broker MQTT `(broker.hivemq.com)`.
2. Mikrokontroler berhasil melakukan subscribe ke topic target dan menerima pesan payload berformat JSON dari MQTT Client (seperti MQTT Explorer).
3. Indikator LED pada pin D4 (GPIO2) menyala (HIGH) saat menerima perintah JSON "ON" dan mati (LOW) saat menerima perintah "OFF".
4. Program secara otomatis mampu mendeteksi kesalahan parsing JSON (deserialization error) dan mencetak penanganan pesan kesalahan ke Serial Monitor tanpa membuat sistem mengalami crash.

## Alat dan Bahan
1. Board ESP8266 DevKit (1 buah)
2. LED (1 buah) sebagai simulasi aktuator, dan Resistor 220 Ohm (1 buah)
3. Breadboard dan Kabel Jumper (secukupnya)
4. Kabel USB (Micro-USB/USB-C sesuai board)
5. Laptop/PC dengan Arduino IDE (sudah terpasang board manager ESP8266, pustaka DHT sensor library, PubSubClient, dan ArduinoJson)
6. Jaringan WiFi yang terhubung ke internet
7. Aplikasi client MQTT (MQTT Explorer/HiveMQ WebSocket Client) untuk mengirim perintah kendali dan memantau data
8. Broker MQTT publik (port 1883)

## Rangkaian Percobaan
Pada percobaan ini, LED terhubung ke pin GPIO2 (D4) pada ESP8266 NodeMCU. ESP8266 dikonfigurasi sebagai MQTT Client (Subscriber) yang terhubung ke jaringan WiFi dan melakukan koneksi berkelanjutan ke broker MQTT. Ketika pesan baru dipublikasikan pada topic yang diikuti, broker meneruskan pesan tersebut ke ESP8266 untuk memicu fungsi callback.

## Gambar Rangkaian Percobaan 4A
Kondisi Saat Menerima {"perintah": "OFF"}
![Percobaan 4A Bagian 1](<percobaan 4A-1.jpg>)
Kondisi Saat Menerima {"perintah": "ON"}
![Percobaan 4A Bagian 2](<percobaan 4A-2.jpg>)

## Source Code dan Penjelasan Program
Library/Dependencies:
- <ESP8266WiFi.h>: Library utama untuk mengelola koneksi nirkabel WiFi pada ESP8266 agar terhubung ke jaringan lokal/internet.
- <PubSubClient.h>: Library penanganan protokol MQTT yang menyediakan fungsi koneksi ke broker, publikasi pesan, pemesanan topik (subscribe), serta pendeteksian pesan masuk melalui fungsi callback.
- <ArduinoJson.h>: Library untuk mengalokasikan memori dokumen JSON (JsonDocument) serta melakukan deserialisasi string/buffer JSON menjadi objek data yang dapat diakses oleh logika program.

## Penjelasan Logika Program
**Alur Kerja Program**
1. **Inisialisasi (`setup`)**: Saat program dijalankan, ESP8266 mengaktifkan komunikasi Serial dengan kecepatan 115200 bps dan mengatur pin D4 sebagai `OUTPUT` untuk LED. Setelah itu, perangkat tersambung ke jaringan WiFi, menetapkan alamat broker MQTT melalui `client.setServer()`, dan mendaftarkan fungsi callback dengan `client.setCallback()`.
2. **Pemeliharaan Koneksi (`loop`)**: Di dalam `loop()`, program secara berkelanjutan menjalankan `client.loop()` agar komunikasi dengan broker tetap berjalan. Bila koneksi MQTT terputus, fungsi `reconnect()` dipanggil untuk menyambung ulang ke broker sekaligus melakukan subscribe kembali ke topic kontrol.
3. **Penerimaan Pesan (`callback`)**: Setiap kali broker mengirimkan pesan pada topic yang di-subscribe, fungsi `callback(topic, payload, length)` akan berjalan secara otomatis. Data mentah (payload) yang diterima kemudian diubah menjadi string atau langsung diproses oleh pustaka `ArduinoJson`.
4. **Deserialisasi JSON dan Eksekusi Perintah**: Fungsi `deserializeJson(doc, payload)` digunakan untuk mengurai data JSON. Jika proses berhasil, program membaca nilai dari kunci `"perintah"`. Bila nilainya `"ON"`, LED dinyalakan (`HIGH`), sedangkan bila `"OFF"`, LED dimatikan (`LOW`).


## Penjelasan Fungsi Utama
1. `client.setServer(broker, port)`: Menentukan alamat broker MQTT (IP atau domain) beserta port yang digunakan untuk berkomunikasi (port standarnya 1883).
2. `client.setCallback(callback)`: Mendaftarkan fungsi penangan pesan (callback handler) yang akan dijalankan setiap kali ada pesan masuk dari broker.
3. `client.subscribe(topic)`: Meminta broker untuk meneruskan semua pesan yang dipublikasikan pada topic tertentu ke ESP8266.
4. `client.loop()`: Menjaga sesi MQTT tetap aktif dengan mengirim paket keep-alive/ping, memproses data yang masuk ke buffer, serta memicu pemanggilan fungsi `callback()`.
5. `deserializeJson(doc, payload)`: Membaca payload mentah dan mengubahnya menjadi struktur data JSON di dalam memori mikrokontroler.
6. `digitalWrite(pin, status)`: Mengatur kondisi logika pin output (HIGH/LOW) untuk menyalakan atau mematikan LED sebagai aktuator.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)
1. `if (error)`: Struktur percabangan yang digunakan untuk memeriksa hasil deserialisasi JSON. Apabila data JSON yang diterima rusak atau tidak valid, program akan menampilkan pesan error di Serial Monitor lalu menghentikan proses pengolahan perintah.
2. `if (perintah == "ON") ... else if (perintah == "OFF")`: Percabangan logika yang membandingkan isi string perintah dari JSON untuk menentukan aksi fisik pada LED, yaitu menyalakan atau mematikannya.
3. `while (!client.connected())`: Perulangan berbasis kondisi di dalam fungsi `reconnect()` yang menahan alur program hingga ESP8266 berhasil tersambung kembali ke broker MQTT.

## Konfigurasi Pin
| Komponen       | Pin NodeMCU | Pin GPIO | Modus  | Keterangan                                 |
| :------------- | :---------: | :------: | :----: | :----------------------------------------- |
| Anoda LED (+)  |   Pin D4    |  GPIO2   | OUTPUT | Terhubung ke kaki anoda LED (sisi +)       |
| Katoda LED (-) |   Pin GND   |    -     |   -    | Terhubung ke Ground NodeMCU                |
| Resistor 220Ω  |  Antara D4  |    -     |   -    | Pembatas arus listrik untuk LED (opsional) |
| Built-in LED   |   Pin D4    |  GPIO2   | OUTPUT | LED internal bawaan modul ESP8266          |

## Source Code
**Bagian 1**

![Percobaan4A](<Sourcecode_Percobaan-4A-1.png>)

**Bagian 2**

![Percobaan4A](<Sourcecode_Percobaan-4A-2.png>)

## Pertanyaan Praktikum
1. Gambarkan diagram alur (flowchart) proses penerimaan dan pemrosesan pesan pada fungsi callback di atas! \
**Jawab:**
![Percobaan3A](<Flowchart.png>)
2. Apa yang akan terjadi apabila pesan yang dipublikasikan bukan merupakan format JSON yang valid? \
**Jawab:**
Fungsi deserializeJson() mendeteksi kesalahan struktur dan mengembalikan kode DeserializationError saat struktur data tidak valid. Jika terjadi kesalahan, program menampilkan pesan "Gagal parsing JSON" pada Serial Monitor lalu keluar dari callback() (return) tanpa mengubah kondisi LED, sehingga sistem tetap aman dan tidak hang.
3. Jelaskan mengapa fungsi client.subscribe() dipanggil di dalam fungsi hubungkanMQTT(), bukan di dalam setup()! \
**Jawab:**
Pendaftaran topik subscribe memerlukan koneksi aktif ke broker MQTT. Apabila client.subscribe() diletakkan di setup(), proses tersebut hanya berjalan sekali saat perangkat dinyalakan, sehingga langganan akan hilang ketika koneksi terputus dan tersambung kembali. Dengan memanggil client.subscribe() di dalam hubungkanMQTT(), ESP8266 akan terdaftar ulang pada topik perintah setiap kali berhasil melakukan reconnect.
4. Modifikasi program agar data JSON yang diterima juga memuat nilai intensitas (misalnya {"perintah": "ON", "intensitas": 200}) yang digunakan untuk mengatur kecerahan LED menggunakan PWM (analogWrite/ledcWrite), dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md \
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