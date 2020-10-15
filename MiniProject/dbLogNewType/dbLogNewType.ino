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

String path = "";
int startHour, stopHour, startMin, stopMin, value;

void loop() {

  if (digitalRead(13) == 0) {
    while (digitalRead(13) == 0) {
      Serial.print(".");
    }
    Serial.println("\n\n\n\n");
    //digitalWrite(12, HIGH); //LED Status
    manualSwitch();
  }

  webClick();

  if (Firebase.setTimestamp(firebaseData, "/serverTime")) {
    std::time_t result = firebaseData.intData() + 25200;
    String date = asctime(std::localtime(&result));
    date.trim();
    int hourInt = date.substring(11, 13).toInt();
    int minuteInt = date.substring(14, 16).toInt();
    getInitialTimeSet();
    getFinalTimeSet();
    if ((hourInt == startHour) && (minuteInt == startMin)) {    //Turn on water pump  ======= > && (minuteInt == 15)
      Serial.println("This line in AutoWatering");
      path =  "newLog/" + getLog();
      value = readSoilMoisture();
      if (value < 40) {
        pumpSwitch(true);
        if (Firebase.setString(firebaseData, path + "/ControlBy", "Automation"))
          Serial.println("Turn ON water pump");
        if (Firebase.setString(firebaseData, path + "/date", date))
          Serial.println("SUCCESS to add DATE to database : \n" + date);
        addPumpStatus();
        updateLog();
        delay(45000);
      }
      else {
        if (Firebase.setString(firebaseData, path + "/date", date))
          Serial.println("SUCCESS to add DATE to database : \n" + date);
        addPumpStatus();
        updateLog();
        delay(40000);
      }
    }
    else if ((hourInt == stopHour) && (minuteInt == stopMin)) {
      path =  "newLog/" + getLog();
      value = readSoilMoisture();
      if (value < 45) {             //watering for 1 hour but soil moisture still low
        Serial.println("Something wrong with Water system please check!!! \n");
        if (Firebase.setString(firebaseData, path + "/ControlBy", "Automation"))
          Serial.println("watering System ERROR!!");
        Firebase.setString(firebaseData, path + "/date", date);
        pumpSwitch(false);
      }
      else {
        pumpSwitch(false);          //watering for 1 hour
        if (Firebase.setString(firebaseData, path + "/ControlBy", "Automation")) {
          Serial.println("Turn OFF water pump");
        }
        if (Firebase.setString(firebaseData, path + "/date", date))
          Serial.println(date);
      }
      addPumpStatus();
      updateLog();
      delay(45000);
    }
    else if (minuteInt % 30 == 0) {       //keep log to database without pump control
      path =  "newLog/" + getLog();
      int value = readSoilMoisture();
      addPumpStatus();

      if (Firebase.setString(firebaseData, path + "/date", date)) {
        Serial.println(date);
        updateLog();
        delay(50000);
      }
    }
  }
  delay(500);
  Serial.println(".");
}

String getLog() {
  int logCount;
  String logPath;
  if (Firebase.getInt(firebaseData, "/LogKeep/logCount")) {
    logCount = firebaseData.intData() + 1;
    logPath = String(logCount);
  }
  if (Firebase.setInt(firebaseData, "/LogKeep/logCount", logCount));
  if (Firebase.setInt(firebaseData, "/newLog/" + logPath + "/id", logCount));
  
  return logPath;
}

void addPumpStatus() {
  if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
    boolean pStatus = firebaseData.boolData();
    if (Firebase.setFloat(firebaseData, path + "/pumpStatus", pStatus)) {
      Serial.print("Waterpump status : ");
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
      path =  "newLog/" + getLog();
      Serial.println("\n\n" + path + "\n\n");
      addDate();
      addPumpStatus();
      readSoilMoisture();
      if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
        boolean pStatus = firebaseData.boolData();
        pumpSwitch(pStatus);
        if (Firebase.setString(firebaseData, path + "/ControlBy", "Web Click")) {
          Serial.println("Web click aleart!! \n\n");
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

void manualSwitch() {
  path =  "newLog/" + getLog();

  if (Firebase.getBool(firebaseData, "/waterControl/pump/pumpStatus")) {
    boolean pStatus = firebaseData.boolData();
    pumpSwitch(!pStatus);
    if (Firebase.setString(firebaseData, path + "/ControlBy", "ManualSwitch")) {
      addPumpStatus();
      readSoilMoisture();
      addDate();
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

void updateLog() {
  if (Firebase.setBool(firebaseData, path + "/updateFinish", true)) {
    Serial.println("\n\n");
  }
  else
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
}

void addDate() {
  if (Firebase.setTimestamp(firebaseData, "/serverTime")) {
    std::time_t result = firebaseData.intData() + 25200;
    String date = asctime(std::localtime(&result));
    date.trim();
    if (Firebase.setString(firebaseData, path + "/date", date))
      Serial.println(date);
    else
      Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }
}

void pumpSwitch(boolean con) {
  if (Firebase.setBool(firebaseData, "/waterControl/pump/pumpStatus", con)) {
    Serial.print("Manage waterpump to : ");
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
  //Serial.print("ADC Value : ");
  //Serial.println(value);
  value = map(value, 4095, 0, 0, 100);
  Serial.print("Percent Value : ");
  Serial.println(value);

  if (Firebase.setFloat(firebaseData, path + "/SoilMoisture", value));
  else {
    Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
  }

  return value;
}

void getInitialTimeSet() {

  if (Firebase.getInt(firebaseData,  "timeControl/time/initialHour")) {
    startHour = firebaseData.intData();
  }
  if (Firebase.getInt(firebaseData,  "timeControl/time/initialMin")) {
    startMin = firebaseData.intData();
  }
  //Serial.print("Start time : "); Serial.print(startHour);
  //Serial.print(" : "); Serial.println(startMin);
}

void getFinalTimeSet() {
  if (Firebase.getInt(firebaseData,  "timeControl/time/finalHour")) {
    stopHour = firebaseData.intData();
  }
  if (Firebase.getInt(firebaseData,  "timeControl/time/finalMin")) {
    stopMin = firebaseData.intData();
  }
  //Serial.print("End time : "); Serial.print(stopHour);
  //Serial.print(" : "); Serial.println(stopMin);

}
