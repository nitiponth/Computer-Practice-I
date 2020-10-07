#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ctime>
#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_HOST "esp32test-9f958.firebaseio.com"
#define FIREBASE_AUTH "DzD4zyUm7S8nk6E64zcsjpSObEPWYVadsG0Imw5L"
#define WIFI_SSID "Nitiponth"
#define WIFI_PASSWORD "JUMMAIDAIPIMLEK"
FirebaseData firebaseData;
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


void setup() {

  Initialization();
  WiFiConnection();
  sensors.begin();
}

void Initialization() {
  Serial.begin(115200);
  pinMode(13, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(13), pumpSwitch, RISING);
}

void WiFiConnection() {
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
float tempC = 0.0;

void loop() {
  
  if(digitalRead(13)==0){
    digitalWrite(12,HIGH);
    pumpSwitch(true);
  }
  else if(digitalRead(13)==1){
    digitalWrite(12,LOW);
    pumpSwitch(false);
  }
  
  if (Firebase.setTimestamp(firebaseData, "/serverTime")) {
    std::time_t result = firebaseData.intData() + 25200;
    String date = asctime(std::localtime(&result));
    date.trim();
    String year = date.substring(20, 24);
    String month = "/" + date.substring(4, 7);
    String day = "/" + date.substring(8, 10);
    String hour = "/" + date.substring(11, 13);
    String min = "/" + date.substring(14, 16);
    int minInt = min.substring(1, 4).toInt();

    //sensors.requestTemperatures();
    //tempC = sensors.getTempCByIndex(0);
    //Serial.println(tempC);

    //if (minInt % 5 == 0) {
      if (Firebase.setString(firebaseData, year + month + day + hour + min + "/date", date)) {
        Serial.println("SUCCESS to add DATE to database : \n" + date);
      }
      else {
        Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
      }
      /*sensors.requestTemperatures();
        tempC = sensors.getTempCByIndex(0);
        if(Firebase.setFloat(firebaseData,year + month + day + hour + min + "/temperature",tempC)){
        Serial.println("SUCCESS to add TEMP to database : ");
        Serial.println(tempC);
        }
        else{
        Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }*/

      if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
        boolean pStatus = firebaseData.boolData();
        if (Firebase.setFloat(firebaseData, year + month + day + hour + min + "/pumpStatus", pStatus)) {
          Serial.print("SUCCESS to add status to database : ");
          Serial.println(pStatus);
        }
        else {
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
      }
      else {
        Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
      }
    }

    //delay(60000);
  //}
}

void pumpSwitch(boolean con) {
  if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
    boolean pStatus = firebaseData.boolData();
    if (Firebase.setBool(firebaseData, "/waterControl/pump/pumpStatus", con)) {
      Serial.print("SUCCESS to add status to database : ");
      Serial.println(con);
    }
    else {   //else of set status
      Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
    }
  }
  else {    //else of get status
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }
}
