# Brightness Hotkey Fix for ASUS VivoBook K15 OLED K513EA

## Problem
Fn+F5 / Fn+F6 brightness hotkeys do not adjust screen brightness on macOS Tahoe 26.7.1.

## Root Cause Analysis

### Finding 1: BrightnessKeys.kext LOADED but no ACPI events
- `as.acidanthera.BrightnessKeys` (1.0.4) IS loaded in kernel
- BrightnessKeys.kext requires ACPI brightness event methods (`_Qxx`) from the Embedded Controller
- SSDT-EC.aml is enabled but may lack `_Q14`/`_Q15` (or equivalent) methods for Fn+F5/F6

### Finding 2: VoodooPS2Controller NOT LOADED
- ENABLED in config.plist but NOT present in `kextstat` output
- PS/2 keyboard Fn key passthrough may be broken
- All Fn key combinations affected, not just brightness

### Finding 3: DeviceProperties correctly configured
- `APPL,backlight-control` = 01 (enabled)
- `applbkl` = 01 (enabled)
- `AAPL,slot-name` = built-in
- Boot arg `applbkl=1` present

### Finding 4: SSDT-PNLF.aml ENABLED
- Backlight device defined: `_HID: APP0002`, `_CID: "backlight"`, `_UID: 0x13`
- Status method returns 0x0B (active) on Darwin

## Reversible Fix

### Step 1: Create Timestamped EFI Backup (DONE)
- Backup: `/Volumes/1401/EFI-backup-20261107-170600/`
- Validation: `plutil -lint` OK

### Step 2: Investigate VoodooPS2Controller Loading Issue
**On macOS, run:**
```bash
kextstat | grep -i VoodooPS2
```
**If NOT loaded:**
- Check `/var/log/install.log` or `log show --predicate 'process == "kernel" && eventMessage contains "VoodooPS2"'`
- Verify VoodooPS2Controller.kext dependencies are met
- Check if VoodooInput (separate kext) conflicts

### Step 3: Verify EC Brightness Event Methods
**On macOS, run:**
```bash
ioreg -lw0 | grep -i PNLF
ioreg -lw0 | grep -i _Q
```
**Look for:**
- `PNLF` device under `_SB_.PC00.GFX0`
- `_Q14` / `_Q15` methods (or model-specific brightness event methods)
- `BrightnessKeys` service in IORegistry

### Step 4: Test Fn Keys
**On macOS:**
- Press `Fn+F5` and `Fn+F6`
- Check `Console.app` for BrightnessKeys messages
- Check if brightness slider moves

## Files Modified (No modifications yet - investigation only)

| File | Status |
|------|--------|
| `/Volumes/1401/EFI/OC/config.plist` | NOT modified |
| `/Volumes/1401/EFI/OC/ACPI/SSDT-EC.aml` | NOT modified |
| `/Volumes/1401/EFI/OC/ACPI/SSDT-PNLF.aml` | NOT modified |
| `/Volumes/1401/EFI/OC/Kexts/BrightnessKeys.kext/` | NOT modified |

## Rollback Instructions

```bash
# Restore from verified backup
./scripts/restore-efi-backup.sh /Volumes/1401/EFI-backup-20261107-170600
```

**Or manually:**
```bash
# Verify backup first
plutil -lint /Volumes/1401/EFI-backup-20261107-170600/OC/config.plist

# Create safety backup of current EFI
cp -a /Volumes/1401/EFI /Volumes/1401/EFI-safety-backup-$(date +%Y%m%d-%H%M%S)

# Restore from backup
cp -a /Volumes/1401/EFI-backup-20261107-170600/* /Volumes/1401/EFI/

# Validate restored config
plutil -lint /Volumes/1401/EFI/OC/config.plist
```

## Next Steps (Require macOS Hardware Access)

1. **Boot macOS** with USB 1401 inserted
2. **Check kextstat** for VoodooPS2Controller loading
3. **Check ioreg** for PNLF device and _Qxx methods
4. **Test Fn+F5/F6** and observe brightness changes
5. **Check Console.app** for BrightnessKeys messages
6. **If VoodooPS2Controller not loading:** Investigate dependency conflicts
7. **If EC lacks _Qxx methods:** Extract and patch DSDT/SSDT from actual hardware

## Files to Potentially Modify (After Verification)

| File | Potential Change |
|------|------------------|
| `SSDT-EC.aml` | Add `_Q14`/`_Q15` brightness event methods |
| `SSDT-PNLF.aml` | Verify backlight device properties |
| `config.plist` | Adjust kernel quirks if needed |
| `config.plist` | Disable conflicting kexts if identified |

## Preservation

- Iris Xe/NMX graphics: NOT modified
- Windows EFI: NOT modified
- Internal partitions: NOT modified
- SIP/Gatekeeper: NOT modified
- Administrator passwords: NOT stored
