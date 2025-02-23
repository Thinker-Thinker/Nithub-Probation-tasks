// C++ code
//
const int Led = 11;
const int Photoresistor = A0;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(Photoresistor, INPUT);
  Serial.begin(9600);
}

void loop()
{
 float x = (analogRead(Photoresistor)/67.9) * 25;
  // 67.9 because the maximum value of Analog write is 679 but C++ has fuzzy maths behaviour
  
  if (x > 250) 
    digitalWrite(Led, HIGH); 
  // In case the Photoresistor goes out of range
  
  else if (x > 100 &  x <= 250) 
    analogWrite(Led , x);
  //Only switch on the light when ambient lighting 40%oes down by 2
  else
    digitalWrite(Led, LOW);
    
  delay(1000); 
  // Wait for 1000 millisecond(s)
  Serial.println(x);
}