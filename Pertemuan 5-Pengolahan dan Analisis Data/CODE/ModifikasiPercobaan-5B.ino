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