# Hardware Notes

## Core MVP

| Item | Quantity | Amazon search phrase | Why |
| --- | ---: | --- | --- |
| ESP32 development board | 1-2 | ESP32 DevKit V1 development board USB C | Main controller with Wi-Fi |
| WS2812B 5V LED strip or LED ring | 1 | WS2812B 5V addressable LED strip 1m 60 LEDs | Race alert lights |
| 0.96-inch OLED display, I2C, SSD1306 | 1 | SSD1306 0.96 inch OLED I2C display ESP32 | Race messages |
| Breadboard | 1 | solderless breadboard electronics | Prototype wiring |
| Jumper wires | 1 pack | male to female male to male jumper wires kit | Connect components |
| Push buttons | 1 pack | momentary push button switch breadboard | Mute/mode button |
| Resistor kit | 1 | resistor assortment kit 220 ohm 330 ohm 10k | Button pull-downs and LED data protection |
| 1000 uF capacitor | 1 pack | 1000uF capacitor 6.3V 10V electronics | Protect LED strip power input |
| 5V power supply | 1 | 5V 3A power supply LED strip | Power LEDs safely |
| USB cable for ESP32 | 1 | USB C data cable or micro USB data cable | Programming and power |

Estimated total: $45-$75 depending on kits and brands.

Must-have items:

- ESP32
- LED strip
- OLED display
- Breadboard
- Jumper wires
- 5V power supply

## Strongly Recommended

| Item | Quantity | Amazon search phrase | Why |
| --- | ---: | --- | --- |
| Logic level shifter | 1 pack | 3.3V to 5V logic level shifter 74AHCT125 | Stabilizes LED data from ESP32 |
| Dupont connector kit | 1 | Dupont connector kit crimp housing | Cleaner wiring later |
| Small project box | 1 | plastic electronics project box enclosure | Makes it look like a real device |
| Heat shrink tubing | 1 pack | heat shrink tubing assortment | Cleaner and safer wiring |
| Soldering kit | 1 | beginner soldering iron kit electronics | Needed for final build |

The logic level shifter is worth buying early. ESP32 GPIO pins output 3.3V data, while many WS2812B strips are powered at 5V. Some strips work directly, but a 74AHCT125 or similar shifter makes the LED signal much more reliable.

## Optional Later Upgrades

| Item | Amazon search phrase | Adds |
| --- | --- | --- |
| Buzzer or small speaker | active buzzer module ESP32 | Sound alerts |
| Rotary encoder knob | rotary encoder module KY-040 | Scroll/select driver or mode |
| Larger TFT display | 2.8 inch TFT SPI display ESP32 | Better pit wall UI |
| LED matrix panel | WS2812B 8x8 LED matrix | Flag animations/icons |
| E-ink display | 2.9 inch e paper display ESP32 | Standings board |
| 3D printed case later | ESP32 project enclosure | Final polished look |
| Raspberry Pi Zero 2 W | Raspberry Pi Zero 2 W | Full dashboard or voice assistant later |

## Do Not Buy Yet

- Raspberry Pi
- Large touchscreen
- E-ink display
- Microphone
- Big LED matrix
- 3D-printed custom parts
- Multiple track maps

Those are good later. First, get one physical device reacting correctly to race events.

## Initial Wiring Assumptions

These are the default pins used by the starter firmware:

| Component | ESP32 pin |
| --- | --- |
| WS2812B LED data | GPIO 5 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Button | GPIO 18 |

Power notes:

- Power the LED strip from a 5V supply.
- Connect ESP32 ground and LED power supply ground together.
- Put a 1000 uF capacitor across LED strip 5V and GND near the strip input.
- Put a 330 ohm resistor in series with the LED data line.
- Use a 74AHCT125 or similar level shifter between ESP32 data and WS2812B data input.
