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