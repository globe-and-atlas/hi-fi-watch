---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:45:00-06:00"
---
# Dependencies
- @rebble/clay1.1.0, exact pin in watchface/package.json and npm lock. Handles configuration UI. Actual bundled Clay integration tests cover Save, Cancel, ready/reload.
- Pebble SDK4.33.1/CLI5.0.40 used for build; native target emery only.
- Pillow used by native capture helper; no font resources or external artwork are embedded.

- Pillow 12.3.0 pinned for native capture/image comparisons; removed unused template requests/dotenv dependencies. No new package installation was required.

## 2026-11-06 — Round SDK targets
Installed SDK4.33.1/tool5.0.40 built Emery, Chalk, and Gabbro without an SDK upgrade. Clay1.1.0 recognizes both round platforms. Pillow12.3.0 is native QA tooling only.
