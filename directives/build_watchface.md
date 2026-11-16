---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:25:00-06:00"
---
# Build watchface
Run `node watchface/settings.test.js`, `python3 -m pytest tests/ -q`, `python3 execution/build.py`, then `python3 execution/verify.py`.

For native QA, clean the watchface build before `HIFI_FIXTURE=1 pebble build`. Install only when emulator ownership is confirmed. Fixture AppMessage integer keys are 11000 hour, 11001 minute, 11002 second, 11004 battery. The date is 2026-11-05. Fixture input is absent from production builds.

Rebuild production with execution/build.py after native captures. Do not publish fixture PBWs.
