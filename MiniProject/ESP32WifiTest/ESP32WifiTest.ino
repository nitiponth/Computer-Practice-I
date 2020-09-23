#include <WiFi.h>
#include <FirebaseESP32.h>
#include "time.h"
#include <ctime>
#include <iostream>

const long  gmtOffset_sec = 25200;

#define FIREBASE_HOST "esp32test-9f958.firebaseio.com"
#define FIREBASE_AUTH "DzD4zyUm7S8nk6E64zcsjpSObEPWYVadsG0Imw5L"
#define WIFI_SSID "Room221_2.4G"
#define WIFI_PASSWORD "Jummaidai"
FirebaseData firebaseData;

void setup(){
  
  Initialization();
  WiFiConnection();
  printLocalTime();
 }

int counter = 0;
float counter2 = 0.5;

void Initialization(){
  
  Serial.begin(115200); 
 }

void WiFiConnection(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop(){
  

  if(Firebase.setInt(firebaseData, "/Count",counter++)){
    }

   if(Firebase.getInt(firebaseData, "/Count")){
    if(firebaseData.dataType() == "int"){
      Serial.print("data = ");
      Serial.println(firebaseData.intData());
      printLocalTime();
      Serial.println("\n");
    }
  }
}

void printLocalTime(){
  
  if (Firebase.setTimestamp(firebaseData,"/Timestamp"))
  {
    //Timestamp saved in millisecond, get its seconds from intData()
    Serial.print("TIMESTAMP (Seconds): ");
    Serial.println(firebaseData.intData());
    std::time_t result = firebaseData.intData() + 25200;
    std::cout << std::asctime(std::localtime(&result));
  }
}
