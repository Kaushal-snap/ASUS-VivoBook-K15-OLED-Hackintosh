# ASUS K513EA Hardware Specification

## Hardware Overview

| Component | Specification |
|-----------|---------------|
| Laptop | ASUS VivoBook K15 OLED K513EA |
| CPU | Intel Core i5-1135G7 Tiger Lake-U (4 cores, 8 threads with HT) |
| iGPU | Intel Iris Xe 8086:9A49 |
| Wi-Fi | Intel AX201 |
| Audio | Realtek ALC0256 |
| Keyboard | I2C-based (VoodooI2C handles input) |
| OS Target | macOS Tahoe 26.7.1 |

## ACPI Devices Discovered

### ATK Device Tree

```
\_SB_ATKD           - ASUS ATK hotkey device (PNP0C14 compatible)
  ATKQ              - ATK query device
  ATKP              - ATK parameter device
    KBLC            - Keyboard backlight control
    KBLD            - Keyboard backlight down
    KBLU            - Keyboard backlight up
```

### Embedded Controller

```
\_SB_PC00LPCB.EC__  - Embedded Controller (ACID0001)
  BLCT              - Backlight control register
  GLKB              - Get keyboard backlight level
  SLKB              - Screen/Keyboard backlight device
```

### ACPI Query Methods

| Method | Location | Purpose | Status in DSDT |
|--------|----------|---------|----------------|
| `_Q14` | `\_SB_ATKD` | Display brightness down | Present |
| `_Q15` | `\_SB_ATKD` | Display brightness up | Present |
| `_QD7` | `\_SB_ATKD` | Keyboard backlight toggle | Present |
| `_QD8` | `\_SB_ATKD` | Keyboard backlight toggle | Present |
| `_Q0A`-`_Q0F` | `\_SB_ATKD` | Unknown | Present |
| `_Q10`-`_Q18` | `\_SB_ATKD` | Unknown | Present |
| `_Q40`-`_Q41` | `\_SB_ATKD` | Unknown | Present |
| `_Q6C`-`_Q6F` | `\_SB_ATKD` | Unknown | Present |
| `_Q73`-`_Q78` | `\_SB_ATKD` | Unknown | Present |
| `_QA0`-`_QAC` | `\_SB_ATKD` | Unknown | Present |
| `_QAF` | `\_SB_ATKD` | Unknown | Present |
| `_QB0`-`_QB7` | `\_SB_ATKD` | Unknown | Present |
| `_QC1`-`_QCE` | `\_SB_ATKD` | Unknown | Present |
| `_QD3`-`_QDD` | `\_SB_ATKD` | Unknown | Present |
| `_QEB`-`_QEF` | `\_SB_ATKD` | Unknown | Present |

**Note:** Do not add speculative EC query methods. Only use methods verified present in the DSDT.

## Keyboard Map

| Physical Key | Legend | macOS Mapping | Status |
|-------------|--------|---------------|--------|
| F1 | Speaker Mute | Volume Mute | TBD |
| F2 | Volume Down | Volume Down | TBD |
| F3 | Volume Up | Volume Up | TBD |
| F4 | Display Brightness Down | Display Brightness Down | TBD |
| F5 | Display Brightness Up | Display Brightness Up | TBD |
| F6 | Display / Screen Icon | TBD | TBD |
| F7 | Keyboard Backlight | Keyboard Backlight Toggle | TBD |
| F8 | Display Output / Projection | TBD | TBD |
| F9 | Lock Icon | TBD | TBD |
| F10 | Camera / Camera Disable | TBD | TBD |
| F11 | Screen / Rectangle | TBD | TBD |
| F12 | Airplane Mode | TBD | TBD |

**Fn Combinations:**
- Fn+F4: Display Brightness Down
- Fn+F5: Display Brightness Up
- Fn+F7: Keyboard Backlight Toggle
- Fn+Esc: Fn Lock Toggle

**Modifier Mapping (macOS):**
- Windows key → Command (⌘)
* Alt → Option (⌥)
* Ctrl → Control (⌃)
* Shift → Shift (⇧)

## Known Hardware Limitations

- VoodooPS2Controller NOT required (keyboard is I2C-based)
- VoodooI2C 2.9.1 handles keyboard input
- AsusSMC 1.4.1 built for macOS 10.15 - Tahoe compatibility unverified
- BrightnessKeys.kext 1.0.4 built for macOS 13.1 - loads but ACPI event delivery unverified
