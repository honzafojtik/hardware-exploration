#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 //

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "FPSC";
const char *password = "arsenal4life";

AsyncWebServer server(80);

int ledPin = 19; // Use the appropriate pin for the built-in LED on the LilyGO TTGO T-Lite W5500 ESP32

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  
  display.clearDisplay();

  // Draw text
  display.setTextSize(5);      
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0, 0);     
  display.println(F("ON"));

  // Show the display buffer on the screen
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    request->send(200, "text/plain", "LED is ON");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("ON"));
    display.display();
  });

  server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
    request->send(200, "text/plain", "LED is OFF");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("OFF"));
    display.display();
  });

  server.begin();
}

void loop() {
  // Add your own loop code if needed
}
