# Brightness Fix for ASUS VivoBook K15 OLED (K513EA)

One‑click script that automatically applies the required SSDT/ACPI patch and OpenCore `config.plist` changes to enable native backlight control on the Intel Iris Xe iGPU.

## Supported hardware
- **Laptop:** ASUS VivoBook K15 OLED K513EA
- **CPU:** Intel Core i5‑1135G7
- **iGPU:** Intel Iris Xe (device ID `8086:9A49`)

If your machine has a different iGPU, the script will exit early and report the detected ID so you can verify compatibility.

## What the script does
1. **Detects** the current iGPU device ID from the running system (`ioreg` / `system_profiler`).
2. **Compares** it against the known good ID (`9A49`). If it does not match, the script aborts – no changes are made.
3. **Creates/updates** an SSDT (`brightness_fix.aml`) in `EFI/OpenCore/ACPI/patched/` (or a location you specify). A minimal template is provided; replace it with your own compiled SSDT if you have one.
4. **Patches** `config.plist` (if present) by adding an `ACPI.Add` entry named `BrightnessFix` and a small kernel quirk for backlight.
5. **Backs up** any files it modifies (`.backup_YYYYMMDDHHMMSS` suffix).

## Usage

```bash
# Normal run (will modify files)
./fix_brightness.sh

# Dry‑run – show what would happen without writing anything
./fix_brightness.sh --dry-run

# Verbose output
./fix_brightness.sh --verbose

# Both flags
./fix_brightness.sh --dry-run --verbose
```

## Prerequisites
- macOS (or a Hackintosh running macOS Tahoe) with OpenCore bootloader.
- OpenCore EFI folder at `./EFI/OpenCore` relative to the repository root.
- (Optional) `iasl` compiler if you want to compile your own SSDT instead of using the template.
- `PlistBuddy` is used for `config.plist` edits and is included with macOS.

## License
MIT – feel free to adapt and share.

buymeacoffee.com/kl06

## Contributing
1. Fork this repository.
2. Add your own compiled SSDT (replace the template) or improve detection.
3. Open a pull request; please keep the script portable and add tests for other Intel iGPUs if you wish.

---

**Note:** This script was written specifically for the ASUS VivoBook K15 OLED configuration. Use at your own risk; always keep backups of your EFI folder.
