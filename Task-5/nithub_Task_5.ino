#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHT.h>
#include "esp_sleep.h"

// Wi-Fi Credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// NTP Client Setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);  // Sync every 60 seconds (UTC)

// DHT Sensor Setup
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Sleep Settings
#define uS_TO_S_FACTOR 1000000  // Conversion factor for microseconds to seconds
#define SLEEP_DURATION 300      // Sleep for 5 minutes when active (300 seconds)

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Wi-Fi connected.");

  // Initialize NTP
  timeClient.begin();
  timeClient.update();

  // Get current time (24-hour format)
  int currentHour = timeClient.getHours();
  Serial.println("Current Hour: " + String(currentHour));

  // Sleep between 11 PM and 5 AM
  if (currentHour >= 23 || currentHour < 5) {
    Serial.println("Nighttime: Entering deep sleep...");
    esp_sleep_enable_timer_wakeup(6 * 60 * 60 * uS_TO_S_FACTOR);  // Sleep for 6 hours
    esp_deep_sleep_start();
  } else {
    Serial.println("Daytime: Collecting sensor data...");

    // Read Sensor
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      Serial.println("Temperature: " + String(temp) + " °C");
      Serial.println("Humidity: " + String(hum) + " %");
    } else {
      Serial.println("Failed to read from DHT sensor.");
    }

    // Sleep for shorter duration during the day
    Serial.println("Sleeping for 5 minutes...");
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
  }
}

void loop() {
  // Nothing here since ESP32 will sleep after setup()
}
