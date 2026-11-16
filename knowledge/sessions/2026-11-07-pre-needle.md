---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-07T16:44:14-06:00"
---
# Hi-Fi — published round update

Goal: publish the verified multi-platform release with round screenshots.
Agent: OpenAI Codex
Handoff-from: OpenAI Codex
Handoff-type: continuation
Status: published and public assets independently verified

## Result
- Version0.1.1 is public: [Hi-Fi](https://apps.repebble.com/67e59e9db94d4a2f8a981e22).
- Native binaries cover Emery200×228, Chalk180×180, and Gabbro260×260. Original UUID/settings mappings preserved.
- Two native screenshots appear under each round watch selection.
- Existing appID and original gallery bytes preserved; animated first preview unchanged.
- User explicitly excluded T9; its public release/gallery remain unchanged.

## Evidence
- Public PBW SHA-256 matches the reviewed production artifact: `86eb4dd9371b31dbe08f38f2f3d0ed3fd46ddfd5fd327317f05df73b733a8107`.
- Four public round images decode to exact native source pixels. Both watch selections were checked in the public UI.
- Fresh independent verifier accepted all15PBWs,60round images, sixnewGIFs, existing9galleries, and unchangedT9.
- Upload guards/read-retry/failure-path tests:5passed. No repeated release uploads were used for screenshot correction.
- No Git commits or pushes were performed. Physical-device testing remains separate.

## Artifacts and continuation
- `release/store-release.json`: current verified receipt.
- `release/round-update/0.1.1/public-proof.json`: public asset URLs and assertions.
- `media/store/round-update/`: native round listing assets and release plan.
- [Round store workflow](procedural/publish_round_update.md); [local validation archive](sessions/2026-11-06-round-local-validation.md).

S.D.G.: Lets owners of either round Pebble install Hi-Fi from the public store with accurate native previews.
Feynman gap: Public bytes and preview pixels are verified; physical-watch readability and endurance remain device checks.
