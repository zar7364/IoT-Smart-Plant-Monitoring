# IoT Smart Plant Monitoring

Sistem Internet of Things (IoT) untuk memantau kondisi tanaman secara real-time dan melakukan penyiraman otomatis menggunakan ESP32.

<img width="1672" height="941" alt="ChatGPT Image May 31, 2026, 03_39_19 PM" src="https://github.com/user-attachments/assets/2c6f11fb-e664-4b20-9d2e-ad0db8dda62e" />

#Video Implementation
https://youtube.com/shorts/6cHyz2kbB1E?feature=share


## Deskripsi

Proyek ini mengintegrasikan berbagai sensor lingkungan untuk memantau kondisi tanaman secara otomatis. Data dikumpulkan oleh ESP32, diproses secara lokal, kemudian dikirim ke dashboard untuk monitoring dan analisis.

Sistem juga mampu melakukan tindakan otomatis seperti mengaktifkan pompa air ketika kelembaban tanah berada di bawah ambang batas yang ditentukan.

## Fitur Utama

- Monitoring suhu dan kelembaban udara
- Monitoring kelembaban tanah
- Monitoring intensitas cahaya
- Penyiraman otomatis berbasis kondisi tanah
- Indikator status menggunakan traffic light
- Alarm menggunakan buzzer
- Dashboard monitoring real-time
- Penyimpanan data historis

## Perangkat yang Digunakan

### Hardware

- ESP32-S3 Development Board
- DHT22 Temperature & Humidity Sensor
- Capacitive Soil Moisture Sensor v2.0
- TEMT6000 Ambient Light Sensor
- Mini Water Pump 5V
- Active Buzzer 3V
- Traffic Light Module 5V
- Breadboard
- Jumper Wire
- Power Bank

### Software

- Arduino IDE
- ESP32 Board Package
- MQTT / HTTP Protocol
- Dashboard Monitoring Platform

## Arsitektur Sistem

```text
DHT22
Soil Moisture
TEMT6000
     │
     ▼
   ESP32
     │
     ▼
    WiFi
     │
     ▼
 Dashboard
     │
     ▼
 Automation
(Pump, Buzzer, LED)
