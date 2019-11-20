#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

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

  const char* ssid = "your_ssid";
  const char* key =  "your_key";
 
  WiFi.begin(ssid, key);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
   
  server.begin();
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
