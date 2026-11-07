# ASUSK513Controller Kernel Extension — Keyboard Backlight & ATK Handling

- **Category:** Keyboard
- **Status:** Build Verified
- **Hardware affected:** ASUS VivoBook K15 OLED (K513EA) — Intel Core i5-1135G7, macOS Tahoe 26.7.1

## Problem description

The ASUS VivoBook K15 OLED K513EA has no native macOS kernel extension for handling the ASUS ATK (PNP0C14) hotkey device, keyboard backlight (SLKB/GLKB), or Fn key ACPI events (_Q14/_Q15 display brightness, _QD7/_QD8 keyboard backlight). The existing AsusSMC.kext 1.4.1 is built for macOS 10.15 and its Tahoe 26.7.1 compatibility is unverified; BrightnessKeys.kext loads but receives no ACPI brightness events because SSDT-EC.aml does not expose _Q14/_Q15.

## Hardware affected

- Laptop: ASUS VivoBook K15 OLED K513EA
- CPU: Intel Core i5-1135G7 (Tiger Lake-U)
- iGPU: Intel Iris Xe 8086:9A49
- Keyboard: I2C-based (VoodooI2C handles input; VoodooPS2Controller not required)
- ACPI: ATKD/ATKQ/ATKP devices, SLKB/GLKB keyboard backlight devices, EC__ (_SB_PC00LPCB.EC__)

## Root cause

No macOS kernel extension exists for the K513EA's ASUS ATK/PNP0C14 hotkey device. The DSDT contains `_Q14`/`_Q15` (display brightness), `_QD7`/`_QD8` (keyboard backlight toggle), and `SLKB`/`GLKB`/`KBLC`/`KBLD`/`KBLU` fields, but no kext binds to these devices or processes these events on Tahoe 26.7.1.

## Solution

`ASUSK513Controller.kext` is an independent kernel extension (built from source, not adopted from upstream) that:

- Binds to `IOACPIPlatformDevice` providers matching PNP0C14/ASUS010/ASUS0100/ASUS0010/ATK0100
- Discovers ACPI ATKD/ATKQ/ATKP devices and SLKB/GLKB keyboard backlight devices
- Provides a read-only `ACPIInterface` module for device discovery (no speculative EC writes, no blind _Qxx additions)
- Provides a `KeyboardBacklight` module that activates only after hardware verification (SLKB + _QD7 or _QD8 present)
- Exposes `_Q14`/`_Q15`/`_QD7`/`_QD8` ACPI event handling stubs
- Includes a diagnostics module that logs detected devices without modifying hardware state

**Build status:** `xcodebuild` DEBUG configuration compiles successfully on macOS Tahoe 26.7.1 with SDK 26.0, x86_64 architecture. Build artifacts are in `ASUSK513Controller/build/Debug/`.

**Verification status:** Build Verified only. NOT hardware-tested — the kext has not been loaded on the actual K513EA hardware, and Fn+F5/F6/F7 key response has not been verified. Per AGENTS.md, this must not be marked "Confirmed Working" until hardware testing is performed.

## Files included

Published source files (build artifacts and Apple-provided headers excluded):

- `ASUSK513Controller/src/ASUSK513Controller.{cpp,h}` — main kext, ATK device discovery
- `ASUSK513Controller/src/ACPIInterface.{cpp,h}` — read-only ACPI device/method discovery
- `ASUSK513Controller/src/KeyboardBacklight.{cpp,h}` — keyboard backlight control (stub)
- `ASUSK513Controller/src/ASUSK513ControllerDiagnostics.{cpp,h}` — diagnostic reporting
- `ASUSK513Controller/src/Info.plist` — kext bundle configuration
- `ASUSK513Controller/include/ASUSK513ControllerDefs.h` — ACPI defines, version, bundle IDs
- `ASUSK513Controller/Makefile` — build entry point
- `ASUSK513Controller/LICENSE` — project license (MIT)
- `ASUSK513Controller/ATTRIBUTION.md` — upstream attribution
- `ASUSK513Controller/scripts/build-debug.sh`, `scripts/build-release.sh` — build scripts
- `ASUSK513Controller/docs/BUILD.md`, `docs/HARDWARE.md`, `docs/README.md`, `docs/TEST-MATRIX.md`, `docs/ACPI-INTERFACE.md` — documentation

Not published:
- `ASUSK513Controller/MacKernelSDK/` — Apple-provided kernel headers; redistribution license not confirmed, excluded.
- `ASUSK513Controller/build/` — build artifacts (.o, .kext binaries); excluded per .gitignore.
- `ASUSK513Controller/ASUSK513Controller.xcodeproj/project.pbxproj.bak` — backup, excluded.

## Installation instructions

1. Build the DEBUG kext:
   ```bash
   cd ASUSK513Controller
   ./scripts/build-debug.sh
   ```
2. Copy `build/Debug/ASUSK513Controller.kext` to `/Volumes/1401/EFI/OC/Kexts/`
3. Add a `Kernel → Add` entry in `config.plist` for the kext
4. Boot macOS and verify loading with `kextstat | grep ASUSK513`
5. Check diagnostic output in `Console.app`
6. Only after successful hardware testing, build and deploy the RELEASE version

## Source repository and contributor credits

This is an independent implementation for the ASUS VivoBook K15 OLED K513EA. Upstream references (consulted, not copied):

- **hieplpvip/AsusSMC** — VirtualSMC plugin reference for ATK device handling and keyboard backlight control (MIT)
- **acidanthera/BrightnessKeys** — brightness hotkey event handling architecture reference
- **acidanthera/OpenCorePkg** — KextInfo.plist structure and kext development patterns
- **Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh** — reference EFI configuration for K513EA (MIT)

## License and redistribution notes

ASUSK513Controller source is an independent implementation (MIT, see `ASUSK513Controller/LICENSE`). The `MacKernelSDK/` directory contains Apple-provided headers and is NOT published here due to unconfirmed redistribution rights. No third-party binaries are included.

## Testing procedure

1. Build DEBUG: `./scripts/build-debug.sh` (compiled successfully — Build Verified)
2. Deploy to EFI USB (`/Volumes/1401/EFI/OC/Kexts/`)
3. Boot mac Tahoe 26.7.1 with USB 1401
4. Verify `kextstat` shows ASUSK513Controller loaded
5. Verify Console.app diagnostic output shows ATKD/SLKB/GLKB discovery
6. Test Fn+F5/F6 (display brightness) and Fn+F7 (keyboard backlight toggle)
7. Test sleep/wake and reboot for state preservation
8. Only then build RELEASE and consider production deployment

## Verification status

**Build Verified** — the kext compiles cleanly with `xcodebuild` on macOS Tahoe 26.7.1 / SDK 26.0 / x86_64. Not yet hardware-tested; Fn-key response and ACPI event delivery unverified on actual hardware.
