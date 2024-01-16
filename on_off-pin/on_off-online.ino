#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>  // Pastikan Anda sudah menginstal library ArduinoJSON melalui Library Manager

const char *ssid = "belik";
const char *password = "putra12345";

void setup() {
  Serial.write(27);     // ESC command
  Serial.print("[2J");  // clear screen command
  Serial.write(27);
  Serial.print("[H");
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Buat objek HTTPClient
  HTTPClient http;

  // Atur URL target
  String url = "https://api-iot-testing.vercel.app/getMode";
  http.begin(url);

  // Kirim permintaan GET
  int httpResponseCode = http.GET();

  // Cetak hasil permintaan
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  // Baca dan cetak respons dari server
  String payload = http.getString();
  Serial.println("Response payload: " + payload);

  // Parse respon JSON menggunakan ArduinoJSON
  DynamicJsonDocument doc(1024);  // Sesuaikan ukuran dokumen sesuai kebutuhan
  DeserializationError error = deserializeJson(doc, payload);

  // Periksa kesalahan parsing
  if (error) {
    Serial.print("Parsing failed: ");
    Serial.println(error.c_str());
  } else {
    // Ambil nilai dari kunci "mode"
    int modeValue = doc["mode"];

    // Serial.println(modeValue);
    if (modeValue == 1) {
      // Switch ON, hidupkan lampu
      digitalWrite(2, HIGH);
    } else {
      // Switch OFF, matikan lampu
      digitalWrite(2, LOW);
    }
  }

  // Tutup koneksi
  http.end();
}
