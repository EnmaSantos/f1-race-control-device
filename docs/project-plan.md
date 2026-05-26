# Project Plan

## Phase 1: Race Control Alert Box

Goal: make LEDs and OLED react to fake/sample race events.

Tasks:

- Wire ESP32, LED strip, OLED, and button.
- Flash simple firmware.
- Create event types:
  - `GREEN_FLAG`
  - `YELLOW_FLAG`
  - `RED_FLAG`
  - `SAFETY_CAR`
  - `VSC`
  - `FASTEST_LAP`
  - `PIT_STOP`
  - `RACE_FINISH`
- Create LED animations for each event.
- Display event text on OLED.

Success criteria:

- ESP32 boots and connects to Wi-Fi.
- Device polls the backend.
- LEDs change for each sample event.
- OLED shows the latest event message.
- Button mutes or cycles display mode.

## Phase 2: API-Backed Race Data

Goal: replace fake events with OpenF1 data.

Tasks:

- Fetch race-control/session data from OpenF1.
- Normalize API responses into the project event format.
- Replay historical events through the backend.
- ESP32 polls every few seconds and reacts to real historical data.

Suggested normalized event:

```json
{
  "event_type": "SAFETY_CAR",
  "message": "Safety Car deployed",
  "timestamp": "2024-05-26T14:22:00Z",
  "priority": "high",
  "color": "yellow_blue"
}
```

## Phase 3: Favorite Driver Mode

Goal: make the device personal.

Tasks:

- Add favorite driver config.
- Show driver position.
- Alert on pit stops.
- Alert on overtakes if data allows.
- Use team color for LEDs.

Example payload:

```json
{
  "favorite_driver": "LEC",
  "position": 3,
  "last_event": "Pit stop completed",
  "team_color": "#DC0000"
}
```

## Phase 4: Mini Pit Wall Dashboard

Goal: add a local web dashboard.

Tasks:

- Add a local web UI.
- Show session status.
- Show recent race-control messages.
- Show favorite driver status.
- Add manual test buttons for hardware events.

## Phase 5: Strategy Simulator

Goal: add simple "box or stay out?" strategy logic.

Tasks:

- Build a simple race strategy model.
- Compare one-stop vs two-stop.
- Add tire age and pit loss.
- Add Safety Car scenario button.
- Show recommendations:
  - "Box now"
  - "Stay out"
  - "Undercut window"
  - "Safety car gamble"

This should stay a later phase so the hardware MVP can land first.
