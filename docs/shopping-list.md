# Shopping List

This list is for Version 1: a desk/TV-side alert box with an ESP32, OLED display, WS2812B LEDs, one button, and a local replay backend.

Amazon listings change constantly, so use the links as searches and pick listings that match the specs. Avoid the absolute cheapest unknown listing if the reviews mention bad USB cables, dead boards, flickering LEDs, or missing parts.

## Buy First

| Priority | Item | Qty | Target spec | Amazon search |
| --- | --- | ---: | --- | --- |
| Must-have | ESP32 development board | 1-2 | ESP32 DevKit V1 or ESP32-DevKitC, USB-C preferred, CP2102 or CH340 USB serial | [ESP32 DevKit V1 USB C](https://www.amazon.com/s?k=ESP32+DevKit+V1+USB+C) |
| Must-have | WS2812B LED strip | 1 | 5V addressable RGB, 1m, 60 LEDs/m, non-waterproof is easier to cut/prototype | [WS2812B 5V addressable LED strip 1m 60 LEDs](https://www.amazon.com/s?k=WS2812B+5V+addressable+LED+strip+1m+60+LEDs) |
| Must-have | OLED display | 1 | 0.96 inch, SSD1306, 128x64, I2C, 4 pins: GND/VCC/SCL/SDA | [SSD1306 0.96 inch OLED I2C ESP32](https://www.amazon.com/s?k=SSD1306+0.96+inch+OLED+I2C+ESP32) |
| Must-have | Breadboard + jumper wire kit | 1 | Full or half-size breadboard, male-to-male and male-to-female Dupont wires | [breadboard jumper wire kit](https://www.amazon.com/s?k=breadboard+jumper+wire+kit) |
| Must-have | 5V power supply | 1 | 5V 3A DC supply for LEDs, ideally with screw terminal or barrel adapter | [5V 3A power supply LED strip](https://www.amazon.com/s?k=5V+3A+power+supply+LED+strip) |
| Strongly recommended | 74AHCT125 logic level shifter | 1 pack | 3.3V-to-5V data buffer, 74AHCT125 or 74HCT125 breakout/module | [74AHCT125 level shifter](https://www.amazon.com/s?k=74AHCT125+level+shifter) |
| Strongly recommended | Resistor/capacitor/button kit | 1 | Includes 330 ohm resistors, 10k resistors, momentary push buttons, 1000 uF capacitors | [electronics resistor capacitor button kit](https://www.amazon.com/s?k=electronics+resistor+capacitor+button+kit) |
| Nice polish | Plastic project enclosure | 1 | Small electronics box, enough room for ESP32, wires, LED output, OLED cutout | [plastic electronics project box enclosure](https://www.amazon.com/s?k=plastic+electronics+project+box+enclosure) |

## Recommended Cart

Start with these 8 line items:

1. ESP32 DevKit board, USB-C preferred.
2. WS2812B 5V LED strip, 1 meter, 60 LEDs.
3. SSD1306 0.96-inch I2C OLED display.
4. Breadboard + jumper wire kit.
5. Resistor/capacitor/button starter kit.
6. 5V 3A LED strip power supply.
7. 74AHCT125 or 74HCT125 logic level shifter.
8. Plastic project enclosure.

Estimated total: $45-$75 depending on bundles, shipping, and brands.

## Buying Notes

- Prefer USB-C ESP32 boards, but confirm the listing says it is a data/programming port, not power-only.
- Buy two ESP32 boards if the price difference is small. Having a spare saves a lot of debugging time.
- For the LED strip, 1 meter / 60 LEDs is plenty for the first build. More LEDs need more power and more careful wiring.
- Get a real 5V power supply for the LED strip. Do not power a long strip directly from the ESP32 5V pin.
- A 74AHCT125 or 74HCT125 is the clean choice for the LED data signal because the ESP32 is a 3.3V device and the LED strip is usually powered at 5V.
- Use a 300-500 ohm resistor in series with the first LED data input.
- Add a 500-1000 uF capacitor across LED strip 5V and GND near the strip input.

## Skip For Now

Do not buy these yet:

- Raspberry Pi
- Large touchscreen
- E-ink display
- Microphone
- Big LED matrix
- Rotary encoder
- 3D printed custom case
- Track-map display hardware

Those are good Phase 3+ upgrades. The first win is getting one alert box reacting reliably to replayed race events.

## Reference Notes

- Espressif's ESP32-DevKitC docs describe the ESP32 dev board family and GPIO breakout approach.
- Adafruit's NeoPixel guide recommends a data-line resistor and a large capacitor for LED strip reliability, and explains why 3.3V microcontrollers often need logic level shifting for 5V NeoPixels.

Sources:

- https://www.espressif.com/en/products/devkits/esp32-devkitc
- https://learn.adafruit.com/adafruit-neopixel-uberguide/power
- https://learn.adafruit.com/adafruit-neopixel-uberguide/best-practices
- https://learn.adafruit.com/neopixel-levelshifter/shifting-levels
