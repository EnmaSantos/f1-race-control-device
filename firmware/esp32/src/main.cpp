#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if __has_include("config.h")
#include "config.h"
#else
#include "config.example.h"
#endif

namespace {
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;

CRGB leds[LED_COUNT];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String currentEventType = "INFO";
String currentMessage = "Booting race control";
String currentColor = "white";
String currentPriority = "low";

bool muted = false;
bool displayReady = false;
unsigned long lastPollMs = 0;
unsigned long lastButtonMs = 0;

void setAll(const CRGB &color) {
  fill_solid(leds, LED_COUNT, color);
  FastLED.show();
}

CRGB colorFromName(const String &colorName) {
  if (colorName == "green") return CRGB::Green;
  if (colorName == "yellow") return CRGB::Yellow;
  if (colorName == "red") return CRGB::Red;
  if (colorName == "purple") return CRGB::Purple;
  if (colorName == "team") return CRGB(220, 0, 0);
  if (colorName == "white") return CRGB::White;
  return CRGB::Blue;
}

void showMessage() {
  if (!displayReady) return;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("F1 Race Control");
  display.println(muted ? "MUTED" : "LIVE");
  display.println();
  display.println(currentEventType);
  display.println();
  display.println(currentMessage);
  display.display();
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(DEVICE_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("Connecting to Wi-Fi: %s\n", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWi-Fi connected: %s\n", WiFi.localIP().toString().c_str());
}

void applyEvent(const String &eventType, const String &message, const String &color, const String &priority) {
  currentEventType = eventType;
  currentMessage = message;
  currentColor = color;
  currentPriority = priority;

  Serial.printf("[%s] %s (%s)\n", currentEventType.c_str(), currentMessage.c_str(), currentColor.c_str());
  showMessage();
}

void fetchNextEvent() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  HTTPClient http;
  http.begin(BACKEND_EVENT_URL);
  const int statusCode = http.GET();

  if (statusCode != HTTP_CODE_OK) {
    Serial.printf("Backend request failed: HTTP %d\n", statusCode);
    http.end();
    return;
  }

  const String body = http.getString();
  http.end();

  JsonDocument doc;
  const DeserializationError error = deserializeJson(doc, body);
  if (error) {
    Serial.printf("JSON parse failed: %s\n", error.c_str());
    return;
  }

  applyEvent(
    doc["event_type"] | "INFO",
    doc["message"] | "Race control update",
    doc["color"] | "white",
    doc["priority"] | "low"
  );
}

void handleButton() {
  const bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;
  const unsigned long now = millis();

  if (buttonPressed && now - lastButtonMs > 350) {
    muted = !muted;
    lastButtonMs = now;
    Serial.printf("Muted: %s\n", muted ? "yes" : "no");
    showMessage();
  }
}

void animateYellowPulse(unsigned long now) {
  const uint8_t brightness = beatsin8(20, 20, LED_BRIGHTNESS);
  fill_solid(leds, LED_COUNT, CRGB::Yellow);
  FastLED.setBrightness(muted ? 8 : brightness);
  FastLED.show();
}

void animateRedFlash(unsigned long now) {
  const bool on = (now / 250) % 2 == 0;
  setAll(on && !muted ? CRGB::Red : CRGB::Black);
}

void animateSafetyCar(unsigned long now) {
  for (int i = 0; i < LED_COUNT; i++) {
    const bool blueSegment = ((i + now / 120) % 6) < 3;
    leds[i] = blueSegment ? CRGB::Blue : CRGB::Yellow;
  }
  FastLED.setBrightness(muted ? 8 : LED_BRIGHTNESS);
  FastLED.show();
}

void animateCheckered(unsigned long now) {
  for (int i = 0; i < LED_COUNT; i++) {
    const bool white = ((i + now / 250) % 2) == 0;
    leds[i] = white ? CRGB::White : CRGB::Black;
  }
  FastLED.setBrightness(muted ? 8 : LED_BRIGHTNESS);
  FastLED.show();
}

void updateLeds() {
  const unsigned long now = millis();

  if (muted) {
    FastLED.setBrightness(8);
  } else {
    FastLED.setBrightness(LED_BRIGHTNESS);
  }

  if (currentEventType == "YELLOW_FLAG" || currentEventType == "VSC") {
    animateYellowPulse(now);
  } else if (currentEventType == "RED_FLAG") {
    animateRedFlash(now);
  } else if (currentEventType == "SAFETY_CAR") {
    animateSafetyCar(now);
  } else if (currentEventType == "RACE_FINISH" || currentColor == "checkered") {
    animateCheckered(now);
  } else if (currentEventType == "FASTEST_LAP") {
    setAll(muted ? CRGB::Black : CRGB::Purple);
  } else {
    setAll(muted ? CRGB::Black : colorFromName(currentColor));
  }
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(LED_BRIGHTNESS);
  setAll(CRGB::Black);

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  displayReady = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (!displayReady) {
    Serial.println("OLED not found at 0x3C");
  }

  showMessage();
  connectWifi();
  fetchNextEvent();
}

void loop() {
  handleButton();

  const unsigned long now = millis();
  if (now - lastPollMs >= POLL_INTERVAL_MS) {
    lastPollMs = now;
    fetchNextEvent();
  }

  updateLeds();
  delay(20);
}
