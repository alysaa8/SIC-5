#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "MAN1";
const char *password = "majesa1234";
const char *serverName = "http://192.168.233.254:5000/submit";

#define TRIGGER_PIN 32
#define ECHO_PIN 33

void setup() {
  Serial.begin(115200);
  
  // Setup WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup HC-SR04 pins
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Triggering HC-SR04 to send ultrasonic pulse
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Calculating jarak based on echo duration
    long duration = pulseIn(ECHO_PIN, HIGH);
    float jarak_cm = duration * 0.034 / 2;

    // Construct JSON payload
    String jsonPayload = "{\"jarak\":";
    jsonPayload += jarak_cm;
    jsonPayload += "}";

    // Send POST request
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();

    delay(1000); // Delay before next measurement
  } else {
    Serial.println("WiFi not connected");
    delay(1000);
  }
}
