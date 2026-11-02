# Percobaan 1: Akuisisi Data Sensor

Dokumentasi ini memuat detail pelaksanaan Percobaan 1 mengenai proses akuisisi data suhu dan kelembaban menggunakan sensor DHT11 yang terhubung dengan mikrokontroler ESP8266 NodeMCU.

## Tujuan

1. Memahami proses akuisisi data suhu dan kelembaban menggunakan sensor DHT11 melalui mikrokontroler ESP8266.
2. Mengamati perubahan nilai suhu dan kelembaban sensor pada kondisi lingkungan yang berbeda.
3. Memahami proses pembacaan dan penampilan data sensor melalui Serial Monitor.

## Spesifikasi yang Diharapkan

1. ESP8266 NodeMCU dapat membaca data suhu dan kelembaban dari sensor DHT11.
2. Data suhu dan kelembaban dapat ditampilkan pada Serial Monitor.
3. Program dapat mendeteksi kegagalan pembacaan sensor dan menampilkan pesan peringatan.
4. Nilai suhu dan kelembaban dapat diamati pada kondisi lingkungan yang berbeda.

## Alat dan Bahan

- ESP8266 NodeMCU
- Sensor DHT11
- Breadboard
- Kabel jumper
- Kabel USB

## Rangkaian Percobaan

Pada percobaan ini, sensor DHT11 dihubungkan dengan ESP8266 NodeMCU. Sensor digunakan untuk membaca nilai suhu dan kelembaban lingkungan, kemudian data hasil pembacaan ditampilkan melalui Serial Monitor.

## Gambar Rangkaian Percobaan 1A

![Percobaan1A](<Rangkaian_Percobaan-1.jpeg>)

# Source Code - Percobaan 1

Folder ini berisi kode program untuk akuisisi data suhu dan kelembaban udara menggunakan sensor DHT11 berbasis mikrokontroler NodeMCU ESP8266.

## Penjelasan Logika Program

Kode pada Percobaan 1 Akuisisi Data Sensor DHT11. Berikut adalah alur logikanya:

1. Program melakukan inisialisasi komunikasi Serial pada baud rate 115200 bps serta mengaktifkan sensor DHT11.
2. Mikrokontroler membaca nilai kelembaban (`readHumidity`) dan suhu (`readTemperature`) dari sensor DHT11 secara berkala.
3. Program mengecek kevalidan data menggunakan fungsi `isnan()`. Jika data bernilai *NaN* (gagal terbaca), Serial Monitor akan menampilkan pesan kesalahan. Jika data valid, Serial Monitor akan menampilkan nilai suhu (°C) dan kelembaban (%).
4. Proses pembacaan sensor diberi jeda waktu (*delay*) selama 500 ms sebelum melakukan pengulangan *loop*.

## Konfigurasi Pin

| Komponen     | Pin NodeMCU |  Modus  | Keterangan                          |
| :----------- | :---------- | :-----: | :---------------------------------- |
| Pin DATA DHT | Pin D4      | `INPUT` | Pin data sinyal (GPIO2)             |
| Pin VCC DHT  | Pin 3V3     |    -    | Sumber tegangan 3.3V dari NodeMCU   |
| Pin GND DHT  | Pin GND     |    -    | Ground NodeMCU                      |

## Sour Code

![Percobaan1A](<Sourcode-1.png>)

## Pertanyaan Praktikum

1. Gambarkan diagram alur (flowchart) proses akuisisi data sensor DHT22 pada program di atas! \
**Jawab:**
![Percobaan1A](<Flowchart.png>)

2. Apa fungsi dari perintah isnan() pada program tersebut? \
**Jawab:** digunakan untuk mengecek apakah data sensor yang diterima tidak valid atau mengalami error. 

3. Jelaskan mengapa diperlukan jeda (delay) minimal sekitar 2 detik antar pembacaan sensor DHT11! \
**Jawab:** Sensor DHT11 membutuhkan jeda waktu tertentu untuk mengubah data suhu dan kelembaban menjadi sinyal digital yang stabil agar hasil pembacaan tetap akurat.  

4. Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan sebelum ditampilkan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md! \
**Jawab:**

![Percobaan1A](<Modifikasi_Percobaan-1A.png>)
