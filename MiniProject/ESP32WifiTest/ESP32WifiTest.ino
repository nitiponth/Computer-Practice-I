#include <WiFi.h>
#include <FirebaseESP32.h>
#include "time.h"
#include <ctime>
#include <iostream>

#define FIREBASE_HOST "esp32test-9f958.firebaseio.com"
#define FIREBASE_AUTH "DzD4zyUm7S8nk6E64zcsjpSObEPWYVadsG0Imw5L"
#define WIFI_SSID "Room221_2.4G"
#define WIFI_PASSWORD "Jummaidai"
FirebaseData firebaseData;

void setup(){
  
  Initialization();
  WiFiConnection();
  //if(Firebase.setInt(firebaseData,"/logCount",1));
 }

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


int logCount = -1;
int counter = 1;

void loop(){
  if (Firebase.setTimestamp(firebaseData,"/serverTime")){
      std::time_t result = firebaseData.intData() + 25200;
      String date = asctime(std::localtime(&result));
      date.trim();
      String year = date.substring(20,24);
      String month = "/" + date.substring(4,7);
      String day = "/" + date.substring(8,10);
      String hour = "/" + date.substring(11,13);
      String min = "/" + date.substring(14,16);
      int minInt = min.substring(1,4).toInt();
      //Serial.println(min);
      //Serial.println(minInt);
      //Serial.println(date);
      if(minInt%10 == 0){
        if (Firebase.setString(firebaseData,year + month + day + hour + min + "/date",date)){
          Serial.println("SUCCESS to add DATE to database : \n" + date);
        }
        else{
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
      }
  }

  delay(10000);
  /****
  if(Firebase.getInt(firebaseData,"/logCount")){
      Serial.print("log folder number :");
      logCount = firebaseData.intData();
      Serial.println(firebaseData.intData());
      //Firebase.setInt(firebaseData,"/logCount",logCount++);
    }
  /* dB path setting here 
  String path = "logNo" + String(logCount);
  
  if(Firebase.setInt(firebaseData, path + "/Count",counter++)){
      //Serial.println("Count value is Set ");
    }
  if (Firebase.setTimestamp(firebaseData,path + "/Timestamp")){
      std::time_t result = firebaseData.intData() + 25200;
      String date = asctime(std::localtime(&result));
      date.trim();
      //date = date.substring(4,23);
      //Serial.println(date);
      if (Firebase.setString(firebaseData,path + "/date",date)){
        //Serial.println("SUCCESS to add DATE to database");
      }
      else{
        Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
      }
  }

  Serial.println(" ========= Result ========= ");
  
  
  if(Firebase.getInt(firebaseData,"/logCount")){
    Serial.print("Count value in LogNo");
    Serial.print(firebaseData.intData());
    Serial.print(" : ");
  }
  if(Firebase.getInt(firebaseData,path + "/Count")){
    Serial.println(firebaseData.intData());
  }
  
  if(Firebase.getString(firebaseData,path +"/date")){
      Serial.print("time date is : ");
      Serial.println(firebaseData.stringData());
  }

  if(Firebase.setInt(firebaseData,"/logCount",++logCount));

  Serial.println("\n\n ==================================================== \n\n");
  */
}

void printLocalTime(){
    Serial.print("TIMESTAMP (Seconds): ");
    Serial.println(firebaseData.intData());
    std::time_t result = firebaseData.intData() + 25200;
    std::cout << std::asctime(std::localtime(&result));
}
