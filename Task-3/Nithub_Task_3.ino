#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// MQTT broker settings (use test broker or your cloud)
const char* mqtt_server = "broker.hivemq.com";  
const char* topic = "home/sensor";

// DHT sensor setup
#define DHTPIN 4       // GPIO for DHT22 data
#define DHTTYPE DHT22  // Change to DHT11 if needed
DHT dht(DHTPIN, DHTTYPE);

// Initialize Wi-Fi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Wi-Fi connected");

  // Connect to MQTT broker
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void loop() {
  // Read sensor values
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Check if reading is valid
  if (!isnan(temp) && !isnan(hum)) {
    String payload = "{ \"temperature\": " + String(temp) + ", \"humidity\": " + String(hum) + " }";
    client.publish(topic, payload.c_str());
    Serial.println("Published: " + payload);
  } else {
    Serial.println("Failed to read from DHT sensor");
  }

  client.loop();
  delay(5000);  // Send data every 5 seconds
}
