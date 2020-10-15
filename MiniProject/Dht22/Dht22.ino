#include "DHT.h"
DHT dht(18, DHT22);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println();
  delay(500);

}
