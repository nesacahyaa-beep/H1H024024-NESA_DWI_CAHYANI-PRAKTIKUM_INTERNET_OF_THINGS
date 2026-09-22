## Percobaan 3A: Komunikasi Data Menggunakan HTTP

Dokumentasi ini memuat detail pelaksanaan Percobaan 3A mengenai komunikasi data antara mikrokontroler ESP8266 NodeMCU dengan *web server* menggunakan protokol HTTP POST dan enkripsi SSL (HTTPS) dengan format payload JSON.

## Tujuan

1. Memahami konsep komunikasi data berbasis protokol HTTP/HTTPS pada mikrokontroler.
2. Mengonfigurasi pustaka `ESP8266HTTPClient` dan `WiFiClientSecure` untuk melakukan *request* data ke server.
3. Menyusun data berformat JSON (*JavaScript Object Notation*) dan mengisikannya ke dalam *body request* HTTP.
4. Menganalisis *response code* dari server HTTP serta melakukan penanganan kesalahan (*error handling*).

## Spesifikasi yang diharapkan 

1. ESP8266 NodeMCU berhasil terhubung ke jaringan WiFi eksternal dengan status `WL_CONNECTED`.
2. Serial Monitor menampilkan Alamat IP lokal, MAC Address fisik, dan nilai kuat sinyal RSSI (dBm).
3. Indikator LED pada pin D4 menyala (`HIGH`) saat terhubung dan mati (`LOW`) saat koneksi terputus.
4. Program secara otomatis mendeteksi pemutusan jaringan dan melakukan koneksi ulang (*auto-reconnect*) tanpa perlu melakukan *reset* manual pada mikrokontroler.

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

Pada percobaan ini, LED terhubung ke pin **GPIO2 (D4)** pada ESP8266 NodeMCU. ESP8266 dikonfigurasi untuk memancarkan jaringan WiFi mandiri (*Access Point*) yang dapat diakses oleh perangkat lain (seperti *smartphone* atau *laptop*).

## Gambar Rangkaian Percobaan 3A

![Percobaan3A](<Rangkaian_Percobaan-3A.jpeg>)

## Source Code dan Penjelasan Program

Library/Dependencies:
<ESP8266WiFi.h>: Library utama untuk mengelola antarmuka nirkabel pada ESP8266. Pustaka ini menyediakan fungsi pemancaran jaringan (softAP), pembacaan Alamat IP AP, koneksi ke jaringan eksternal, serta pemantauan jumlah station/client yang terhubung secara real-time.

## Penjelasan Logika Program

Alur Kerja Program
1. **Inisialisasi (`setup`)**: Program mengaktifkan komunikasi Serial pada baud rate 115200 bps, mengeset pin D4 sebagai `OUTPUT` dengan kondisi awal `LOW` (LED mati), lalu mengatur mode WiFi ke `WIFI_STA`.
2. **Proses Koneksi**: Fungsi `WiFi.begin(ssid, password)` dipanggil untuk memulai proses autentikasi ke *router*. Program menahan eksekusi pada perulangan `while` sambil mencetak indikator *loading* `.` sampai `WiFi.status()` bernilai `WL_CONNECTED`.
3. **Menampilkan Informasi Jaringan**: Setelah terhubung, program mencetak Alamat IP lokal (`WiFi.localIP()`), MAC Address (`WiFi.macAddress()`), dan kuat sinyal (`WiFi.RSSI()`), kemudian menyalakan LED (`HIGH`).
4. **Monitoring & Auto-Reconnect (`loop`)**: Di dalam fungsi `loop()`, program secara periodik mengecek status koneksi. Jika koneksi terputus (`WiFi.status() != WL_CONNECTED`), LED akan dimatikan, koneksi lama diputus (`WiFi.disconnect()`), lalu program mencoba menghubungkan kembali secara otomatis.

## Penjelasan Fungsi Utama

1. `WiFi.mode(WIFI_STA)`: Mengatur modul WiFi internal ESP8266 agar beroperasi sebagai klien (Station).
2. `WiFi.begin(ssid, password)`: Mengirim kredensial SSID dan Password ke *router* target untuk memulai koneksi.
3. `WiFi.status()`: Mengembalikan nilai status koneksi jaringan saat ini (misalnya `WL_CONNECTED`).
4. `WiFi.localIP()`: Mengambil Alamat IP lokal yang dialokasikan oleh server DHCP *router* ke ESP8266.
5. `WiFi.macAddress()`: Mengambil Alamat MAC (*Media Access Control*) fisik dari antarmuka jaringan ESP8266.
6. `WiFi.RSSI()`: Mengukur nilai RSSI (*Received Signal Strength Indicator*) dalam satuan dBm.
7. `WiFi.disconnect()`: Memutus sesi koneksi jaringan yang aktif atau bermasalah sebelum mencoba koneksi ulang.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)

1. `while (WiFi.status() != WL_CONNECTED)`: Perulangan berbasis kondisi (*conditional loop*) yang digunakan untuk menahan alur program agar tidak berlanjut ke baris berikutnya sebelum ESP8266 benar-benar terhubung ke WiFi.
2. `if (WiFi.status() != WL_CONNECTED)`: Struktur percabangan (*conditional statement*) pada fungsi `loop()` yang berfungsi mendeteksi kondisi darurat apabila jaringan WiFi terputus di tengah jalan, sehingga memicu blok kode *auto-reconnect* dan mematikan LED indikator.

## Konfigurasi Pin

| Komponen       | Pin NodeMCU | Pin GPIO |  Modus | Keterangan                                 |
| :------------- | :---------: | :------: | :----: | :----------------------------------------- |
| Anoda LED (+)  |   Pin D4    |  GPIO2   | OUTPUT | Terhubung ke kaki anoda LED (sisi +)       |
| Katoda LED (-) |   Pin GND   |    -     |   -    | Terhubung ke Ground NodeMCU                |
| Resistor 220Ω  |  Antara D4  |    -     |   -    | Pembatas arus listrik untuk LED (opsional) |
| Built-in LED   |   Pin D4    |  GPIO2   | OUTPUT | LED internal bawaan modul ESP8266          |

## Source Code

![Percobaan3A](<Sourcecode-1.png>)

## Pertanyaan Praktikum

1. Gambarkan diagram alur (flowchart) proses pengiriman data melalui HTTP POST pada
program di atas! /
**Jawab:**
![Percobaan3A](<Flowchart.png>)

2. Apa fungsi dari perintah http.addHeader("Content-Type", "application/json") pada program
tersebut? /
**Jawab:**
Berfungsi untuk memberi tahu kepada server bahwa data (payload) dalam
permintaan HTTP POST memiliki format JSON, sehingga server dapat mengenali dan
mengolah data tersebut dengan benar.

3. Jelaskan arti dari kode response HTTP 200 dan sebutkan salah satu contoh kode response
HTTP lain beserta artinya! /
**Jawab:**
HTTP 200 (OK): Permintaan HTTP berhasil diterima, diproses, dan direspon oleh
server dengan sukses.
Contoh Kode Lain: 404 Not Found (artinya URL/endpoint tujuan tidak ditemukan di
server) atau 500 Internal Server Error (terjadi kesalahan pada sistem internal server).

4. Modifikasi program agar ESP32 dapat mengirimkan data tambahan berupa waktu (dalam
milidetik sejak dinyalakan menggunakan millis()) ke dalam JSON yang dikirim, dan
berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md /
**Jawab:**

![Percobaan 3A Bagian 1](<Modifikasi_Percoban-1.png>)