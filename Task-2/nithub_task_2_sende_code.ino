// C++ code
//
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,5);  
// RX, TX pins (use any GPIOs except 0, 1)
int celsius = 0;
void setup()
{
  pinMode(A0, INPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
 
}

void loop()
{
  celsius = map(((analogRead(A0) - 20) * 3.03), 0, 1023, -40, 125);
  // Maximum value from TMP36 sensor is 358 and minimum is 20 
  // subtract 20 to reduce minimum to zero
  // maximum becomes 338, 1024/338 = 3.026
  // map() maps minimum value of 0 to -40 and max value of 1023 to 125
  mySerial.println(celsius);
  Serial.println(celsius);
  
  
  delay(1000); // Wait for 1000 millisecond(s)
}