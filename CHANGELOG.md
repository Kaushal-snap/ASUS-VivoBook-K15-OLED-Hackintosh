# Changelog

All notable fixes and changes are documented here.
Format based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

<!-- publish-fix.sh inserts new entries directly below the Unreleased heading. -->

## [Unreleased]

- **Graphics** — Expose EC brightness event methods for Fn+F5/F6 display brightness — _Experimental_: Investigation documents that BrightnessKeys.kext loads but receives no ACPI _Q14/_Q15 brightness events because SSDT-EC.aml is minimal; VoodooPS2Controller correctly not loaded (I2C keyboard); DeviceProperties backlight controls already correct. No EFI changes made yet. ([details](Fixes/Brightness-Hotkeys/README.md))

### Added
- Project automation and documentation structure:
  - `AGENTS.md` — permanent project rules for every session.
  - `Fixes/` — per-issue fix documentation (one folder per fix).
  - `scripts/` — selective, sanitized, attributed publishing workflow.
  - `docs/PUBLISHING.md` — how the workflow is used.
  - This `CHANGELOG.md` and `CREDITS.md`.

### Fixed
- `fix(graphics): enable Intel Iris Xe 8086:9A49 framebuffer configuration`
  - Added DeviceProperties entry with AAPL,ig-platform-id 0x9A49
  - Added framebuffer properties (con0-width, con0-height, alpha-length, pixel-rate, stolenmem)
  - Added APPL,backlight-control for backlight support
  - Updated config.plist for proper iGPU initialization


[Buy Me a Coffee](https://buymeacoffee.com/KL06)
