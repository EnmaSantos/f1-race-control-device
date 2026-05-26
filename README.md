# F1 Race Control Device

A hardware-powered Formula 1 race control alert device that displays live race status, flag events, safety car alerts, fastest-lap moments, and favorite-driver updates using an ESP32, LED strip, OLED screen, and F1 data APIs.

F1 Race Control Device turns live and historical Formula 1 data into a physical race-control alert system. The device uses an ESP32, RGB LED strip, OLED display, and optional buttons or speaker to react to F1 session events such as yellow flags, red flags, Safety Car, Virtual Safety Car, DRS status, weather changes, fastest laps, and favorite-driver updates.

The first version is a simple desk or TV-side alert box. Later versions can expand into a full mini pit wall with a dashboard, strategy simulator, TV ambient lighting, standings board, and engineer-style voice alerts.

## Version 1 Goal

Build a desk/TV-side F1 alert box that replays race-control events from historical F1 sessions and lights up based on flags, Safety Car, fastest laps, and race-finish events.

Example behavior:

| Race event | Device behavior |
| --- | --- |
| Yellow flag | Yellow pulsing LED animation |
| Red flag | Flashing red LEDs |
| Safety Car | Yellow/blue animation |
| Fastest lap | Purple flash |
| Favorite driver pit stop | Team-color alert |
| Race finished | Checkered-flag-style animation |
| Latest event | OLED message text |

## MVP Scope

Hardware:

- ESP32
- Small SSD1306 I2C OLED screen
- WS2812B RGB LED strip or LED ring
- One button
- USB power

Software:

- ESP32 connects to Wi-Fi
- ESP32 polls a local backend endpoint
- Backend serves replay events from sample data or OpenF1 historical race-control data
- Device maps race events to LED colors/animations
- OLED displays the latest event text
- Button cycles modes or mutes alerts

## Repo Structure

```text
f1-race-control-device/
├── README.md
├── docs/
│   ├── project-plan.md
│   ├── hardware.md
│   └── api-notes.md
├── firmware/
│   └── esp32/
│       ├── src/
│       ├── include/
│       └── platformio.ini
├── backend/
│   ├── app/
│   │   ├── main.py
│   │   ├── openf1_client.py
│   │   └── event_mapper.py
│   ├── requirements.txt
│   └── sample_events.json
├── web-dashboard/
│   └── README.md
└── assets/
    └── wiring-diagrams/
```

## Quick Start: Backend Replay

The backend lets you test the project before the hardware is wired.

```bash
cd backend
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
uvicorn app.main:app --reload
```

Then open:

- Health check: [http://localhost:8000/health](http://localhost:8000/health)
- Current replay event: [http://localhost:8000/api/replay/current](http://localhost:8000/api/replay/current)
- Next replay event: [http://localhost:8000/api/replay/next](http://localhost:8000/api/replay/next)
- API docs: [http://localhost:8000/docs](http://localhost:8000/docs)

## Firmware Setup

The ESP32 firmware is a PlatformIO/Arduino starter.

1. Install PlatformIO.
2. Copy `firmware/esp32/include/config.example.h` to `firmware/esp32/include/config.h`.
3. Edit Wi-Fi and backend settings in `config.h`.
4. Build and upload:

```bash
cd firmware/esp32
pio run
pio run --target upload
```

## Event Format

The ESP32 polls the backend and receives a normalized event:

```json
{
  "event_type": "SAFETY_CAR",
  "message": "Safety Car deployed",
  "timestamp": "2024-05-26T14:22:00Z",
  "priority": "high",
  "color": "yellow_blue"
}
```

## Roadmap

- [ ] Wire ESP32 + LED strip
- [ ] Add OLED display
- [ ] Add button input
- [ ] Create local test event mode
- [ ] Add LED animations for race events
- [ ] Build Python backend
- [ ] Connect backend to OpenF1
- [ ] Add historical race replay mode
- [ ] Add favorite driver mode
- [ ] Add web dashboard
- [ ] Add strategy simulator

## Data Sources

- [OpenF1](https://openf1.org/docs/) for race-control, session, weather, pit, interval, position, and timing data. Historical data from 2023 onward is available without authentication; real-time data requires a paid subscription.
- [FastF1](https://docs.fastf1.dev/) later for deeper analysis, lap timing, telemetry, tire data, session results, and strategy simulation.

This project is unofficial and is not associated with Formula 1, FIA, Formula One Licensing B.V., OpenF1, or FastF1.
