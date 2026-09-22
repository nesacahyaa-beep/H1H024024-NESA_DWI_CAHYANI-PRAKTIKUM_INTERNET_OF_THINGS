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



