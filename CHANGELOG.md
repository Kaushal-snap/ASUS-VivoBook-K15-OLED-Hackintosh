# Changelog

All notable fixes and changes are documented here.
Format based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

<!-- publish-fix.sh inserts new entries directly below the Unreleased heading. -->

## [Unreleased]

- **Keyboard** — ASUSK513Controller kernel extension for keyboard backlight and ATK handling — _Build Verified_: Independent kernel extension (source only) binding to PNP0C14/ASUS ATK devices, discovering SLKB/GLKB keyboard backlight devices, with read-only ACPI interface and keyboard backlight control stub. Compiles successfully on macOS Tahoe 26.7.1; not yet hardware-tested. ([details](Fixes/Keyboard/asusk513-controller/README.md))

- **Keyboard** — Keyboard backlight Fn+F7 and display brightness via ACPI _Qxx methods — _Experimental_: Investigation documents that SSDT-EC.aml does not expose _Q14/_Q15 (_QD7/_QD8) brightness events, AsusSMC 1.4.1 may be incompatible with Tahoe 26.7.1, and SSDT-KBBacklight.aml is a minimal stub. No EFI changes made yet. ([details](Fixes/Display-KB-Backlight/README.md))

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
