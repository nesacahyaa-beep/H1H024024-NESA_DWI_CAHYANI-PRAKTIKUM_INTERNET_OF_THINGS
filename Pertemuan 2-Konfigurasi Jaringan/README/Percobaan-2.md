## Percobaan 2: Konfigurasi WiFi Mode Access Point (AP)

Dokumentasi ini memuat detail pelaksanaan Percobaan 2B mengenai konfigurasi mikrokontroler ESP8266 NodeMCU sebagai Access Point (AP) mandiri untuk memancarkan jaringan WiFi, memantau jumlah perangkat (client) yang terhubung, serta mengimplementasikan mode gabungan (AP+STA).

## Tujuan

1. Memahami konsep serta konfigurasi mode Access Point (AP) pada modul ESP8266 NodeMCU.
2. Melakukan konfigurasi ESP8266 sebagai penyedia jaringan nirkabel secara mandiri menggunakan SSID dan Password yang telah ditentukan.
3. Mengetahui Alamat IP default Access Point serta memantau jumlah *client* yang terhubung secara *real-time*.
4. Menerapkan mode ganda (AP+STA) sehingga ESP8266 dapat terhubung ke jaringan WiFi eksternal sekaligus menyediakan *hotspot* lokal secara bersamaan.

## Spesifikasi yang diharapkan

1. ESP8266 NodeMCU berhasil memancarkan jaringan WiFi dengan SSID `ESP8266-PraktikumIoT` dan Password yang telah ditentukan.
2. Serial Monitor berhasil menampilkan Alamat IP default AP (`192.168.4.1`) serta mendeteksi jumlah perangkat yang terhubung setiap 5 detik.
3. Perangkat *client*, seperti smartphone atau laptop, berhasil menemukan dan terhubung ke jaringan WiFi yang dibuat oleh ESP8266.
4. Program modifikasi berhasil menjalankan mode `WIFI_AP_STA` secara stabil.

## Alat dan Bahan

- ESP8266
- LED 
- Breadboard
- Kabel Jumper
- Kabel Micro-USB

## Rangkaian Percobaan

Pada percobaan ini, LED terhubung ke pin GPIO2 (D4) ESP8266. ESP8266 dikonfigurasi memancarkan jaringan WiFi mandiri (Access Point) yang dapat diakses oleh perangkat lain.

## Gambar Rangkaian Percobaan 2B

![Percobaan2B](<Rangkaian_Percobaan-2B.jpeg>)

## Source Code & Penjelasan Program

Library / Dependencies:
<ESP8266WiFi.h>: Library utama untuk mengelola antarmuka nirkabel pada ESP8266, menyediakan fungsi pemancaran jaringan (softAP), pembacaan IP AP, dan pemantauan jumlah station/client.

## Penjelasan Logika Program

Alur logika program Percobaan 2B (Mode AP+STA) adalah sebagai berikut:

1. Inisialisasi Mode Ganda (`setup`): Program memulai komunikasi Serial dengan baud rate 115200 bps, kemudian mengatur mode WiFi ke `WIFI_AP_STA` agar ESP8266 dapat berfungsi sebagai klien sekaligus pemancar WiFi.
2. Koneksi ke Router (Station): ESP8266 menggunakan `WiFi.begin(ssid, password)` untuk terhubung ke hotspot eksternal. Program menunggu hingga koneksi berhasil, kemudian menampilkan Alamat IP Station pada Serial Monitor.
3. Mengaktifkan Access Point: Program membuat jaringan WiFi lokal menggunakan `WiFi.softAP(ap_ssid, ap_password)`. Alamat IP default AP (`192.168.4.1`) diperoleh melalui `WiFi.softAPIP()` dan ditampilkan pada Serial Monitor.
4. Monitoring Client (`loop`): Pada fungsi `loop()`, `WiFi.softAPgetStationNum()` digunakan secara berkala setiap 5 detik untuk mengetahui jumlah perangkat yang terhubung ke jaringan AP ESP8266.

## Penjelasan Fungsi Utama

1. WiFi.mode(WIFI_AP_STA): Mengonfigurasi radio WiFi ESP8266 agar beroperasi ganda sebagai Access Point sekaligus Station.
2. WiFi.softAP(ap_ssid, ap_password): Mengaktifkan modul hotspot (Software Access Point) dengan nama SSID dan kata sandi yang telah ditentukan.
3. WiFi.softAPIP(): Mengambil Alamat IP bawaan dari jaringan Access Point yang dibuat oleh ESP8266 (secara default 192.168.4.1).
4. WiFi.softAPgetStationNum(): Mengembalikan jumlah perangkat (client/station) yang saat ini terhubung ke Access Point ESP8266 secara real-time.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)

while (WiFi.status() != WL_CONNECTED): Perulangan penahan pada fungsi setup() yang memastikan ESP8266 tersambung terlebih dahulu ke WiFi eksternal sebelum melanjutkan ke proses pengaktifan Access Point internal.

## Konfigurasi Pin

| Komponen       | Pin NodeMCU  | Modus    | Keterangan                           |
| :------------- | :----------: | :------: | :----------------------------------- |
| Anoda LED (+)  | Pin D4/GPIO2 | `OUTPUT` | Terhubung ke kaki anoda LED (sisi +) |
| Katoda LED (-) |   Pin GND    |    -     | Terhubung ke Ground NodeMCU          |
| Resistor 220Ω  |  Antara D4   |    -     | Pembatas arus untuk LED (opsional)   |

## Source Code

![Percobaan2B](<Sourcecode-2.png>)

## Pertanyaan Praktikum

1. Mengapa alamat IP default Access Point pada ESP32 umumnya bernilai 192.168.4.1? \
**Jawab:**
Alamat IP 192.168.4.1 merupakan konfigurasi bawaan pada perangkat ESP8266 saat beroperasi dalam mode Access Point (AP). Penggunaan alamat tersebut bertujuan untuk menghindari konflik dengan alamat gateway yang umum digunakan oleh router, seperti 192.168.1.1 atau 192.168.0.1.  

2. Apa perbedaan mendasar antara mode Station dan mode Access Point pada ESP32? \
**Jawab:**
Mode STA berfungsi sebagai klien yang terhubung ke jaringan WiFi dan memperoleh alamat IP dari router. Sementara itu, mode AP memungkinkan mikrokontroler bertindak sebagai penyedia jaringan atau hotspot yang dapat memberikan alamat IP kepada perangkat lain yang terhubung.  

3. Jelaskan risiko keamanan apabila password Access Point tidak diberikan atau terlalu sederhana! \
**Jawab:**
 Membuka celah bagi pengguna asing untuk terhubung, melakukan interupsi data, atau mengambil alih kendali sistem. 

 4. Modifikasi program agar ESP32 berjalan pada mode AP+STA (terhubung ke WiFi rumah sekaligus menyediakan Access Point), dan berikan penjelasan di setiap baris kode nya dalam bentuk README.md! \
 **Jawab:**

 ![Percobaan2B](<Modifikasi_Percobaan-2B.png>)
