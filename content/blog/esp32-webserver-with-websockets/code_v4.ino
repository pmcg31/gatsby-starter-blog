#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define POT_PIN 36
#define POT_AVG_MAX 30

int potVal = 0;

int potAvg = 5;
int potVals[POT_AVG_MAX];
int potValPtr = 0;

AsyncWebServer server(80);

void handleRoot(AsyncWebServerRequest* request) {
  request->send(200, "text/plain", "Hello from ESP32!");
}

void setup() {
  for (int i = 0; i < POT_AVG_MAX; i++) {
    potVals[i] = 0;
  }

  Serial.begin(115200);

  const char* ssid = "default";
  const char* key =  "default";
 
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File configFile = SPIFFS.open("/config.json", "r");
  DynamicJsonDocument doc(1024);
  if(!configFile){
    Serial.println("Failed to open config.json for reading");
    return;
  } else {
    DeserializationError error = deserializeJson(doc, configFile);
    if (error) {
      Serial.print("Error parsing config.json [");
      Serial.print(error.c_str());
      Serial.println("]");
    }

    ssid = doc["ssid"];
    key = doc["key"];

    configFile.close();
  }
  
  Serial.print("ssid: '");
  Serial.print(ssid);
  Serial.println("'");
  Serial.print("key: '");
  Serial.print(key);
  Serial.println("'");
  WiFi.begin(ssid, key);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up mDNS responder");
  } else {
    Serial.println("mDNS responder started: esp32.local");
  }

  server.on("/", HTTP_GET, handleRoot);
   
  server.begin();

  MDNS.addService("http", "tcp", 80);
}

void loop(){
  potVals[potValPtr] = analogRead(POT_PIN);
  int tmp = 0;
  for (int i = 0; i < potAvg; i++) {
    tmp += potVals[i];
  }
  int tmpPotVal = tmp / potAvg;

  potValPtr++;
  if (!(potValPtr < potAvg)) {
    potValPtr = 0;
  }

  if (tmpPotVal != potVal) {
    potVal = tmpPotVal;
  }

  delay(100);
}
