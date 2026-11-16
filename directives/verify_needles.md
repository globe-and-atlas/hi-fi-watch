---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-07T16:40:13-06:00"
---
# Verify optional needle meters

User wants the Hi-Fi preview's needle meters as a setting. The discrepancy is platform-specific: original Emery uses bars, original round layouts already use needles. Keep platform defaults unchanged; appended NeedleDials10007/P_NEEDLES8 selects either style. Existing seven keys retain their numbers. SEC follows seconds, MIN follows minutes; SecondsOff removes only the SEC hand and uses minute-only ticks.

Build fixture with `python3 execution/build_round_fixture.py`. Own one emulator, install `.tmp/round-qa/fixture.pbw`, explicitly select Hi-Fi in Watchfaces if install resumes another face. `python3 execution/capture_needles.py --platform emery` (repeatchalk/gabbro) rejects a wrong foreground face using a bar geometry/palette pixel check, verifies motion isolated toSEC, secondsOffstatic, and both toggle values across reinstall. Native evidence stays `.tmp/needle-native/`.

Needle panels quantize their ghost color to nearest85 before GColor conversion to match round/reference tint. Existing bars keep their old ghost function for exact appearance preservation.

`node watchface/settings.test.js` covers actual Clay save/cancel/reload, toggle values, stable keys and fresh startup defaults for each target. `python3 -m pytest tests -q` covers host policy and release helpers. Clean production build via `execution/build.py`; `execution/verify.py` checks metadata, native binary equality and absence of fixture globals. Fresh reviewer verifies captures and exact PBW. No store update is included in this user request.
