from __future__ import annotations

import json
from pathlib import Path
from typing import Annotated, List, Optional

from fastapi import FastAPI, HTTPException, Query
import httpx
from pydantic import BaseModel

from app.event_mapper import RaceEvent, map_openf1_race_control
from app.openf1_client import OpenF1Client


APP_ROOT = Path(__file__).resolve().parents[1]
SAMPLE_EVENTS_PATH = APP_ROOT / "sample_events.json"

app = FastAPI(
    title="F1 Race Control Device API",
    description="Local replay backend for the ESP32 race control alert box.",
    version="0.1.0",
)

openf1 = OpenF1Client()
replay_cursor = 0


class HealthResponse(BaseModel):
    status: str
    sample_events: int


def load_sample_events() -> List[RaceEvent]:
    try:
        rows = json.loads(SAMPLE_EVENTS_PATH.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise HTTPException(status_code=500, detail="sample_events.json is missing") from exc

    return [RaceEvent.model_validate(row) for row in rows]


@app.get("/health", response_model=HealthResponse)
def health() -> HealthResponse:
    return HealthResponse(status="ok", sample_events=len(load_sample_events()))


@app.get("/api/events", response_model=List[RaceEvent])
def events() -> List[RaceEvent]:
    return load_sample_events()


@app.get("/api/replay/current", response_model=RaceEvent)
def current_event() -> RaceEvent:
    events = load_sample_events()
    return events[replay_cursor % len(events)]


@app.get("/api/replay/next", response_model=RaceEvent)
def next_event() -> RaceEvent:
    global replay_cursor

    events = load_sample_events()
    event = events[replay_cursor % len(events)]
    replay_cursor = (replay_cursor + 1) % len(events)
    return event


@app.post("/api/replay/reset", response_model=RaceEvent)
def reset_replay() -> RaceEvent:
    global replay_cursor

    replay_cursor = 0
    return current_event()


@app.get("/api/openf1/race-control", response_model=List[RaceEvent])
async def openf1_race_control(
    session_key: Annotated[Optional[str], Query(description="OpenF1 session_key, such as latest")] = None,
    meeting_key: Annotated[Optional[str], Query(description="OpenF1 meeting_key, such as latest")] = None,
    date_gte: Annotated[Optional[str], Query(description="Filter date lower bound, ISO 8601")] = None,
    date_lt: Annotated[Optional[str], Query(description="Filter date upper bound, ISO 8601")] = None,
    limit: Annotated[int, Query(ge=1, le=200)] = 50,
) -> List[RaceEvent]:
    try:
        rows = await openf1.get_race_control(
            session_key=session_key,
            meeting_key=meeting_key,
            date_gte=date_gte,
            date_lt=date_lt,
            limit=limit,
        )
    except httpx.HTTPError as exc:
        raise HTTPException(status_code=502, detail=f"OpenF1 request failed: {exc}") from exc

    return [map_openf1_race_control(row) for row in rows]
