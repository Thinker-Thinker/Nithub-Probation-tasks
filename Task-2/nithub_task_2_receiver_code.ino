#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,5);  // RX, TX (reverse of sender)

void setup() {
  Serial.begin(9600);      // USB serial for monitoring
  mySerial.begin(9600);    // Communication with sender
}

void loop() {
  if (mySerial.available()) {
   int receivedData = mySerial.parseInt();  // Read incoming data
    Serial.println("Received Temperature: " + String(receivedData));
  }
  delay(1000); // Wait for 1000 millisecond(s)
}
