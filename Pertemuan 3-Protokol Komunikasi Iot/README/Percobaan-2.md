## Percobaan 3B: Konfigurasi WiFi Mode Access Point (AP)

Dokumentasi ini memuat detail pelaksanaan Percobaan 3B mengenai implementasi protokol komunikasi MQTT (*Message Queuing Telemetry Transport*) pada mikrokontroler ESP8266 NodeMCU. Percobaan ini mencakup pengiriman data (*Publish*) dan penerimaan perintah (*Subscribe*) melalui *MQTT Broker*.

## Tujuan

1. Memahami konsep arsitektur *Publish-Subscribe* serta peran *MQTT Broker* dalam ekosistem IoT.
2. Mengonfigurasi pustaka `PubSubClient` pada ESP8266 untuk terhubung dengan *MQTT Broker*.
3. Mengirimkan (*Publish*) data telemetry/status perangkat secara periodik ke topik tertentu.
4. Menerima (*Subscribe*) pesan perintah dari topik tertentu menggunakan fungsi *callback* untuk mengontrol hardware (LED).

## Spesifikasih yang diharapkan

1. ESP8266 NodeMCU berhasil terhubung ke jaringan WiFi dan melakukan otentikasi ke *MQTT Broker* (misal: `broker.hivemq.com`).
2. Perangkat berhasil mempublikasikan (*Publish*) data status setiap 5 detik ke topik `praktikum/iot/data`.
3. Perangkat berhasil berlangganan (*Subscribe*) ke topik `praktikum/iot/kendali` dan mengontrol kondisi LED (ON/OFF) secara *real-time*.
4. Program menyertakan fungsi *auto-reconnect* jika koneksi ke *MQTT Broker* terputus.

## Alat dan Bahan

1. ESP8266
2. Kabel USB
3. Laptop/PC
4. Arduino IDE
5. Library ArduinoJson
6. Library PubSubClient
7. aringan WiFi yang terhubung ke internet
8. Aplikasi MQTT Explorer atau HiveMQ WebSocket Client
9. Broker MQTT broker.hivemq.com
10. Endpoint HTTP httpbin.org/post

## Rangkaian Percobaan

Pada percobaan ini, LED terhubung ke pin GPIO2 (D4) ESP8266. Selain berfungsi sebagai indikator visual lokal, LED ini dapat dikendalikan secara jarak jauh melalui pesan MQTT yang dikirimkan oleh klien lain (misal: MQTT Dash / Serial / Node-RED) ke topik kendali.

## Gambar Rangkaian Percobaan 3B

![Percobaan3A](<Rangkaian_Percobaan-3B.jpeg>)

## Source Code & Penjelasan Program

Library / Dependencies:
1. `<ESP8266WiFi.h>`: Mengelola koneksi jaringan nirkabel pada modul ESP8266.
2. `<PubSubClient.h>`: Pustaka utama komunikasi MQTT yang memfasilitasi pembuatan koneksi client, proses *publish*, *subscribe*, dan manajemen *callback*.

## Penjelasan Logika Program

Alur Kerja Program
1. Inisialisasi & Setup: Program mengaktifkan komunikasi Serial, mengatur pin LED sebagai `OUTPUT`, serta menentukan alamat *MQTT Broker* dan *port* (1883) melalui `client.setServer()`.
2. Koneksi WiFi & Broker: ESP8266 terhubung ke WiFi terlebih dahulu, kemudian memanggil fungsi `reconnect()` untuk menghubungkan *client* ke *MQTT Broker* menggunakan `clientID` unik.
3. Mendaftarkan Topic (Subscribe): Setelah terhubung ke Broker, ESP8266 mendaftarkan diri (*Subscribe*) ke topik `praktikum/iot/kendali` dan menetapkan fungsi `callback()` sebagai penanganan pesan masuk.
4. Handling Incoming Message (Callback): Setiap ada pesan masuk pada topik berlangganan, fungsi `callback()` mengeksekusi perintah. Jika payload bernilai `"ON"`, LED menyala; jika `"OFF"`, LED mati.
5. Periodic Publishing & Loop: Pada fungsi `loop()`, perintah `client.loop()` dipanggil terus-menerus untuk menjaga sesi MQTT. Setiap 5 detik, ESP8266 mempublikasikan (*Publish*) data telemetry ke topik `praktikum/iot/data`.

## Penjelasan Fungsi Utama

1. `client.setServer(broker, port)`: Mengonfigurasi alamat IP/domain *MQTT Broker* beserta nomor port-nya.
2. `client.setCallback(callback)`: Menentukan fungsi penangan (*handler*) yang otomatis dipanggil saat pesan masuk dari topik *subscribe*.
3. `client.subscribe(topic)`: Mendaftarkan perangkat untuk mendengarkan lalu lintas pesan pada topik tertentu.
4. `client.publish(topic, payload)`: Mengirimkan data berupa *string* atau JSON ke topik tujuan di Broker.
5. `client.loop()`: Memproses antrean pesan masuk, menjaga *heartbeat/ping* koneksi MQTT, serta memastikan koneksi tetap hidup.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)

1. `while (!client.connected())`: Perulangan berbasis kondisi pada fungsi `reconnect()` yang berfungsi menahan alur program dan terus mencoba menghubungkan ulang ESP8266 ke *MQTT Broker* sampai koneksi berhasil dibuat.
2. `if (now - lastMsg > 5000)`: Struktur percabangan penanda waktu (*non-blocking timer*) berbasis fungsi `millis()` untuk memicu pengiriman data *publish* setiap 5 detik sekali tanpa menghentikan eksekusi program (`delay`).
3. `if (String(topic) == topic_subscribe)`**: Percabangan pada fungsi `callback()` yang memastikan bahwa pesan yang diterima benar-benar berasal dari topik kendali sebelum mengeksekusi logika perintah (`ON`/`OFF`) pada LED.

## Konfigurasi Pin

| Komponen       | Pin NodeMCU | Pin GPIO |  Modus | Keterangan                                 |
| :------------- | :---------: | :------: | :----: | :----------------------------------------- |
| Anoda LED (+)  |   Pin D4    |  GPIO2   | OUTPUT | Terhubung ke kaki anoda LED (sisi +)       |
| Katoda LED (-) |   Pin GND   |    -     |   -    | Terhubung ke Ground NodeMCU                |
| Resistor 220Ω  |  Antara D4  |    -     |   -    | Pembatas arus listrik untuk LED (opsional) |

## Source Code

![Percobaan3B](<Sourcecode-2.png>)

## Pertanyaan Praktikum

1. Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu
dibuat unik? \
**Jawab**: Topic berfungsi sebagai alamat untuk mengelompokkan pesan, sehingga broker
dapat menyalurkan data dari publisher kepada subscriber yang sesuai. Penggunaan topic
yang unik diperlukan untuk menghindari tercampurnya data atau konflik dengan pengguna
lain pada broker publik.
2. Jelaskan fungsi dari perintah client.loop() yang dipanggil pada setiap iterasi loop()! \
**Jawab:** Berfungsi untuk menjaga koneksi TCP dengan broker tetap aktif (keep-alive ping),
mengelola pengiriman buffer pesan, serta memproses pesan data masuk (incoming
messages) secara terus menerus.
3. Apa yang akan terjadi apabila koneksi ke broker MQTT terputus di tengah program
berjalan? \
**Jawab**: Pengiriman data publish akan gagal. Namun, pada kode program terdapat
pengecekan if (!client.connected()), sehingga ESP akan mendeteksi terputusnya koneksi
dan secara otomatis memanggil hubungkanMQTT() untuk mencoba menyambungkan
ulang setiap 2 detik.