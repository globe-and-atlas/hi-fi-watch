---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:45:00-06:00"
---
# Errors
## 2026-11-05 — Progress meters never reached full
Error: floor(value/5) yielded11 blocks at59, so the12-block meter never filled.
Cause: treating the active zero-based interval as completed intervals.
Fix: (value+1)/5 for valid0..59, making4→1 and59→12; next00 resets. Host boundary tests and native capture sampling cover this mapping.
Graduated-to: knowledge/DECISIONS.md.

## 2026-11-05 — Silver meter contrast
Native captures found active and inactive Silver meter blocks identical after two-bit palette conversion: accent555555 collided with the quantized background/foreground blend. Changed Silver accent to000000, retaining gray ghost blocks; native capture assertions compare each active/inactive block and prevent accepting this palette collision.

##2026-11-05 — Public PBW storage redirect
Initial public verifier rejected the PBW endpoint redirect because its host allowlist contained only RePebble domains. Observed the public endpoint redirect hostname, allowed the exact store R2host, and verified downloadedPBW SHA256. No signed URL query is logged.

## 2026-11-06 — Preserve meter semantics during round adaptation
Error: an early round layout drove both needles from seconds (or minutes when seconds were disabled).
Fix: independent SEC and MIN inputs; disabling seconds removes only the SEC needle and shows OFF. Native image-region comparisons prove each input changes only its own meter.
Graduated-to: round_digital.h and execution/round_cases.json.

## 2026-11-07 — needle panel tint quantization
The new Emery needle-panel blend0x553800 became dark red when GColorFromHEX floored green56 to0. Round/reference panels round to palette steps and are olive0x555500. Fix: dial_ghost rounds each channel to nearest85 only on the new needle-panel path; old bar ghost colors remain unchanged. Graduated to main.c and directives/verify_needles.md.

## 2026-11-07 — retained active watchface after install
Installing Hi-Fi can resume WorldTimer instead of selecting Hi-Fi. Native toggle comparison caught the wrong app; no wrong screenshot was accepted as final evidence. Explicitly select Hi-Fi under Watchfaces and require a meter pixel identity check before cases. See capture_needles.py.
