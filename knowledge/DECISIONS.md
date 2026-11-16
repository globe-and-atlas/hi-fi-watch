---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:45:00-06:00"
---
# Decisions
## 2026-11-05 — Native layout and honest meters
Use original polygon seven-segment digits and fixed 200×228 Emery geometry. Reuse the proven active-ink centering algorithm from LCD Club so leading1 times remain visually centered. Header, time, date, and battery redraw on minute changes; the separate meter layer redraws on seconds.

SEC/MIN labels replace the concept's audio L/R labels to communicate real watch data. Each block counts five intervals including the current interval; at59 all twelve light, next00 resets. Seconds Off stops second subscriptions.

## 2026-11-05 — Independent settings
Use a new UUID and own Clay settings, keeping LCD Club unchanged. Five preset palettes plus explicit custom selection prevent a color editor from silently changing an active preset. Date modes and time preference retain workspace conventions.

## 2026-11-06 — Native round layouts
Keep Emery's original draw path behind round-only branches. Add Chalk and Gabbro to the same UUID rather than creating separate products. Dedicated circular compositions were chosen over cropping the rectangular face, preserving readable hierarchy and existing settings. Shared helpers use180 logical units and rasterize at each screen's actual resolution. Round selection is automatic, with no added settings key. Local0.1.1 is not published by this task.

## 2026-11-07 — optional needle meters with platform defaults preserved
The reference image is the existing round composition. Emery uses segmentedbar meters and round targets already use needles. Add NeedleDials as appended AppMessage10007/persistence8. Missing preference defaults EmeryOff and Chalk/GabbroOn in both C and companion/Clay startup. Explicit preference wins on every target. Existing appearance is not silently changed.

Emery retains its established layout; optional needle panels replace only the middle meter strip. Round style4 keeps its original needle path whenOn and gains segmented bars whenOff. SEC uses seconds and MIN uses minutes. SecondsOff suppresses the SEC hand, labelsOFF and keeps minute-only subscription. These are time meters, not audiolevel inputs. NewEmery panel tint rounds channels to nearest85 to match round/reference; existingbar colors are untouched.
