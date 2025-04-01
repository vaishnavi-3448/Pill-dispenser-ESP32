#define BLYNK_TEMPLATE_ID "TMPL3FC-d7f2R"
#define BLYNK_TEMPLATE_NAME "ESP32 Servo"
#include <time.h>  // Include the time library
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char auth[] = "OKu5ABxncyIXSk3okLPYk9kmPCsKI7FI";  
char ssid[] = ""; 
char password[] = "";    

Servo myServo;
int servoPin = 13;
int setHour = -1, setMinute = -1;  // Initialize to avoid errors

BLYNK_WRITE(V1) {
  setHour = param.asInt();
}

BLYNK_WRITE(V2) {
  setMinute = param.asInt();
}

void connectToWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();  // Connect to WiFi first
  Blynk.begin(auth, ssid, password);

  myServo.attach(servoPin);

  // Configure NTP time
  configTime(19800, 0, "pool.ntp.org");

  Serial.println("Waiting for time sync...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nTime synchronized!");
} // <-- THIS was missing before `loop()` started

void loop() {
  Blynk.run();

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;
    int currentMinute = timeinfo.tm_min;

    Serial.printf("Current Time: %02d:%02d\n", currentHour, currentMinute);

    if (setHour >= 0 && setMinute >= 0 && currentHour == setHour && currentMinute == setMinute) {
      Serial.println("Triggering servo...");
      myServo.write(90);  
      delay(1000);       
      myServo.write(0);   
      delay(60000);       
    }
  }

  delay(1000);  
}
