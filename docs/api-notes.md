# API Notes

## Primary Data Source: OpenF1

OpenF1 is the first data source for this project because it exposes Formula 1 session data over simple JSON/CSV endpoints.

Useful docs:

- OpenF1 documentation: https://openf1.org/docs/
- OpenF1 race-control endpoint: `https://api.openf1.org/v1/race_control`

Current project assumptions:

- Historical data from 2023 onward can be used without authentication.
- Real-time data requires a paid subscription.
- Version 1 should use historical replay mode first.

## Useful OpenF1 Endpoints

| Endpoint | Use |
| --- | --- |
| `/v1/race_control` | Flags, Safety Car, session status, race-control messages |
| `/v1/sessions` | Race weekend/session metadata |
| `/v1/weather` | Track weather |
| `/v1/pit` | Pit stops |
| `/v1/position` | Driver position changes |
| `/v1/intervals` | Gaps and intervals |

## Backend Normalization

The ESP32 should not need to know OpenF1's raw response shape. The backend maps raw API rows into a stable project event:

```json
{
  "event_type": "YELLOW_FLAG",
  "message": "Yellow flag in sector 2",
  "timestamp": "2024-05-26T14:22:00Z",
  "priority": "medium",
  "color": "yellow"
}
```

## Event Types

| Event type | Meaning | LED color mode |
| --- | --- | --- |
| `GREEN_FLAG` | Track clear/session green | `green` |
| `YELLOW_FLAG` | Yellow or double yellow flag | `yellow` |
| `RED_FLAG` | Session stopped | `red` |
| `SAFETY_CAR` | Safety Car deployed or ending | `yellow_blue` |
| `VSC` | Virtual Safety Car | `yellow` |
| `FASTEST_LAP` | Fastest lap moment | `purple` |
| `PIT_STOP` | Pit stop event | `team` |
| `RACE_FINISH` | Chequered flag/race end | `checkered` |
| `INFO` | Any other useful event | `white` |

## Local Backend Endpoints

| Endpoint | Purpose |
| --- | --- |
| `GET /health` | Backend health check |
| `GET /api/events` | All sample replay events |
| `GET /api/replay/current` | Current event without advancing |
| `GET /api/replay/next` | Next event and advance replay cursor |
| `POST /api/replay/reset` | Reset replay cursor |
| `GET /api/openf1/race-control` | Fetch and map OpenF1 race-control data |
