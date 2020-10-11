#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ctime>
#include <iostream>

#define FIREBASE_HOST "smartgarden-5edd9.firebaseio.com"
#define FIREBASE_AUTH "D2itmSj9FpISf4Do8vZYMOf8BcMrUTi28aaf48QR"
#define WIFI_SSID "Room221_2.4G"
#define WIFI_PASSWORD "Jummaidai"
FirebaseData firebaseData;

String year, month, day, hour, minute = "";

void setup() {

  Initialization();
  WiFiConnection();
}

void Initialization() {
  Serial.begin(115200);
  pinMode(13, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  pinMode(34, INPUT);
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
String path = "";

void loop() {
  /*
    if (digitalRead(13) == 0) {
    digitalWrite(12, HIGH);
    pumpSwitch(true);
    }
    else if (digitalRead(13) == 1) {
    digitalWrite(12, LOW);
    pumpSwitch(false);
    }
  */

  webClick();

  if (Firebase.setTimestamp(firebaseData, "/serverTime")) {
    std::time_t result = firebaseData.intData() + 25200;
    String date = asctime(std::localtime(&result));
    date.trim();
    year = "Log/" + date.substring(20, 24);
    month = "/" + date.substring(4, 7);
    day = "/" + date.substring(8, 10);
    hour = "/" + date.substring(11, 13);
    minute = "/" + date.substring(14, 16);
    int minuteInt = minute.substring(1, 4).toInt();
    int hourInt = hour.substring(1, 4).toInt();


    if (minuteInt % 30 == 0) {
      path =  "newLog/" + getLog();
      int value = readSoilMoisture();
      if ((hourInt == 9 && minuteInt == 0) || (hourInt == 15 && minuteInt == 0) && value < 40) {    //Turn on water pump
        pumpSwitch(true);
        if (Firebase.setString(firebaseData, path + "/ControlBy", "Automation"))
          Serial.println("Turn ON water pump");
        else
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        
        if (Firebase.setString(firebaseData, path + "/date", date))
          Serial.println("SUCCESS to add DATE to database : \n" + date);
        else
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());

      }
      else if ((hourInt == 10 && minuteInt == 0) || (hourInt == 16 && minuteInt == 0)) {
        if (value < 50) {             //watering for 1 hour but soil moisture still low
          Serial.println("Something wrong with Water system please check!!!");
          if (Firebase.setString(firebaseData, path + "/ControlBy", "Automation"))
            Serial.println("watering System ERROR!!");
          pumpSwitch(false);
        }
        else {
          pumpSwitch(false);          //watering for 1 hour
          if (Firebase.setString(firebaseData, path + "/ControlBy", "Automation")) {
            Serial.println("Turn OFF water pump");
          }
          else
            Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());

          if (Firebase.setString(firebaseData, path + "/date", date))
            Serial.println("SUCCESS to add DATE to database : \n" + date);
          else
            Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
        addPumpStatus();
        updateLog();
      }
      else {        //keep log to database without pump control
        if (Firebase.setString(firebaseData, path + "/date", date)) {
          Serial.println("SUCCESS to add DATE to database : \n" + date);
        }
        else {
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
      }
      addPumpStatus();
      updateLog();
    }
  }
}

String getLog(){
  if (Firebase.getInt(firebaseData, "/LogKeep/logCount")) {
    int logCount = firebaseData.intData() + 1;
    String logPath = String(logCount);
  }
}

void addPumpStatus() {
  if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
    boolean pStatus = firebaseData.boolData();
    if (Firebase.setFloat(firebaseData, year + month + day + hour + minute + "/pumpStatus", pStatus)) {
      Serial.print("SUCCESS to add statusLog to database : ");
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

void webClick() {
  if (Firebase.getBool(firebaseData, "/webClick/clicked/webClick")) {
    boolean clickStatus = firebaseData.boolData();
    if (clickStatus) {
      addDate();
      addPumpStatus();
      readSoilMoisture();
      if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
        boolean pStatus = firebaseData.boolData();
        pumpSwitch(pStatus);
        if (Firebase.setString(firebaseData, year + month + day + hour + minute + "/ControlBy", "Web Click")) {
          Serial.println("Web click aleart!!");
        }
        if (Firebase.setBool(firebaseData, "/webClick/clicked/webClick", !clickStatus)) {
          updateLog();
        }
        else {
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
      }
      else {
        Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
      }
    }
  }
  else {
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }
}

void updateLog() {
  if (Firebase.setBool(firebaseData, year + month + day + hour + minute + "/updateFinish", true)) {}
  else
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
}

void addDate() {
  if (Firebase.setTimestamp(firebaseData, "/serverTime")) {
    std::time_t result = firebaseData.intData() + 25200;
    String date = asctime(std::localtime(&result));
    date.trim();
    year = "Log/" + date.substring(20, 24);
    month = "/" + date.substring(4, 7);
    day = "/" + date.substring(8, 10);
    hour = "/" + date.substring(11, 13);
    minute = "/" + date.substring(14, 16);

    if (Firebase.setString(firebaseData, year + month + day + hour + minute + "/date", date))
      Serial.println("SUCCESS to add DATE to database : \n" + date);
    else
      Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }
}

void pumpSwitch(boolean con) {
  if (Firebase.setBool(firebaseData, "/waterControl/pump/pumpStatus", con)) {
    Serial.print("SUCCESS to manage water pump : ");
    if (con)
      Serial.println("ON");
    else
      Serial.println("OFF");
  }
  else {   //else of set status
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }
}

int readSoilMoisture() {
  int value = analogRead(34);
  Serial.print("ADC Value : ");
  Serial.println(value);
  value = map(value, 4095, 0, 0, 100);
  Serial.print("Percent Value : ");
  Serial.println(value);

  if (Firebase.setFloat(firebaseData, path + "/SoilMoisture", value)) {
    Serial.print("SUCCESS to add Soil Moisture to database : ");
    Serial.println(value);
  }
  else {
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }

  return value;
}
