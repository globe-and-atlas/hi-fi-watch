---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:45:00-06:00"
---
# Hi-Fi
A native Pebble Time 2 watchface inspired by vintage stereo displays. Large optically centered seven-segment time, amber-on-black default colors, two progress meters, date, centered battery display.

## Local needle-meter update —0.2.0

Adds a **Needle dials** toggle on Time2, TimeRound and Round2. Existing defaults are preserved: Time2 starts with bars; round displays start with needles. Explicit selection persists. The new Time2 needle panels occupy the existing meter area below the clock; the established layout is retained. The round preview image already showed its native needle layout.

Needles represent seconds and minutes, not audio levels. SecondsOff removes theSEC hand and retains minute-only updates. Custom colors and date/time settings remain available. Local artifact: `dist/Hi-Fi-0.2.0.pbw`. This update is not published. Native checks: [verify_needles.md](directives/verify_needles.md).

## Settings
- Time: watch default, 12-hour, 24-hour. AM/PM appears in 12-hour mode.
- Date: Off, THU NOV 05, YYYY-MM-DD, MM/DD/YYYY, DD/MM/YYYY.
- Colors: Amber, Mint, Ice, Silver, Paper, Custom. Custom background, digits/labels, accent.
- Seconds meter On/Off. SEC follows the current second; MIN follows the current minute. Each of twelve blocks counts five intervals, including the current interval: at 04 the first block fills, at 59 all twelve fill, and at 00 the meter resets. Seconds Off marks the upper meter OFF and subscribes to minute updates only.

## Build and checks
```
node watchface/settings.test.js
python3 -m pytest tests/ -q
python3 execution/build.py
python3 execution/verify.py
```

## Native QA
Root coordinates the shared emulator. From watchface, run `pebble clean`, then `HIFI_FIXTURE=1 pebble build`. Install the fixture after confirming emulator ownership. Run `python3 execution/capture_native.py --monitor PORT` from this root. It saves palette/date/time/battery captures and `.tmp/native-checks.json`.

Fixture keys11000/11001/11002/11004 are hour/minute/second/battery. Fixture date2026-11-05Thursday. Fixture handlers are compiled out of production. After native captures, rebuild production with execution/build.py; that script strips HIFI_* variables.

Local PBW: `dist/Hi-Fi-0.2.0.pbw`. Supports Emery, Chalk, and Gabbro; no physical-watch validation or publication is implied.

## First public release history
https://apps.repebble.com/67e59e9db94d4a2f8a981e22

Version 0.1.0. First gallery image cycles through 5native color palettes; four static previews follow. Public GIF and production PBW verified against local release files.


## Round-compatible release — 0.1.1

Hi-Fi includes native layouts for Pebble Time2 (Emery200×228), original Time Round (Chalk180×180), and Round2 (Gabbro260×260). The rectangular layout and existing settings identities are preserved.

Build: `python3 execution/build.py`, then `python3 execution/verify.py`. Local bundle: `dist/Hi-Fi-0.1.1.pbw`. Native round previews: `media/round/`. Repeatable fixture/capture instructions: [round compatibility QA](knowledge/procedural/round_compatibility.md).

This update is [publicly available](https://apps.repebble.com/67e59e9db94d4a2f8a981e22) with round screenshots. Public PBW bytes and screenshot pixels verify; physical-watch validation remains separate.
