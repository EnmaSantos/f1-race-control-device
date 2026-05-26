from __future__ import annotations

from enum import Enum
from typing import Any, Dict, Optional

from pydantic import BaseModel, Field


class EventType(str, Enum):
    GREEN_FLAG = "GREEN_FLAG"
    YELLOW_FLAG = "YELLOW_FLAG"
    RED_FLAG = "RED_FLAG"
    SAFETY_CAR = "SAFETY_CAR"
    VSC = "VSC"
    FASTEST_LAP = "FASTEST_LAP"
    PIT_STOP = "PIT_STOP"
    RACE_FINISH = "RACE_FINISH"
    INFO = "INFO"


class EventPriority(str, Enum):
    LOW = "low"
    MEDIUM = "medium"
    HIGH = "high"
    CRITICAL = "critical"


class RaceEvent(BaseModel):
    event_type: EventType
    message: str
    timestamp: str
    priority: EventPriority = EventPriority.LOW
    color: str = "white"
    raw: Optional[Dict[str, Any]] = Field(default=None, exclude=True)


def map_openf1_race_control(row: Dict[str, Any]) -> RaceEvent:
    """Map an OpenF1 race_control row to the device's stable event shape."""
    category = str(row.get("category") or "").upper()
    flag = str(row.get("flag") or "").upper()
    message = str(row.get("message") or "Race control update")
    timestamp = str(row.get("date") or "")

    event_type = EventType.INFO
    priority = EventPriority.LOW
    color = "white"

    if "RED" in flag or "RED FLAG" in message.upper():
        event_type = EventType.RED_FLAG
        priority = EventPriority.CRITICAL
        color = "red"
    elif "DOUBLE YELLOW" in flag or "YELLOW" in flag:
        event_type = EventType.YELLOW_FLAG
        priority = EventPriority.MEDIUM
        color = "yellow"
    elif "GREEN" in flag:
        event_type = EventType.GREEN_FLAG
        priority = EventPriority.LOW
        color = "green"
    elif "CHEQUERED" in flag or "CHECKERED" in flag:
        event_type = EventType.RACE_FINISH
        priority = EventPriority.HIGH
        color = "checkered"
    elif "SAFETY" in category or "SAFETY CAR" in message.upper():
        if "VIRTUAL" in message.upper() or "VSC" in message.upper():
            event_type = EventType.VSC
            priority = EventPriority.MEDIUM
            color = "yellow"
        else:
            event_type = EventType.SAFETY_CAR
            priority = EventPriority.HIGH
            color = "yellow_blue"
    elif "DRS" in category:
        event_type = EventType.INFO
        priority = EventPriority.LOW
        color = "green"

    return RaceEvent(
        event_type=event_type,
        message=message,
        timestamp=timestamp,
        priority=priority,
        color=color,
        raw=row,
    )
