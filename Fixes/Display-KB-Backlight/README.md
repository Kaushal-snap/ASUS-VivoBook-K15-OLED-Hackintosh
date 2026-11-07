# ASUS VivoBook K15 OLED K513EA - Display + Keyboard Backlight Fix

## EXACT ROOT CAUSES

### Display Brightness (Fn keys)
1. BrightnessKeys.kext IS loaded (as.acidanthera.BrightnessKeys 1.0.4)
2. SSDT-PNLF.aml IS enabled - backlight device defined (_HID: APP0002)
3. DSDT contains _Q14 and _Q15 - brightness down/up methods exist
4. SSDT-EC.aml is minimal - only defines EC device with _HID/ACID0001 and _STA
5. SSDT-EC.aml does NOT expose _Q14/_Q15 brightness event methods to macOS
6. BrightnessKeys.kext has no ACPI events to handle -> brightness keys do nothing

### Keyboard Backlight (Fn+F7)
1. DSDT contains _QD7 and _QD8 - keyboard backlight toggle methods
2. DSDT contains ATKP{KBLC} - keyboard backlight control in ATKP device
3. DSDT contains SLKB and GLKB - keyboard backlight devices
4. SSDT-KBBacklight.aml is only 123 bytes - minimal stub referencing _SB_ATKD and _SB_SLKB
5. AsusSMC.kext is ENABLED but NOT loading - version 1.4.1 built for macOS 10.15, incompatible with Tahoe 26.7.1
6. VirtualSMC IS loaded (1.3.9) - AsusSMC plugin dependency is met

### VoodooPS2Controller NOT LOADED (NOT A BUG)
- Keyboard is I2C-based (I2C0-I2C5 controllers in DSDT)
- VoodooI2C (2.9.1) IS loaded and handles keyboard
- VoodooPS2Controller not loading is CORRECT for this hardware

## REVERSIBLE FIX PLAN

### Fix A: Expose EC Brightness Event Methods
File to modify: SSDT-EC.aml
Change: Add _Q14 and _Q15 methods that forward to ATKD brightness query methods
Why: BrightnessKeys.kext needs these ACPI events to trigger brightness changes

### Fix B: Enable AsusSMC Keyboard Backlight
File to modify: config.plist (no change - already enabled)
Issue: AsusSMC 1.4.1 not loading due to macOS version incompatibility
Workaround: BrightnessKeys.kext + proper SSDT-EC _Qxx methods may handle keyboard backlight via _QD7/_QD8

### Fix C: Update SSDT-KBBacklight.aml
File to modify: SSDT-KBBacklight.aml
Change: Ensure proper interface with _SB_ATKD and _SB_SLKB devices

## PRECAUTIONS (ALL RESPECTED)

| Precaution | Status |
|------------|--------|
| Preserve Iris Xe/NMX graphics | Not modified |
| Preserve GPU DeviceProperties | Not modified |
| Preserve Windows EFI/partitions | Not modified |
| Back up EFI before changes | Done |
| Do not blindly add _Q14/_Q15 | Will verify from actual DSDT |
| Do not duplicate ACPI methods | Will check existing methods first |
| Do not introduce conflicting drivers | VoodooPS2Controller correctly not loading |
| Do not disable SIP/Gatekeeper | Not modified |
| Validate config.plist | plutil -lint OK |
| Compile generated SSDTs | Requires iasl (not available in this env) |
| Provide rollback script | Created |

## ROLLBACK INSTRUCTIONS

```bash
# Method 1: Use rollback script
./scripts/restore-efi-backup.sh /Volumes/1401/EFI-backup-20261107-170600

# Method 2: Manual restore
plutil -lint /Volumes/1401/EFI-backup-20261107-170600/OC/config.plist
cp -a /Volumes/1401/EFI /Volumes/1401/EFI-safety-backup-$(date +%Y%m%d-%H%M%S)
cp -a /Volumes/1401/EFI-backup-20261107-170600/* /Volumes/1401/EFI/
plutil -lint /Volumes/1401/EFI/OC/config.plist
```

## TEST PLAN (REQUIRES macOS HARDWARE)

| Test | Action | Expected | Status |
|------|--------|----------|--------|
| T1 | Fn+F5 | Brightness decreases | NOT VERIFIED |
| T2 | Fn+F6 | Brightness increases | NOT VERIFIED |
| T3 | Fn+F7 | Keyboard backlight toggle | NOT VERIFIED |
| T4 | Multiple Fn+F7 presses | All backlight levels cycle | NOT VERIFIED |
| T5 | Sleep/wake | Keyboard backlight state preserved | NOT VERIFIED |
| T6 | Reboot | Keyboard backlight state preserved | NOT VERIFIED |

## MODIFIED FILE LIST

None yet - investigation phase complete. Fix requires:
1. Boot macOS to verify actual _Qxx method names
2. Extract and disassemble DSDT from running system
3. Create proper SSDT patches based on actual hardware
4. Compile with iasl
5. Test on actual hardware

## UPSTREAM REFERENCES

- https://github.com/hieplpvip/AsusSMC - AsusSMC VirtualSMC plugin
- https://github.com/acidanthera/BrightnessKeys - BrightnessKeys kext
- https://github.com/Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh - Reference config
- https://github.com/zep245/IrisXe-macos-driver - Iris Xe driver reference

## PRESERVATION

- Iris Xe/NMX graphics: NOT modified
- Windows EFI: NOT modified
- Internal partitions: NOT modified
- SIP/Gatekeeper: NOT modified
- Administrator passwords: NOT stored
