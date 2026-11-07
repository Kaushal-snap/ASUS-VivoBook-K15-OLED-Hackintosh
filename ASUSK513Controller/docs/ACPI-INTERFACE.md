# ACPI Interface Specification

## ACPI Device Tree

```
\_SB_ATKD              ASUS ATK hotkey device (PNP0C14 compatible)
  ATKQ                 ATK query device
  ATKP                 ATK parameter device
    KBLC               Keyboard backlight control
    KBLD               Keyboard backlight down
    KBLU               Keyboard backlight up
\_SB_PC00LPCB.EC__     Embedded Controller (ACID0001)
  BLCT                 Backlight control register
  GLKB                 Get keyboard backlight level
  SLKB                 Screen/Keyboard backlight device
\_SB_PC00GFX0.PNLF     Display backlight device (APP0002)
```

## Verified ACPI Methods

The following methods were found in the DSDT via strings analysis. Their exact implementations and return values must be verified on running macOS hardware using `ioreg`.

| Method | Namespace | Type | Purpose | Verified |
|--------|-----------|------|---------|----------|
| `_Q14` | `\_SB_ATKD` | EC Query | Display brightness down | In DSDT |
| `_Q15` | `\_SB_ATKD` | EC Query | Display brightness up | In DSDT |
| `_QD7` | `\_SB_ATKD` | EC Query | Keyboard backlight toggle | In DSDT |
| `_QD8` | `\_SB_ATKD` | EC Query | Keyboard backlight toggle | In DSDT |
| `_Q0A`-`_Q0F` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_Q10`-`_Q18` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_Q40`-`_Q41` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_Q6C`-`_Q6F` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_Q73`-`_Q78` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_QA0`-`_QAC` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_QAF` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_QB0`-`_QB7` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_QC1`-`_QCE` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_QD3`-`_QDD` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |
| `_QEB`-`_QEF` | `\_SB_ATKD` | EC Query | Unknown | In DSDT |

**WARNING:** Do not add speculative EC query methods. Only use methods verified present in the DSDT.

## ACPI Interface Class

The `ACPIInterface` class provides read-only access to ACPI devices and methods:

- `evaluateMethod(methodPath, result)` - Evaluate an ACPI method
- `methodExists(methodPath)` - Check if a method exists
- `hasATKD()`, `hasATKQ()`, `hasATKP()` - Check ATK device presence
- `hasSLKB()`, `hasGLKB()` - Check keyboard backlight device presence

## Diagnostic Mode

The `ASUSK513ControllerDiagnostics` class provides read-only diagnostic logging:
- Logs all detected ACPI devices
- Logs all discovered query methods
- Does NOT send ACPI queries or modify hardware state
- Enabled by default in DEBUG builds

## Implementation Rules

1. **Do not add speculative EC query methods** - Only use methods verified in DSDT
2. **Do not blindly write to EC registers** - Read-only unless hardware-verified
3. **Do not move EC query methods between namespaces** - Confirm EC ownership first
4. **Keep display brightness separate from keyboard backlight** - Separate modules
5. **Do not modify working Iris Xe/NMX graphics configuration** - Separate concern
