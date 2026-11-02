## Percobaan 5B Modif: Akuisisi, Analisis Statistik, dan Visualisasi Data Suhu IoT di Sisi Komputer (Server Processing)

Dokumentasi ini memuat detail pelaksanaan Percobaan 5B Modif mengenai akuisisi data telemetri suhu secara *real-time* via protokol MQTT, penyimpanan dataset historis ke file CSV, analisis statistik deskriptif, penerapan filter *Moving Average* 5 sampel (N = 5), deteksi anomali berbasis metode *Z-Score* (|z| > 2), serta visualisasi grafik komparatif menggunakan bahasa Python.

## Tujuan
1. Memahami tahapan akuisisi data IoT berbasis MQTT menggunakan skrip Python (`paho-mqtt`).
2. Mengimplementasikan penyimpanan data telemetri terstruktur beserta stempel waktu (*timestamp*) ke dalam format CSV menggunakan library `pandas`.
3. Mengolah statistik deskriptif (rata-rata, deviasi standar, minimum, dan maksimum) serta menghitung filter *Moving Average* 5 data (MA5).
4. Mendeteksi anomali data suhu menggunakan metode *Z-Score* dengan batas kritis |z| > 2 serta membandingkan performa deteksi anomali sebelum dan sesudah pemfilteran MA5.
5. Menyajikan visualisasi data interaktif dan menyimpan grafik hasil pengolahan dalam format image (`grafik_suhu.png`) menggunakan library `matplotlib`.

## Spesifikasi yang Diharapkan

1. Skrip Python berhasil terhubung ke broker MQTT (`broker.hivemq.com`) dan melakukan *subscribe* ke topik telemetri suhu `unsoed/tk245004/kelompok2425/data`.
2. Program berhasil mengumpulkan 30 sampel data suhu valid dan menyimpannya ke file `data_suhu.csv` dengan stempel waktu format `YYYY-MM-DD HH:MM:SS`.
3. Kalkulasi *Z-Score* secara otomatis menandai data mentah dan data hasil MA5 yang tergolong anomali (|z| > 2).
4. Menghasilkan output berupa log statistik terminal dan file grafik `grafik_suhu.png` yang memetakan tren suhu mentah, kurva MA5, serta titik anomali data mentah (penanda merah) dan anomali MA5 (penanda ungu `x`).

## Alat dan Bahan
1. Board ESP8266 (1 buah)
2. Sensor DHT22 (1 buah)
3. Breadboard dan Kabel Jumper (secukupnya)
4. Kabel USB (Micro-USB/USB-C sesuai board)
5. Laptop/PC dengan Arduino IDE (pustaka DHT sensor library, PubSubClient, dan ArduinoJson) dan Python 3 (pustaka paho-mqtt, pandas, dan matplotlib)
6. Jaringan WiFi yang terhubung ke internet
7. Broker MQTT publik broker.hivemq.com (port 1883)

## Rangkaian Percobaan
Pada percobaan ini, pengolahan data berfokus di sisi **server/komputer**. Komputer bertindak sebagai **MQTT Client (Subscriber)** yang mendengarkan data telemetri dari broker. Setelah 30 data sampel terkumpul, aliran akuisisi terputus dan proses pembersihan, pemfilteran statistik (**Moving Average**), penandaan anomali (**Z-Score**), serta pembuatan laporan grafik dilakukan secara otomatis menggunakan skrip **Python**.

## Gambar Rangkaian Percobaan 5A
![Percobaan 5B](<RangkaianPercobaan- 5A & 5B.png>)

## Source Code dan Penjelasan Program
### Library/Dependencies:
* `json`: Mengurai (*parsing*) payload string berformat JSON dari pesan MQTT menjadi tipe data *dictionary* Python.
* `time`: Memasukkan stempel waktu (*timestamp*) lokal pada setiap data suhu yang diterima.
* `paho.mqtt.client`: Mengelola koneksi MQTT client (koneksi ke broker, *subscribe* topik, serta callback *event* `on_connect` dan `on_message`).
* `pandas`: Mengelola data tabular dalam bentuk `DataFrame`, menghitung nilai statistik deskriptif, pemfilteran *rolling mean*, kalkulasi *Z-Score*, dan penulisan file CSV.
* `matplotlib.pyplot`: Membuat visualisasi grafik garis, *scatter plot* untuk penanda anomali, dan menyimpan gambar grafik.

### Penjelasan Logika Program:
1. **Akuisisi Data (`Tahap 1`)**: Client Paho-MQTT terhubung ke broker HiveMQ dan melakukan *subscribe* ke topik target. Setiap kali pesan masuk, fungsi `on_message` mendekode payload JSON, mengekstrak nilai suhu, dan menyimpannya ke antrean list `data` bersama *timestamp*. Jika jumlah data mencapai 30 sampel, `client.disconnect()` dipanggil.
2. **Pembentukan DataFrame & Validasi (`Tahap 2`)**: Data `list` dikonversi menjadi `pd.DataFrame`. Sistem memvalidasi `if df.empty` untuk memastikan data tidak kosong sebelum melangkah ke tahap analisis.
3. **Pemfilteran Moving Average (`Tahap 3`)**: Menggunakan fungsi `df["suhu"].rolling(window=5, min_periods=1).mean()` untuk menghitung rata-rata bergerak dengan jendela 5 data ($\text{MA5}$).
4. **Analisis Z-Score Data Mentah & MA5 (`Tahap 4 & 5`)**:
   * Menghitung rata-rata ($\bar{x}$) dan simpangan baku ($\sigma$) untuk data suhu mentah maupun data hasil $\text{MA5}$.
   * Menghitung nilai $Z$-Score: $z = \frac{x - \bar{x}}{\sigma}$.
   * Memetakan kondisi anomali jika $\vert{}z\vert{} > 2$.
5. **Penyimpanan CSV & Visualisasi Grafis (`Tahap 6 & 7`)**: Menyimpan DataFrame ke file `data_suhu.csv`, menampilkan ringkasan statistik di terminal, dan menggambarkan plot perbandingan antara suhu mentah, kurva $\text{MA5}$, serta titik-titik anomali.

### Penjelasan Fungsi Utama:
1. `json.loads(msg.payload.decode())`: Mengonversi *byte payload* MQTT menjadi objek *dictionary* Python.
2. `df.rolling(window=5, min_periods=1).mean()`: Menghitung rata-rata bergerak dari 5 sampel terakhir, dengan batas minimal 1 sampel untuk data awal.
3. `df["suhu"].mean()` & `df["suhu"].std()`: Menghitung nilai rata-rata aritmetika dan simpangan baku (deviasi standar) dari kolom suhu.
4. `df["zscore"].abs() > 2`: Operasi vektor yang mengembalikan nilai *boolean* `True` jika nilai mutlak *Z-Score* lebih besar dari 2.
5. `df.to_csv("data_suhu.csv", index=False)`: Mengekspor seluruh isi `DataFrame` (termasuk kolom waktu, suhu, MA5, Z-Score, dan status anomali) ke dalam file CSV.
6. `plt.savefig("grafik_suhu.png")`: Menyimpan tampilan canvas grafik matplotlib ke dalam file gambar PNG.

### Penjelasan Percabangan dan Perulangan (Conditional & Loop):
1. `try-except (TypeError, ValueError, KeyError)`: Blok penanganan eksepsi untuk mencegah skrip *crash* jika payload yang diterima rusak, tidak berformat JSON, atau kunci `"suhu"` tidak ditemukan.
2. `if len(data) >= JUMLAH_SAMPEL`: Percabangan untuk menghentikan pengumpulan data secara otomatis saat sampel telah mencapai batas target 30 data.
3. `if std == 0`: Percabangan untuk menghindari pembagian dengan nol (*division by zero*) saat menghitung *Z-Score* jika seluruh nilai suhu bernilai konstan.
4. `if df.empty`: Memeriksa apakah `DataFrame` kosong untuk menghentikan program dengan pesan error sebelum kalkulasi dilakukan.

## Struktur Dataset CSV (`data_suhu.csv`)

| Kolom         | Tipe Data | Keterangan                                            |
| :------------ | :-------- | :---------------------------------------------------- |
| `waktu`       | String    | Stempel waktu penerimaan data (`YYYY-MM-DD HH:MM:SS`) |
| `suhu`        | Float     | Data suhu mentah yang diterima dari MQTT (°C)         |
| `suhu_ma5`    | Float     | Nilai suhu hasil pemfilteran *Moving Average* (N = 5) |
| `zscore`      | Float     | Nilai *Z-Score* dari data suhu mentah                 |
| `anomali`     | Boolean   | Status anomali data mentah (`True` jika `\|z\| > 2`)  |
| `zscore_ma5`  | Float     | Nilai *Z-Score* dari data hasil pemfilteran MA5       |
| `anomali_ma5` | Boolean   | Status anomali data MA5 (`True` jika `\|z_MA5\| > 2`) |

## Source Code
**Bagian 1**

![Percobaan5B](<SourcecodePercobaan-5B 1.png>)

**Bagian 2**

![Percobaan5B](<SourcecodePercobaan-5B 2.png>)

**Bagian 3**
![Percobaan5B](<SourcecodePercobaan-5B 3.png>)

## Pertanyaan Praktikum
1. Jelaskan fungsi dari blok `try-except` pada fungsi `on_message()` dan apa yang terjadi apabila blok tersebut dihilangkan! \
   **Jawab:** 
   Blok try-except digunakan untuk menangani kesalahan saat memproses pesan MQTT. Fungsi json.loads() mengubah payload menjadi dictionary, kemudian mengambil nilai suhu dan mengkonversinya menjadi float. Blok except menangani kesalahan ValueError dan KeyError. Jika data tidak valid, program menampilkan pesan peringatan dan mengabaikan data tersebut. Tanpa blok ini, kesalahan pemrosesan dapat menghentikan program sehingga pengumpulan data, penyimpanan CSV, analisis, dan visualisasi terganggu. Hal ini penting karena broker publik broker.hivemq.com memungkinkan siapa saja mengirim pesan pada topik yang sama.

2. Jelaskan arti nilai z-score dan mengapa batas |z| > 2 digunakan sebagai penentu anomali! \
   **Jawab:** 
    Z-score menunjukkan jarak suatu nilai dari rata-rata dalam satuan simpangan baku, dengan rumus z=x-x. Nilai positif menunjukkan data di atas rata-rata, sedangkan nilai negatif menunjukkan data di bawah rata-rata.Batas |z|>2 digunakan karena sekitar 95% data berdistribusi normal berada dalam rentang ±2 simpangan baku. Data di luar rentang tersebut dianggap tidak biasa dan berpotensi menjadi anomali. Batas ini dipilih agar tidak terlalu banyak data normal terdeteksi sebagai anomali atau justru melewatkan penyimpangan yang terjad


3. Apa kelemahan metode *Z-Score* apabila data memiliki tren naik atau turun yang tajam? \
   **Jawab:** 
   Z-score dihitung berdasarkan rata-rata dan simpangan baku seluruh data, sehingga memiliki beberapa kelemahan ketika data mengalami tren naik atau turun: 
   - Data yang mengikuti tren wajar dapat terdeteksi sebagai anomali (false positive) karena jauh dari rata-rata keseluruhan.
   - Simpangan baku yang membesar akibat tren dapat menyebabkan anomali tidak terdeteksi (false negative).
   - Nilai ekstrem dapat mempengaruhi rata-rata dan simpangan baku sehingga mengurangi akurasi deteksi anomali.
   untuk mengatasinya, deteksi anomali dapat menggunakan rolling window atau menghilangkan tren data terlebih dahulu sebelum menghitung z-score.

4. Modifikasi program agar menggunakan metode *Moving Average* (jendela 5 data) pada data hasil CSV sebelum dihitung *Z-Score*-nya, kemudian bandingkan jumlah anomali yang terdeteksi! \
   **Jawab:** 
```cpp
import json
import time

import matplotlib.pyplot as plt
import pandas as pd
import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompok2425/data"
JUMLAH_SAMPEL = 30

data = []


def on_connect(client, userdata, flags, reason_code, properties):
    print("Terhubung ke broker, kode:", reason_code)
    client.subscribe(TOPIC)


def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        suhu = float(payload["suhu"])
    except (TypeError, ValueError, KeyError):
        print("Data tidak valid, diabaikan")
        return

    data.append({
        "waktu": time.strftime("%Y-%m-%d %H:%M:%S"),
        "suhu": suhu,
    })

    print(f"[{len(data)}/{JUMLAH_SAMPEL}] suhu = {suhu}")

    if len(data) >= JUMLAH_SAMPEL:
        client.disconnect()


# Tahap 1: Akuisisi data dari MQTT
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_forever()

# Tahap 2: Membuat DataFrame dari data hasil akuisisi
df = pd.DataFrame(data)

# TAMBAHAN: Memastikan data tersedia sebelum melakukan analisis
if df.empty:
    raise SystemExit("Tidak ada data suhu yang diterima.")

# Tahap 3: Menghitung moving average dengan jendela 5 data
df["suhu_ma5"] = df["suhu"].rolling(window=5, min_periods=1).mean()

# Tahap 4: Menghitung z-score data mentah
rata2 = df["suhu"].mean()
std = df["suhu"].std()

if std == 0:
    df["zscore"] = 0.0
else:
    df["zscore"] = (df["suhu"] - rata2) / std

df["anomali"] = df["zscore"].abs() > 2

# Tahap 5: Menghitung z-score setelah moving average MA5
rata2_ma5 = df["suhu_ma5"].mean()
std_ma5 = df["suhu_ma5"].std()

if std_ma5 == 0:
    df["zscore_ma5"] = 0.0
else:
    df["zscore_ma5"] = (df["suhu_ma5"] - rata2_ma5) / std_ma5

df["anomali_ma5"] = df["zscore_ma5"].abs() > 2

# Tahap 6: Menyimpan hasil analisis ke CSV
df.to_csv("data_suhu.csv", index=False)
print("Data dan hasil analisis tersimpan di data_suhu.csv")

print(f"Rata-rata suhu mentah : {rata2:.2f}")
print(f"Std deviasi mentah    : {std:.2f}")
print(f"Minimum suhu mentah   : {df['suhu'].min():.2f}")
print(f"Maksimum suhu mentah  : {df['suhu'].max():.2f}")

jumlah_anomali_mentah = int(df["anomali"].sum())
jumlah_anomali_ma5 = int(df["anomali_ma5"].sum())
print(f"Anomali data mentah   : {jumlah_anomali_mentah}")
print(f"Anomali setelah MA5   : {jumlah_anomali_ma5}")

# Tahap 7: Visualisasi hasil
plt.figure(figsize=(10, 4))
plt.plot(df.index, df["suhu"], marker="o", label="Suhu mentah")
plt.plot(df.index, df["suhu_ma5"], linestyle="--", label="Moving average (5)")

anom = df[df["anomali"]]
plt.scatter(
    anom.index,
    anom["suhu"],
    color="red",
    s=100,
    label="Anomali data mentah",
    zorder=3,
)

# TAMBAHAN: Menampilkan anomali pada data hasil MA5
anom_ma5 = df[df["anomali_ma5"]]
plt.scatter(
    anom_ma5.index,
    anom_ma5["suhu_ma5"],
    color="purple",
    marker="x",
    s=80,
    label="Anomali setelah MA5",
    zorder=3,
)

plt.axhline(rata2, color="gray", linestyle="--", label="Rata-rata suhu mentah")
plt.xlabel("Urutan data")
plt.ylabel("Suhu (°C)")
plt.title("Perbandingan Suhu Mentah, MA5, dan Deteksi Anomali")
plt.legend()
plt.tight_layout()
plt.savefig("grafik_suhu.png")
plt.show()
