# Percobaan 2: Pengontrolan Relay dan Akuisisi Sensor

Dokumentasi ini memuat detail pelaksanaan Percobaan 2 mengenai integrasi sensor DHT11 dan modul relay menggunakan mikrokontroler ESP8266 NodeMCU untuk pengontrolan aktuator secara otomatis.

## Tujuan

1. Memahami prinsip kerja modul relay sebagai sakelar elektronik yang dikendalikan mikrokontroler.
2. Memahami cara mengintegrasikan sensor DHT11 dengan modul relay pada ESP8266 NodeMCU.
3. Mengimplementasikan logika kontrol (ambang batas) untuk memicu kondisi ON/OFF pada relay berdasarkan data suhu atau kelembaban.
4. Mengamati perubahan status relay melalui indikator LED modul serta pantauan pada Serial Monitor.

## Spesifikasi yang Diharapkan

1. ESP8266 NodeMCU dapat membaca data suhu dan kelembaban dari sensor DHT11.
2. Modul relay dapat aktif (ON) atau mati (OFF) secara otomatis sesuai logika threshold yang ditentukan.
3. Status pembacaan sensor dan status aktif/nonaktif relay dapat ditampilkan secara *real-time* di Serial Monitor.
4. Sistem memiliki jeda atau histori kendali yang stabil saat mendeteksi perubahan kondisi lingkungan.

## Alat dan Bahan

- ESP8266 NodeMCU
- Sensor DHT11
- Modul Relay 2 Channel
- Breadboard
- Kabel jumper
- Kabel USB

## Rangkaian Percobaan

Pada percobaan ini, sensor DHT11 dan modul relay 2 channel dihubungkan dengan mikrokontroler ESP8266 NodeMCU. Sensor DHT11 berfungsi untuk membaca nilai suhu dan kelembaban lingkungan, sedangkan modul relay digunakan sebagai aktuator sakelar otomatis yang dikendalikan berdasarkan kondisi data sensor. Data hasil pembacaan sensor beserta status kondisi relay ditampilkan secara *real-time* melalui Serial Monitor.

## Gambar Rangkaian Percobaan 2A

![Percobaan1A](<Rangkaian_Percobaan-2.jpeg>)

# Source Code - Percobaan 2

Folder ini berisi kode program untuk akuisisi data suhu menggunakan sensor DHT22 serta pengontrolan otomatis aktuator (relay/LED) berbasis mikrokontroler.

## Penjelasan Logika Program

Kode pada Percobaan 2 Pengontrolan Aktuator Berdasarkan Data Sensor. Berikut adalah alur logikanya:

1. Program menetapkan ambang batas (*threshold*) suhu sebesar 30.0 °C, menginisialisasi komunikasi Serial pada baud rate 115200 bps, mengaktifkan sensor DHT22, serta mengkonfigurasi `RELAYPIN` sebagai `OUTPUT` dengan kondisi awal `LOW` (aktuator mati).
2. Mikrokontroler membaca data suhu (`readTemperature`) dari sensor DHT22.
3. Program mengecek kevalidan data menggunakan fungsi `isnan()`. Jika pembacaan gagal, Serial Monitor menampilkan pesan kesalahan.
4. Jika data suhu valid, program membandingkan nilai suhu dengan *threshold*:
   - Jika suhu **> 30.0 °C**, pin relay diberi sinyal `HIGH` dan Serial Monitor menampilkan status `"Aktuator: ON"`.
   - Jika suhu **≤ 30.0 °C**, pin relay diberi sinyal `LOW` dan Serial Monitor menampilkan status `"Aktuator: OFF"`.

## Konfigurasi Pin

| Komponen      |  Pin Board   |  Modus   | Keterangan                            |
| :------------ | :----------: | :------: | :------------------------------------ |
| Pin DATA DHT  |    GPIO 4    | `INPUT`  | Pin sinyal data sensor DHT22          |
| Pin VCC DHT   |   3V3 / 5V   |    -     | Sumber tegangan sensor                |
| Pin GND DHT   |     GND      |    -     | Ground sensor                         |
| Pin IN Relay  |   GPIO 26    | `OUTPUT` | Pin sinyal kendali aktuator/relay     |
| Pin VCC Relay |      5V      |    -     | Sumber tegangan modul relay           |
| Pin GND Relay |     GND      |    -     | Ground modul relay                    |

## Sour Code

![Percobaan2A](<Sourcode-2.png>)

## Pertanyaan Praktikum

1. Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?  \
**Jawab :** Nilai threshold digunakan sebagai batas bagi mikrokontroler untuk menentukan kondisi sensor yang akan menghasilkan keputusan HIGH atau LOW pada aktuator. 

2. Jelaskan apa yang akan terjadi apabila nilai suhuThreshold diturunkan menjadi sangat rendah, misalnya 20.0! \
**Jawab:** Aktuator atau relay akan tetap menyala (ON) karena suhu ruangan rata-rata berada di atas 20,0 °C.

3. Apa perbedaan antara kendali aktuator secara terus-menerus (kondisi tunggal) dengan kendali menggunakan histerisis (dua ambang batas)? \  
**Jawab:** Kondisi dengan satu threshold dapat menyebabkan chattering, yaitu relay terlalu sering ON-OFF saat suhu berubah di sekitar batas. Kendali histerisis memakai dua batas, yaitu batas atas untuk ON dan batas bawah untuk OFF, sehingga aktuator lebih stabil.  

4. Modifikasi program agar menggunakan dua ambang batas (histerisis), misalnya aktuator menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah 28°C, dan berikan penjelasan di setiap baris kode nya dalam bentuk README.md! \
**Jawab:**

![Percobaan2A](<Modifikasi_Percobaan-2A.png>)
