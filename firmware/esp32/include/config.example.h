#pragma once

// Copy this file to config.h and edit these values for your network.

#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Use your computer's LAN IP when running the backend locally.
// Example: http://192.168.1.25:8000/api/replay/next
#define BACKEND_EVENT_URL "http://192.168.1.25:8000/api/replay/next"

#define DEVICE_NAME "f1-race-control-device"

// Hardware pins
#define LED_PIN 5
#define BUTTON_PIN 18
#define OLED_SDA_PIN 21
#define OLED_SCL_PIN 22

// WS2812B strip settings
#define LED_COUNT 30
#define LED_BRIGHTNESS 96

// Poll interval for local backend replay mode
#define POLL_INTERVAL_MS 3000
