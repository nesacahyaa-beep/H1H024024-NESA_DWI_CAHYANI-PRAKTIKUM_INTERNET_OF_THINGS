## Percobaan 1: Konfigurasi WiFi Mode Station (STA)

Dokumentasi ini memuat detail pelaksanaan Percobaan 2A mengenai konfigurasi mikrokontroler ESP8266 NodeMCU sebagai Station (STA) untuk terhubung ke jaringan WiFi eksternal, menampilkan informasi parameter jaringan pada Serial Monitor, serta mengimplementasikan indikator koneksi dan fitur auto-reconnect.

## Tujuan

1. Memahami konsep dasar jaringan nirkabel (WiFi) serta pengaturan mode jaringan pada perangkat ESP8266.
2. Menerapkan konfigurasi mode Station (STA) untuk menghubungkan mikrokontroler dengan jaringan WiFi yang tersedia.
3. Menerapkan mode Access Point (AP) serta mode kombinasi (AP+STA) pada mikrokontroler untuk menyediakan jaringan secara mandiri.
4. Mampu memperoleh dan menganalisis informasi jaringan, meliputi IP Address, MAC Address, serta tingkat kekuatan sinyal (RSSI).

## Alat dan Bahan

- ESP8266
- LED 
- Breadboard
- Kabel Jumper
- Kabel Micro-USB

## Rangkaian Percobaan

Pada percobaan ini, LED dihubungkan ke pin GPIO2 (D4) ESP8266 sebagai indikator status jaringan. ESP8266 dikonfigurasi dalam mode Station untuk menangkap sinyal WiFi dari router/hotspot.

## Gambar Rangkaian Percobaan 2A

![Percobaan2A](<Rangkaian_Percobaan-2A.jpeg>)

## Source Code & Penjelasan Program

Library / Dependencies:
<ESP8266WiFi.h>: Library bawaan board package ESP8266 pada Arduino IDE yang berfungsi mengelola konfigurasi hardware WiFi, koneksi jaringan, pembacaan IP Address, MAC Address, dan sinyal RSSI.

## Penjelasan Logika Program

Alur logika dari program Percobaan 2A adalah sebagai berikut:

1. Inisialisasi (setup): Program mengaktifkan komunikasi Serial pada baud rate 115200 bps, mengeset pin D4 sebagai OUTPUT dengan kondisi awal LOW (LED mati), lalu mengatur mode WiFi ke WIFI_STA.
2. Proses Koneksi: Fungsi WiFi.begin(ssid, password) dipanggil untuk memulai proses autentikasi ke router. Program menahan eksekusi pada perulangan while sambil mencetak indikator loading . sampai WiFi.status() bernilai WL_CONNECTED.
3. Menampilkan Informasi Jaringan: Setelah terhubung, program mencetak Alamat IP lokal (WiFi.localIP()), MAC Address (WiFi.macAddress()), dan kuat sinyal (WiFi.RSSI()), kemudian menyalakan LED (HIGH).
4. Monitoring & Auto-Reconnect (loop): Di dalam fungsi loop(), program secara periodik mengecek status koneksi. Jika koneksi terputus (WiFi.status() != WL_CONNECTED), LED akan dimatikan, koneksi lama diputus (WiFi.disconnect()), lalu program mencoba menghubungkan kembali secara otomatis.

## Penjelasan Fungsi Utama

1. WiFi.mode(WIFI_STA): Mengatur modul WiFi internal ESP8266 agar beroperasi sebagai klien (Station).
2. WiFi.begin(ssid, password): Mengirim kredensial SSID dan Password ke router target untuk memulai koneksi.
3. WiFi.status(): Mengembalikan nilai status koneksi jaringan saat ini (misalnya WL_CONNECTED).
4. WiFi.localIP(): Mengambil Alamat IP lokal yang dialokasikan oleh server DHCP router ke ESP8266.
5. WiFi.macAddress(): Mengambil Alamat MAC (Media Access Control) fisik dari antarmuka jaringan ESP8266.
6. WiFi.RSSI(): Mengukur nilai RSSI (Received Signal Strength Indicator) dalam satuan dBm.
7. WiFi.disconnect(): Memutus sesi koneksi jaringan yang aktif atau bermasalah sebelum mencoba koneksi ulang.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)

while (WiFi.status() != WL_CONNECTED): Perulangan berbasis kondisi (conditional loop) yang digunakan untuk menahan alur program agar tidak berlanjut ke baris berikutnya sebelum ESP8266 benar-benar terhubung ke WiFi.

if (WiFi.status() != WL_CONNECTED): Struktur percabangan (conditional statement) pada fungsi loop() yang berfungsi mendeteksi kondisi darurat apabila jaringan WiFi terputus di tengah jalan, sehingga memicu blok kode auto-reconnect dan mematikan LED indikator.

## Konfigurasi Pin

| Komponen       | Pin NodeMCU  | Modus    | Keterangan                           |
| :------------- | :----------: | :------: | :----------------------------------- |
| Anoda LED (+)  | Pin D4/GPIO2 | `OUTPUT` | Terhubung ke kaki anoda LED (sisi +) |
| Katoda LED (-) |   Pin GND    |    -     | Terhubung ke Ground NodeMCU          |
| Resistor 220Ω  |  Antara D4   |    -     | Pembatas arus untuk LED (opsional)   |

## Source Code

**Bagian 1:**
![Percobaan 2A Bagian 1](<Sourcode-1A.png>)

**Bagian 2:**
![Percobaan 2A Bagian 2](<Sourcode-1B.png>)

## Pertanyaan Praktikum

1. Gambarkan diagram alur (flowchart) proses koneksi ESP32 ke jaringan WiFi pada program di atas! \
**Jawab:**
![Percobaan2A](<Flowchart.jpg>)

2. Apa fungsi dari perintah WiFi.mode(WIFI_STA) pada program tersebut? \
**Jawab:**
Mengkonfigurasi perangkat WiFi pada mikrokontroler agar berfungsi sebagai Station (client) yang terhubung ke jaringan melalui router atau hotspot eksternal.  

3.Jelaskan apa yang terjadi apabila SSID atau password yang dimasukkan salah! \
**Jawab:**
Mikrokontroler gagal menyelesaikan proses handshake autentikasi sehingga WiFi.status() terus menghasilkan nilai selain WL_CONNECTED. Kondisi tersebut menyebabkan program terjebak dalam infinite loop pada blok while, ditandai dengan keluaran berupa tanda titik yang terus muncul pada Serial Monitor.  

4. Modifikasi program agar ESP32 mencoba menghubungkan ulang (reconnect) secara otomatis apabila koneksi WiFi terputus, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md! \
**Jawab:**

**Bagian 1:**
![Percobaan 2A Bagian 1](<Modifikasi_Percobaan_2A1.png>)

**Bagian 2:**
![Percobaan 2A Bagian 2](<Modifikasi_Percobaan-2A2.png>)
