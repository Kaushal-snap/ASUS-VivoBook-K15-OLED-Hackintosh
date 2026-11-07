# ASUSK513Controller Test Matrix

## Test Execution Requirements

All tests require booting macOS Tahoe 26.7.1 with USB 1401 OpenCore drive inserted.

## Test Categories

### A. Driver Loading

| Test ID | Action | Expected Result | Status |
|---------|--------|-----------------|--------|
| T-A1 | Install ASUSK513Controller.kext to EFI/OC/Kexts/ | Kext copies successfully | Not verified |
| T-A2 | Add kext entry to config.plist Kernel → Add | Config validates with plutil | Not verified |
| T-A3 | Boot macOS | System boots to desktop | Not verified |
| T-A4 | Run `kextstat | grep ASUSK513` | ASUSK513Controller loaded | Not verified |
| T-A5 | Run `kextstat | grep AsusSMC` | AsusSMC still loaded (no conflict) | Not verified |

### B. Diagnostic Mode

| Test ID | Action | Expected Result | Status |
|---------|--------|-----------------|--------|
| T-B1 | Boot with diagnostics enabled | Console shows diagnostic report | Not verified |
| T-B2 | Check Console.app for ATKD/ATKQ/ATKP | Devices reported as found | Not verified |
| T-B3 | Check Console.app for _Q14/_Q15 | Methods reported | Not verified |
| T-B4 | Check Console.app for _QD7/_QD8 | Methods reported | Not verified |

### C. Display Brightness (Fn+F4/F5)

| Test ID | Action | Expected Result | Status |
|---------|--------|-----------------|--------|
| T-C1 | Press Fn+F4 | Display brightness decreases | Not verified |
| T-C2 | Press Fn+F5 | Display brightness increases | Not verified |
| T-C3 | Press Fn+F4/F5 repeatedly | Brightness cycles through levels | Not verified |
| T-C4 | Check Console.app for BrightnessKeys | Messages appear on key press | Not verified |

### D. Keyboard Backlight (Fn+F7)

| Test ID | Action | Expected Result | Status |
|---------|--------|-----------------|--------|
| T-D1 | Press Fn+F7 | Keyboard backlight toggles | Not verified |
| T-D2 | Press Fn+F7 repeatedly | Backlight cycles through levels | Not verified |
| T-D3 | Check Console.app for ASUSK513Controller | Messages appear on key press | Not verified |
| T-D4 | Sleep/wake cycle | Keyboard backlight state preserved | Not verified |
| T-D5 | Reboot | Keyboard backlight state preserved | Not verified |

### E. General Keyboard

| Test ID | Action | Expected Result | Status |
|---------|--------|-----------------|--------|
| T-E1 | Press each F1-F12 key | Correct macOS key registered | Not verified |
| T-E2 | Press Fn+F1 through Fn+F12 | Correct event generated | Not verified |
| T-E3 | Press Fn+Esc | Fn Lock toggles | Not verified |
| T-E4 | Press all modifier keys | Each modifier recognized | Not verified |
| T-E5 | Type alphanumeric characters | Each character appears | Not verified |
| T-E6 | Press navigation keys | Each arrow key recognized | Not verified |
| T-E7 | Press numeric keypad keys | Each key recognized | Not verified |

### F. System Stability

| Test ID | Action | Expected Result | Status |
|---------|--------|-----------------|--------|
| T-F1 | Cold boot | macOS boots to desktop | Not verified |
| T-F2 | Restart | macOS restarts cleanly | Not verified |
| T-F3 | Shutdown and cold boot | macOS boots cleanly | Not verified |
| T-F4 | Sleep/wake cycle | System wakes without panic | Not verified |
| T-F5 | Windows boot via ESC menu | Windows boots normally | Not verified |
| T-F6 | Remove USB, boot internal drive | Windows boots from internal SSD | Not verified |

## Success Criteria

A fix is considered successful ONLY when:

1. Display brightness changes with Fn+F4/F5 (observable)
2. Keyboard backlight toggles with Fn+F7 (observable)
3. Keyboard backlight state persists across sleep/wake
4. Keyboard backlight state persists across reboot
5. No kernel panics or system instability
6. Windows dual-boot remains functional
7. All existing functionality (graphics, audio, Wi-Fi, Bluetooth) unaffected

## Remaining Physical Tests

| Test | Priority | Reason |
|------|----------|--------|
| Capture keycodes via Karabiner-EventViewer | Critical | Cannot proceed without actual keycodes |
| Verify _Q14/_Q15/_QD7/_QD8 via ioreg | Critical | Confirms ACPI event delivery |
| Test Fn+F4/F5 brightness | Critical | Primary feature |
| Test Fn+F7 keyboard backlight | Critical | Primary feature |
| Test sleep/wake backlight persistence | High | User experience |
| Test reboot backlight persistence | High | User experience |
| Verify AsusSMC bound to PNP0C14 | Medium | Confirms keyboard backlight driver |
| Test all Fn combinations | Medium | Complete keyboard support |

## Unsupported Functions

| Function | Reason |
|----------|--------|
| VoodooPS2Controller keyboard input | Keyboard is I2C-based; VoodooI2C handles it |
| Numeric keypad navigation | Requires hardware-specific mapping |
| Any keycode mapping without hardware capture | Requires macOS hardware testing |
| AsusSMC Tahoe compatibility | Version 1.4.1 built for macOS 10.15; functional verification needed |
| Display brightness via ASUSK513Controller | Separate from this kext; handled by BrightnessKeys.kext |
