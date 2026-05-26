from __future__ import annotations

from typing import Any, Dict, List, Optional, Union
from urllib.parse import quote_plus

import httpx


OPENF1_BASE_URL = "https://api.openf1.org/v1"


class OpenF1Client:
    def __init__(self, base_url: str = OPENF1_BASE_URL) -> None:
        self.base_url = base_url.rstrip("/")

    async def get_race_control(
        self,
        *,
        session_key: Optional[Union[str, int]] = None,
        meeting_key: Optional[Union[str, int]] = None,
        date_gte: Optional[str] = None,
        date_lt: Optional[str] = None,
        limit: int = 50,
    ) -> List[Dict[str, Any]]:
        params: Dict[str, Union[str, int]] = {}
        if session_key is not None:
            params["session_key"] = session_key
        if meeting_key is not None:
            params["meeting_key"] = meeting_key

        query_parts = [f"{key}={quote_plus(str(value))}" for key, value in params.items()]
        if date_gte is not None:
            query_parts.append(f"date>={quote_plus(date_gte)}")
        if date_lt is not None:
            query_parts.append(f"date<{quote_plus(date_lt)}")

        url = f"{self.base_url}/race_control"
        if query_parts:
            url = f"{url}?{'&'.join(query_parts)}"

        async with httpx.AsyncClient(timeout=15) as client:
            response = await client.get(url)
            response.raise_for_status()
            data = response.json()

        if not isinstance(data, list):
            raise ValueError("Expected OpenF1 race_control response to be a list")

        sorted_rows = sorted(data, key=lambda row: str(row.get("date") or ""))
        return sorted_rows[:limit]
