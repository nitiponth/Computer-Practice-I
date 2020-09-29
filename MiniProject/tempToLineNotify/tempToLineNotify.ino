#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ctime>
#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiClientSecure.h>

#define FIREBASE_HOST "esp32test-9f958.firebaseio.com"
#define FIREBASE_AUTH "DzD4zyUm7S8nk6E64zcsjpSObEPWYVadsG0Imw5L"
#define WIFI_SSID "Room221_2.4G"
#define WIFI_PASSWORD "Jummaidai"
//#define TokenLine "819rOdbODQFuzzttNinrE4x6j5uIQ39yROO2BzBJvoD" //myself
#define TokenLine "Eg4WYLE2C2ij0Tl27s9ejEyFZQbOz9OmTq8zxnkcGfe" //group
//#define TokenLine "rjNiWlHnA0LWBiqL1VCyMXf4mDUJtLFHsX5ougAMChb" //onlyMeGroup

FirebaseData firebaseData;
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


void setup(){
  
  Initialization();
  WiFiConnection();
  sensors.begin();
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
float tempC = 0.0;

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
      
      //sensors.requestTemperatures();
      //tempC = sensors.getTempCByIndex(0);
      //Serial.println(tempC);
      
      if(minInt%5 == 0){
        if (Firebase.setString(firebaseData,year + month + day + hour + min + "/date",date)){
          Serial.println("SUCCESS to add DATE to database : \n" + date);
        }
        else{
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
        sensors.requestTemperatures();
        tempC = sensors.getTempCByIndex(0);
        if(Firebase.setFloat(firebaseData,year + month + day + hour + min + "/temperature",tempC)){
          Serial.println("\n\nSUCCESS to add TEMP to database : ");
          Serial.println(tempC);

          if(minInt%30 == 0){
            String lineTxt = "น้องไข่หีขอแจ้งว่า อุณหภูมิ ณ ขณะนี้คือ ";
            lineTxt.concat(tempC);
            lineTxt.concat(" องศาเซลเซียสจ้า");
          
            NotifyLine(lineTxt);
          }
          delay(60000);
        }
        else{
          Serial.println("FAIL ==> REASON: " + firebaseData.errorReason());
        }
      }
  }
  delay(10000);
  Serial.print(".");
}

void NotifyLine(String t) {
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("Connection failed");
    return;
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP32\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + t).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + t;
  //Serial.println(req);
  client.print(req);
  delay(20);
  //Serial.println("-------------");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
}
