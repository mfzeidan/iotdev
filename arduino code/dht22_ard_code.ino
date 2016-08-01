#include <DHT.h>
DHT dht(4, DHT22, 20);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("hi");
      float h = dht.readHumidity();
      float f = dht.readTemperature(true);
      Serial.println("Temp");
      Serial.println(f);
      Serial.println("humidity");
      Serial.println(h);
  delay(1000);
}
