# ASUSK513Controller

Kernel extension for ASUS VivoBook K15 OLED K513EA running macOS Tahoe 26.7.1.

Provides ACPI ATK device handling, keyboard backlight control, and Fn key event processing.
Works alongside (does not replace) BrightnessKeys.kext for display brightness.

## Hardware Support

- **Laptop:** ASUS VivoBook K15 OLED K513EA
- **CPU:** Intel Core i5-1135G7 Tiger Lake-U
- **GPU:** Intel Iris Xe 8086:9A49
- **OS Target:** macOS Tahoe 26.7.1
- **Keyboard:** I2C-based (VoodooI2C handles input, not PS/2)

## Current Status

**DEBUG BUILD ONLY** - Do not use in production without testing.

This project is in early development. The following features are implemented:

| Feature | Status |
|---------|--------|
| ACPI ATK device discovery | Implemented (not hardware-tested) |
| Keyboard backlight control | Framework only (not hardware-tested) |
| Diagnostic mode | Implemented |
| Display brightness control | NOT implemented (use BrightnessKeys.kext) |
| Fn key event handling | Framework only |

## Installation

**Do not install in production EFI until verified.**

1. Build the DEBUG version: `./scripts/build-debug.sh`
2. Test on macOS Tahoe 26.7.1 hardware
3. Verify diagnostic output
4. Build RELEASE version: `./scripts/build-release.sh`
5. Test thoroughly before enabling in production

## Build Requirements

- macOS Tahoe 26.7.1 or later
- Xcode 16.0 or later
- macOS SDK 26.0

See `BUILD.md` for detailed build instructions.

## Files

| File | Purpose |
|------|---------|
| `src/ASUSK513Controller.cpp` | Main controller - ACPI device discovery |
| `src/ASUSK513ControllerDiagnostics.cpp` | Diagnostic mode - read-only device scan |
| `src/KeyboardBacklight.cpp` | Keyboard backlight control (disabled until verified) |
| `src/ACPIInterface.cpp` | Read-only ACPI interface |
| `src/Info.plist` | Kext bundle metadata |
| `include/ASUSK513ControllerDefs.h` | Common definitions |
| `docs/HARDWARE.md` | Hardware register and ACPI method documentation |
| `docs/ACPI-INTERFACE.md` | ACPI method interface specification |
| `docs/BUILD.md` | Build instructions |
| `docs/TEST-MATRIX.md` | Test matrix |

## Upstream Attribution

See `ATTRIBUTION.md` for upstream repository credits.

## License

MIT License - see LICENSE file.

## Warnings

- **Do not enable in production EFI until verified.**
- **Do not add speculative EC query methods.**
- **Do not blindly write to EC registers.**
- **Do not modify working Iris Xe/NMX graphics configuration.**
- **Preserve Windows EFI and internal disk data.**
