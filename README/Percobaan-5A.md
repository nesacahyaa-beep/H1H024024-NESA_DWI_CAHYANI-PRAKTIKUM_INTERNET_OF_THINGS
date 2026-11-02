## Percobaan 5A Modif: Pengolahan Data Sensor pada Sisi Perangkat (Edge Processing)

Dokumentasi ini memuat detail pelaksanaan Percobaan 5A Modif mengenai pengolahan data sensor suhu langsung di sisi perangkat (*edge processing*) menggunakan mikrokontroler ESP8266. Pemrosesan data dilakukan dengan algoritma pemfilteran *Moving Average* berbasis *circular buffer*, perhitungan variabel statistik *Simpangan Baku* (Standard Deviation), serta pencatatan nilai *Minimum* dan *Maksimum* secara *real-time*.

## Tujuan
1. Memahami proses pengolahan dan analisis data yang diterapkan dalam sistem IoT.
2. Mengetahui perbedaan pengolahan data pada perangkat (edge) dengan pengolahan data di server atau komputer.
3. Menerapkan metode moving average sebagai filter serta melakukan perhitungan statistik sederhana menggunakan ESP8266.
4. Menerapkan proses pengumpulan data IoT melalui protokol MQTT menggunakan Python dan menyimpan hasilnya dalam format file CSV.
5. Menganalisis dan menyajikan data sensor dalam bentuk visualisasi serta mengidentifikasi data yang menyimpang (anomali) menggunakan metode z-score.

## Spesifikasi yang diharapkan
1. Mikrokontroler berhasil mensimulasikan akuisisi data suhu mentah yang bervariasi beserta lonjakan anomali secara berkala.
2. Fungsi *Moving Average* berhasil menghitung rata-rata bergerak dari 5 sampel data terakhir menggunakan mekanisme *circular buffer*.
3. Fungsi perhitungan *Simpangan Baku* berhasil mengukur tingkat sebaran data pada *buffer* secara akurat.
4. Perangkat secara otomatis memperbarui catatan suhu minimum dan maksimum sejak program dijalankan.
5. Serial Monitor menampilkan data mentah, data hasil filter, simpangan baku, nilai minimum, dan nilai maksimum secara terstruktur setiap 2 detik.

## Alat dan Bahan
1. Board ESP8266 (1 buah)
2. Sensor DHT22 (1 buah)
3. Breadboard dan Kabel Jumper (secukupnya)
4. Kabel USB (Micro-USB/USB-C sesuai board)
5. Laptop/PC dengan Arduino IDE (pustaka DHT sensor library, PubSubClient, dan ArduinoJson) dan Python 3 (pustaka paho-mqtt, pandas, dan matplotlib)
6. Jaringan WiFi yang terhubung ke internet
7. Broker MQTT publik broker.hivemq.com (port 1883)

## Rangkaian Percobaan
Pada percobaan ini, seluruh proses pengolahan data dilakukan secara lokal di dalam memori mikrokontroler (*edge processing*). Data suhu diakuisisi melalui fungsi simulasi `bacaSuhuMock()` yang memodelkan sifat dinamik sensor DHT22. Mikrokontroler menyimpan sampel ke dalam *array buffer* berukuran $N=5$, menghitung pemfilteran serta variabel statistik deskriptif, lalu menampilkan hasilnya langsung ke Serial Monitor tanpa tergantung pada koneksi internet maupun broker MQTT.

## Gambar Rangkaian Percobaan 5A
![Percobaan 5A Bagian 1](<RangkaianPercobaan- 5A & 5B.png>)

## Source Code dan Penjelasan Program
Library/Dependencies:
`<math.h>`: Pustaka standar C/C++ yang menyediakan fungsi matematika tingkat lanjut seperti `sin()` untuk membentuk gelombang suhu simulasi dan `sqrt()` untuk kalkulasi akar kuadrat pada perhitungan simpangan baku.

## Penjelasan Logika Program
**Alur Kerja Program**
1. **Inisialisasi (`setup`)**: Saat program diaktifkan, ESP8266 mengalokasikan komunikasi Serial dengan kecepatan 115200 bps dan menginisialisasi generator angka acak (`randomSeed`) menggunakan pembacaan pewaktu mikrosekond (`micros()`).
2. **Akuisisi Data Simulasi (`bacaSuhuMock`)**: Mengambil sampel data suhu buatan yang menggabungkan pola gelombang sinusoidal, *noise* fluktuasi acak, serta lonjakan anomali sebesar $+8.0^\circ\text{C}$ setiap 25 kali pembacaan.
3. **Pemfilteran *Moving Average* (`hitungMovingAverage`)**: Data baru dimasukkan ke dalam *array buffer*. Indeks penulisan diperbarui dengan logika *circular buffer*. Program menghitung rata-rata dari seluruh sampel yang tersimpan di dalam *buffer*.
4. **Kalkulasi Simpangan Baku (`hitungSimpanganBaku`)**: Program menghitung rata-rata *buffer*, mengkalkulasi kuadrat selisih tiap elemen sampel terhadap rata-rata, membaginya dengan jumlah sampel untuk memperoleh varians, lalu mengambil akar kuadratnya.
5. **Pelacakan Min/Max dan Output**: Program membandingkan data mentah dengan variabel `suhuMin` dan `suhuMax`, lalu mencetak seluruh parameter hasil analisis ke Serial Monitor sebelum menunda eksekusi selama 2 detik (`delay(2000)`).

## Penjelasan Fungsi Utama
1. `bacaSuhuMock()`: Fungsi simulasi yang menghasilkan data suhu sintetis bervariasi dengan menambahkan derau acak dan lonjakan anomali periodik.
2. `hitungMovingAverage(float nilaiBaru)`: Menyimpan data baru ke dalam *buffer*, menggeser indeks secara sirkular `(indeks + 1) % N`, dan mengembalikan nilai rata-rata sampel.
3. `hitungSimpanganBaku()`: Menghitung simpangan baku statistik dari sampel data yang tersimpan di *buffer* untuk mengukur tingkat variabilitas data.
4. `randomSeed(micros())`: Menginisialisasi pembuat angka acak berdasarkan waktu mikron mikroprosesor agar pola *noise* acak berbeda setiap kali sistem dinyalakan.
5. `sqrt(varians)`: Fungsi matematika untuk mengkalkulasi akar kuadrat dari nilai varians guna memperoleh simpangan baku.

## Penjelasan Percabangan dan Perulangan (Conditional & Loop)
1. `if (jumlah < N) jumlah++`: Percabangan yang memastikan variabel penanda `jumlah` sampel tidak melebihi batas kapasitas $N=5$ pada awal siklus pengisian *buffer*.
2. `if (jumlahBaca % 25 == 0)`: Percabangan modulo yang menyuntikkan lonjakan suhu $+8.0^\circ\text{C}$ sebagai anomali acuan setiap pembacaan kelipatan 25.
3. `if (suhu < suhuMin) suhuMin = suhu`: Percabangan logika untuk mengecek dan memperbarui nilai suhu terkecil yang pernah tercatat.
4. `if (suhu > suhuMax) suhuMax = suhu`: Percabangan logika untuk mengecek dan memperbarui nilai suhu terbesar yang pernah tercatat.
5. `for (int i = 0; i < jumlah; i++)`: Perulangan `for` yang digunakan untuk melakukan penjumlahan/akumulasi elemen sampel di dalam *buffer*.

## Konfigurasi Pin
| Komponen          | Pin NodeMCU | Pin GPIO | Modus | Keterangan                                              |
| :---------------- | :---------: | :------: | :---: | :------------------------------------------------------ |
| Sensor DHT22/11   |   Pin D2    |  GPIO4   | INPUT | Terhubung ke pin Data Sensor (Opsional Hardware)        |
| VCC Sensor        |   Pin 3V3   |    -     | POWER | Catu daya positif sensor                                |
| GND Sensor        |   Pin GND   |    -     |  GND  | Ground bersama                                          |
| Internal / Mock   |      -      |    -     |   -   | Akuisisi dijalankan internal via fungsi `bacaSuhuMock()`|

## Source Code
**Bagian 1**

![Percobaan5A](<SourcecodePercobaan-5A 1.png>)

**Bagian 2**

![Percobaan5A](<SourcecodePercobaan-5A 2.png>)

## Pertanyaan Praktikum
1. Gambarkan diagram alur (flowchart) proses perhitungan moving average pada fungsi hitungMovingAverage()! \
**Jawab:**

![Percobaan5A](<Flowchart.png>)

2. Jelaskan fungsi dari operasi (indeks + 1) % N pada program tersebut! \
**Jawab:**
Operasi (indeks + 1) % N digunakan untuk membentuk circular buffer atau penyangga melingkar. Setiap kali data baru masuk, nilai indeks bertambah satu. Dengan 𝑁 = 5, indeks dibatasi pada posisi 0 hingga 4. Ketika indeks mencapai 4, perhitungan (4 + 1) % 5 menghasilkan 0 sehingga posisi penulisan kembali ke awal array. Mekanisme ini memungkinkan data lama digantikan oleh data terbaru tanpa harus menggeser seluruh elemen array. Dengan demikian, penggunaan circular buffer dapat menghemat memori dan waktu pemrosesan pada ESP8266.

3. Apa dampaknya terhadap data hasil filter apabila nilai N diperbesar menjadi 20 atau diperkecil menjadi 2? \
**Jawab:**
● 𝑁 = 20 (Jendela Diperbesar): Hasil penyaringan menjadi lebih halus karena derau acak dirata-ratakan dari lebih banyak sampel. Namun, filter membutuhkan waktu lebih lama untuk merespons perubahan suhu. Dengan interval pembacaan 2 detik, 20 sampel mencakup sekitar 40 detik data. Selain itu, kebutuhan memori untuk menyimpan data juga meningkat.
● 𝑁 = 2 (Jendela Diperkecil): Filter lebih cepat merespons perubahan suhu, tetapi kemampuan meredam derau menjadi lebih rendah. Akibatnya, data hasil filter masih menunjukkan fluktuasi yang mendekati data mentah.
● Penggunaan ukuran jendela 𝑁 = 5 pada percobaan ini dipilih karena memberikan keseimbangan antara kehalusan data hasil filter dan kecepatan respons terhadap perubahan suhu.

4. Modifikasi program agar ESP32 juga menghitung simpangan baku dari data pada buffer, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md. \
**Jawab:**
```cpp
#include <math.h>

// Mock function pengganti sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    // Suhu dasar berubah secara bertahap
    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

    // Noise kecil untuk mensimulasikan fluktuasi sensor
    float noise = random(-5, 6) * 0.1;
    float suhu = suhuDasar + noise;

    // Lonjakan suhu setiap 25 pembacaan untuk simulasi anomali
    if (jumlahBaca % 25 == 0) {
        suhu += 8.0;
    }

    return suhu;
}

const int N = 5;
float buffer[N];
int indeks = 0;
int jumlah = 0;

float suhuMin = 1000.0;
float suhuMax = -1000.0;

float hitungMovingAverage(float nilaiBaru) {
    buffer[indeks] = nilaiBaru;
    indeks = (indeks + 1) % N;
    if (jumlah < N) {
        jumlah++;
    }

    float total = 0;
    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    return total / jumlah;
}

// TAMBAHAN: Menghitung simpangan baku data yang tersimpan di buffer
float hitungSimpanganBaku() {
    float total = 0.0;
    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    float rataRata = total / jumlah;
    float jumlahKuadratSelisih = 0.0;

    for (int i = 0; i < jumlah; i++) {
        float selisih = buffer[i] - rataRata;
        jumlahKuadratSelisih += selisih * selisih;
    }

    float varians = jumlahKuadratSelisih / jumlah;
    return sqrt(varians);
}

void setup() {
    Serial.begin(115200);
    randomSeed(micros());
}

void loop() {
    float suhu = bacaSuhuMock();

    if (isnan(suhu)) {
        Serial.println("Pembacaan gagal, data diabaikan");
    } else {
        float suhuFilter = hitungMovingAverage(suhu);

        // TAMBAHAN: Menghitung simpangan baku setelah buffer diperbarui
        float simpanganBaku = hitungSimpanganBaku();

        if (suhu < suhuMin) suhuMin = suhu;
        if (suhu > suhuMax) suhuMax = suhu;

        Serial.print("Mentah: ");
        Serial.print(suhu);
        Serial.print(" | Filter: ");
        Serial.print(suhuFilter);
        Serial.print(" | Simpangan Baku: ");
        Serial.print(simpanganBaku);
        Serial.print(" | Min: ");
        Serial.print(suhuMin);
        Serial.print(" | Max: ");
        Serial.println(suhuMax);
    }

    delay(2000);
}


