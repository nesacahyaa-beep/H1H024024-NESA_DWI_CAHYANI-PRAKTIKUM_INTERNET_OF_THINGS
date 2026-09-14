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

## Konfigurasi Pin

### Konfigurasi Pin

| Komponen       | Pin NodeMCU  | Modus    | Keterangan                           |
| :------------- | :----------: | :------: | :----------------------------------- |
| Anoda LED (+)  | Pin D4/GPIO2 | `OUTPUT` | Terhubung ke kaki anoda LED (sisi +) |
| Katoda LED (-) |   Pin GND    |    -     | Terhubung ke Ground NodeMCU          |
| Resistor 220Ω  |  Antara D4   |    -     | Pembatas arus untuk LED (opsional)   |

