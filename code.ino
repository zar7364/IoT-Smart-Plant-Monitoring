#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_PIN 7

// Kalibrasi sensor
#define SOIL_DRY 3350
#define SOIL_WET 1675

// ADDED TRAFFIC LIGHT - Pin definitions
#define LED_RED    38
#define LED_YELLOW 39
#define LED_GREEN  40

// ADDED BUZZER - Pin definition
#define BUZZER_PIN 41

// Konfigurasi Wi-Fi & Kredensial Supabase (DIPINDAHKAN KE ATAS)
const char* ssid        = "NEZAR";
const char* password    = "inggit.salsabila";
const char* supabaseUrl = "https://mfygcfwdmoigkhgacrdu.supabase.co";
const char* supabaseKey = "sb_publishable_AVN6xgQJ65xWddWG0vquzQ_Y7hlHkQZ";

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

float tempHistory[20];
float humHistory[20];
float soilHistory[20];

unsigned long lastSend = 0;

// ADDED BUZZER - Definisi nada lengkap Mario Bros
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST     0

// ADDED BUZZER - FS5 alias HARUS di atas array melody
#define FS5 NOTE_FS5

// ADDED BUZZER - Mario Bros Full Theme (tempo 200 BPM)
int marioTempo = 200;

int marioMelody[] = {
  // Intro
  NOTE_E5, NOTE_E5, REST,    NOTE_E5, REST,    NOTE_C5, NOTE_E5, REST,
  NOTE_G5, REST,    REST,    REST,    NOTE_G4,  REST,    REST,    REST,

  // Section A
  NOTE_C5, REST,    REST,    NOTE_G4, REST,    REST,    NOTE_E4,  REST,
  REST,    NOTE_A4, REST,    NOTE_B4, REST,    NOTE_AS4, NOTE_A4, REST,
  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, REST,   NOTE_F5,  NOTE_G5, REST,
  NOTE_E5, REST,    NOTE_C5, NOTE_D5, NOTE_B4, REST,    REST,    REST,

  // Section A repeat
  NOTE_C5, REST,    REST,    NOTE_G4, REST,    REST,    NOTE_E4,  REST,
  REST,    NOTE_A4, REST,    NOTE_B4, REST,    NOTE_AS4, NOTE_A4, REST,
  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, REST,   NOTE_F5,  NOTE_G5, REST,
  NOTE_E5, REST,    NOTE_C5, NOTE_D5, NOTE_B4, REST,    REST,    REST,

  // Section B
  REST,    NOTE_G5, FS5,     NOTE_F5, NOTE_DS5, REST,   NOTE_E5, REST,
  NOTE_GS4,NOTE_A4, NOTE_C5, REST,   NOTE_A4,  NOTE_C5, NOTE_D5, REST,
  REST,    NOTE_G5, FS5,     NOTE_F5, NOTE_DS5, REST,   NOTE_E5, REST,
  NOTE_C6, REST,    NOTE_C6, NOTE_C6, REST,    REST,    REST,    REST,

  REST,    NOTE_G5, FS5,     NOTE_F5, NOTE_DS5, REST,   NOTE_E5, REST,
  NOTE_GS4,NOTE_A4, NOTE_C5, REST,   NOTE_A4,  NOTE_C5, NOTE_D5, REST,
  REST,    NOTE_DS5,REST,    REST,    NOTE_D5,  REST,   REST,    NOTE_C5,
  REST,    REST,    REST,    REST,    REST,     REST,   REST,    REST,

  // Section A again
  NOTE_C5, REST,    REST,    NOTE_G4, REST,    REST,    NOTE_E4,  REST,
  REST,    NOTE_A4, REST,    NOTE_B4, REST,    NOTE_AS4, NOTE_A4, REST,
  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, REST,   NOTE_F5,  NOTE_G5, REST,
  NOTE_E5, REST,    NOTE_C5, NOTE_D5, NOTE_B4, REST,    REST,    REST,

  NOTE_C5, REST,    REST,    NOTE_G4, REST,    REST,    NOTE_E4,  REST,
  REST,    NOTE_A4, REST,    NOTE_B4, REST,    NOTE_AS4, NOTE_A4, REST,
  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, REST,   NOTE_F5,  NOTE_G5, REST,
  NOTE_E5, REST,    NOTE_C5, NOTE_D5, NOTE_B4, REST,    REST,    REST,

  // Outro
  NOTE_E5, NOTE_C5, REST,    NOTE_G4, REST,    NOTE_GS4, REST,
  NOTE_A4, NOTE_F5, REST,    NOTE_F5, NOTE_A4, REST,    REST,
  NOTE_B4, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, REST,
  NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4, REST,    REST,    REST,    REST
};

int marioDurations[] = {
  // Intro
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 4, 4, 4, 4, 4, 4,
  // Section A
  4, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 8, 8, 8,
  4, 8, 8, 8, 4, 4, 4, 4,
  // Section A repeat
  4, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 8, 8, 8,
  4, 8, 8, 8, 4, 4, 4, 4,
  // Section B
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 4, 4, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 4, 4, 4, 4, 4, 4,
  // Section A again
  4, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 8, 8, 8,
  4, 8, 8, 8, 4, 4, 4, 4,
  4, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 8, 8, 8,
  4, 8, 8, 8, 4, 4, 4, 4,
  // Outro
  8, 8, 8, 4, 8, 8, 8,
  8, 8, 8, 8, 4, 4, 4,
  8, 8, 8, 8, 4, 8,
  8, 8, 8, 4, 4, 4, 4, 4
};

int marioLength = sizeof(marioMelody) / sizeof(marioMelody[0]);

// ADDED BUZZER - State machine untuk melody & logika 3x + 12 jam
enum BuzzerState {
  BUZZER_IDLE,
  BUZZER_PLAYING,
  BUZZER_WAITING,
  BUZZER_COOLDOWN
};

BuzzerState   buzzerState     = BUZZER_IDLE;
int           playCount       = 0;
int           melodyNoteIndex = 0;
unsigned long noteStartTime   = 0;
unsigned long waitStartTime   = 0;
unsigned long cooldownStart   = 0;

#define BUZZER_JEDA_MS     15000UL
#define BUZZER_COOLDOWN_MS 43200000UL
#define BUZZER_MAX_PLAY    3

// ADDED BUZZER - Non-blocking melody player, return true jika selesai
bool playMarioNonBlocking() {
  if (melodyNoteIndex >= marioLength) {
    noTone(BUZZER_PIN);
    return true;
  }

  int wholenote    = (60000 * 4) / marioTempo;
  int noteDuration = wholenote / marioDurations[melodyNoteIndex];
  unsigned long now = millis();

  // Mulai note pertama
  if (noteStartTime == 0) {
    noteStartTime = now;
    if (marioMelody[melodyNoteIndex] != REST) {
      tone(BUZZER_PIN, marioMelody[melodyNoteIndex], noteDuration * 0.9);
    }
  }

  // Cek apakah durasi note sudah habis
  if (now - noteStartTime >= (unsigned long)noteDuration) {
    noTone(BUZZER_PIN);
    melodyNoteIndex++;
    noteStartTime = 0;

    if (melodyNoteIndex < marioLength) {
      int wholenote2    = (60000 * 4) / marioTempo;
      int nextDuration  = wholenote2 / marioDurations[melodyNoteIndex];
      noteStartTime      = millis();
      if (marioMelody[melodyNoteIndex] != REST) {
        tone(BUZZER_PIN, marioMelody[melodyNoteIndex], nextDuration * 0.9);
      }
    }
  }

  return false;
}

// ADDED BUZZER - Reset posisi melodi
void resetMelody() {
  melodyNoteIndex = 0;
  noteStartTime   = 0;
  noTone(BUZZER_PIN);
}

// ADDED BUZZER - Update logika buzzer (dipanggil tiap loop)
void updateBuzzer(float soilMoisture) {
  if (soilMoisture >= 30.0) {
    noTone(BUZZER_PIN);
    buzzerState = BUZZER_IDLE;
    playCount   = 0;
    resetMelody();
    return;
  }

  unsigned long now = millis();

  switch (buzzerState) {
    case BUZZER_IDLE:
      playCount   = 0;
      buzzerState = BUZZER_PLAYING;
      resetMelody();
      Serial.println("Buzzer: mulai play Mario ke-1...");
      break;

    case BUZZER_PLAYING: {
      bool selesai = playMarioNonBlocking();
      if (selesai) {
        noTone(BUZZER_PIN);
        playCount++;
        resetMelody();
        if (playCount >= BUZZER_MAX_PLAY) {
          buzzerState   = BUZZER_COOLDOWN;
          cooldownStart = now;
          Serial.println("Buzzer: 3x selesai, cooldown 12 jam dimulai.");
        } else {
          buzzerState   = BUZZER_WAITING;
          waitStartTime = now;
          Serial.print("Buzzer: play ke-");
          Serial.print(playCount);
          Serial.println(" selesai, jeda 15 detik...");
        }
      }
      break;
    }

    case BUZZER_WAITING:
      if (now - waitStartTime >= BUZZER_JEDA_MS) {
        buzzerState = BUZZER_PLAYING;
        resetMelody();
        Serial.print("Buzzer: mulai play Mario ke-");
        Serial.print(playCount + 1);
        Serial.println("...");
      }
      break;

    case BUZZER_COOLDOWN:
      if (now - cooldownStart >= BUZZER_COOLDOWN_MS) {
        buzzerState = BUZZER_IDLE;
        playCount   = 0;
        Serial.println("Buzzer: cooldown 12 jam selesai, siap bunyi lagi.");
      }
      break;
  }
}

// ============================================================
// Fungsi-fungsi sensor & utilitas
// ============================================================

void shiftArray(float arr[], float value) {
  for (int i = 0; i < 19; i++) arr[i] = arr[i + 1];
  arr[19] = value;
}

String getStatus(float temp) {
  if (temp < 25) return "Dingin";
  if (temp < 32) return "Normal";
  return "Panas";
}

String getSoilStatus(float moisture) {
  if (moisture < 30) return "Kering";
  if (moisture < 70) return "Lembap";
  return "Basah";
}

float readSoilMoisture() {
  int raw = analogRead(SOIL_PIN);
  float percent = map(raw, SOIL_DRY, SOIL_WET, 0, 100);
  percent = constrain(percent, 0, 100);
  return percent;
}

// ADDED TRAFFIC LIGHT - Kontrol 3 LED sekaligus
void setTrafficLight(bool red, bool yellow, bool green) {
  digitalWrite(LED_RED,    red    ? HIGH : LOW);
  digitalWrite(LED_YELLOW, yellow ? HIGH : LOW);
  digitalWrite(LED_GREEN,  green  ? HIGH : LOW);
}

// ADDED TRAFFIC LIGHT - Update LED berdasarkan soil moisture
void updateTrafficLight(float soilMoisture) {
  if (soilMoisture < 30.0) {
    setTrafficLight(true, false, false);
  } else if (soilMoisture <= 70.0) {
    setTrafficLight(false, true, false);
  } else {
    setTrafficLight(false, false, true);
  }
}

void sendToSupabase(float suhu, float hum, float soil) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(supabaseUrl) + "/rest/v1/sensor_data";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", supabaseKey);
    http.addHeader("Authorization", "Bearer " + String(supabaseKey));
    http.addHeader("Prefer", "return=minimal");

    String jsonData = "[{";
    jsonData += "\"temperature\":" + String(suhu) + ",";
    jsonData += "\"humidity\":" + String(hum) + ",";
    jsonData += "\"soil_moisture\":" + String(soil, 2);
    jsonData += "}]";

    int httpResponseCode = http.POST(jsonData);
    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);
    Serial.println(http.getString());
    http.end();
  }
}

// ============================================================
// Web Dashboard
// ============================================================

void handleRoot() {
  float suhu = dht.readTemperature();
  float hum  = dht.readHumidity();
  float soil = readSoilMoisture();

  shiftArray(tempHistory, suhu);
  shiftArray(humHistory, hum);
  shiftArray(soilHistory, soil);

  String tempData = "", humData = "", soilData = "";
  for (int i = 0; i < 20; i++) {
    tempData += String(tempHistory[i]);
    humData  += String(humHistory[i]);
    soilData += String(soilHistory[i]);
    if (i < 19) { tempData += ","; humData += ","; soilData += ","; }
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<style>
body{background:#0f172a;color:white;font-family:Arial;margin:0;padding:20px;}
.title{text-align:center;font-size:32px;margin-bottom:20px;font-weight:bold;}
.grid{display:grid;grid-template-columns:1fr 1fr 1fr;gap:20px;}
.card{background:#1e293b;border-radius:20px;padding:20px;box-shadow:0 0 20px rgba(0,0,0,0.3);}
.value{font-size:45px;font-weight:bold;}
.label{color:#94a3b8;margin-bottom:10px;}
.status{margin-top:10px;font-size:18px;color:#38bdf8;}
.status-soil{margin-top:10px;font-size:18px;color:#a3e635;}
.chart-container{margin-top:20px;background:#1e293b;padding:20px;border-radius:20px;}
.footer{text-align:center;margin-top:20px;color:#64748b;}
@media(max-width:768px){.grid{grid-template-columns:1fr;}}
</style>
</head>
<body>
<div class="title">ESP32 Smart Environment Dashboard</div>
<div class="grid">
<div class="card">
<div class="label">Suhu</div>
<div class="value">)rawliteral";
  html += String(suhu);
  html += R"rawliteral(°C</div>
<div class="status">Status: )rawliteral";
  html += getStatus(suhu);
  html += R"rawliteral(</div>
</div>
<div class="card">
<div class="label">Kelembapan Udara</div>
<div class="value">)rawliteral";
  html += String(hum);
  html += R"rawliteral('%</div>
<div class="status">Realtime Monitoring</div>
</div>
<div class="card">
<div class="label">Kelembapan Tanah</div>
<div class="value">)rawliteral";
  html += String(soil, 1);
  html += R"rawliteral('%</div>
<div class="status-soil">Status: )rawliteral";
  html += getSoilStatus(soil);
  html += R"rawliteral(</div>
</div>
</div>
<div class="chart-container">
<canvas id="sensorChart"></canvas>
</div>
<div class="footer">ESP32 + Supabase IoT System</div>
<script>
const tempData = [)rawliteral";
  html += tempData;
  html += R"rawliteral(];
const humData = [)rawliteral";
  html += humData;
  html += R"rawliteral(];
const soilData = [)rawliteral";
  html += soilData;
  html += R"rawliteral(];
const ctx = document.getElementById('sensorChart');
new Chart(ctx, {
    type: 'line',
    data: {
        labels: [...Array(20).keys()],
        datasets: [
        { label: 'Suhu (°C)',            data: tempData, borderColor: '#38bdf8', tension: 0.4 },
        { label: 'Kelembapan Udara (%)', data: humData,  borderColor: '#22c55e', tension: 0.4 },
        { label: 'Kelembapan Tanah (%)', data: soilData, borderColor: '#a3e635', tension: 0.4 }
        ]
    },
    options: {
        responsive: true,
        plugins:{ legend:{ labels:{ color:'white' } } },
        scales:{ x:{ ticks:{ color:'white' } }, y:{ ticks:{ color:'white' } } }
    }
});
setTimeout(()=>{ location.reload(); }, 2000);
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// ============================================================
// Setup & Loop
// ============================================================

void setup() {
  Serial.begin(115200);
  dht.begin();

  // ADDED TRAFFIC LIGHT - Initialize LED pins
  pinMode(LED_RED,    OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN,  OUTPUT);

  // ADDED BUZZER - Initialize buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);

  // ADDED TRAFFIC LIGHT + ADDED BUZZER - Semua OFF saat startup
  setTrafficLight(false, false, false);
  noTone(BUZZER_PIN);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  // Baca soil setiap 500ms
  static unsigned long lastSoilRead = 0;
  static float currentSoil = 50.0;

  if (millis() - lastSoilRead > 500) {
    currentSoil = readSoilMoisture();

    Serial.print("RAW ADC Soil: ");
    Serial.print(analogRead(SOIL_PIN));
    Serial.print(" | Soil: ");
    Serial.print(currentSoil);
    Serial.print("% | Status: ");
    Serial.print(getSoilStatus(currentSoil));
    Serial.print(" | BuzzerState: ");
    Serial.print(buzzerState);
    Serial.print(" | PlayCount: ");
    Serial.println(playCount);

    lastSoilRead = millis();

    // ADDED TRAFFIC LIGHT - Update LED
    updateTrafficLight(currentSoil);
  }

  // ADDED BUZZER - Update melodi Mario non-blocking setiap loop
  updateBuzzer(currentSoil);

  // Kirim ke Supabase setiap 60 detik
  if (millis() - lastSend > 60000) {
    float suhu = dht.readTemperature();
    float hum  = dht.readHumidity();
    sendToSupabase(suhu, hum, currentSoil);
    lastSend = millis();
  }
}
