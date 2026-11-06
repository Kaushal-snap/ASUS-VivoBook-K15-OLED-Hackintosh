# EFI Configuration Fix for ASUS VivoBook K15 OLED (K513EA)

## Problem Description
Users attempting to use a pre-built EFI folder for the ASUS VivoBook K15 OLED (K513EA) on Hackintosh setups often encounter issues with:
- SMBIOS serial number conflicts (MLB, ROM, SystemSerialNumber, SystemUUID)
- UUID mismatches causing kernel panics
- Invalid SMBIOS data preventing proper platform identification
- NVRAM UUID conflicts from copied configurations

This repository's EFI configuration contains original SMBIOS data that must be replaced before use on another unit.

## Hardware Affected
- Laptop: ASUS VivoBook K15 OLED — K513EA
- CPU: Intel Core i5-1135G7 (Tiger Lake-U)
- iGPU: Intel Iris Xe Graphics 8086:9A49
- Wi-Fi: Intel AX201
- Audio: Realtek ALC0256

## Root Cause
The original EFI configuration (from volume "1401") contains hardcoded SMBIOS data:
- **MLB**: `C02032501J9P8PG1F`
- **ROM**: binary data
- **SystemSerialNumber**: `C02D6XY4ML7H`
- **SystemUUID**: `385339AC-3465-458E-A503-F45B10FD9287`
- **SystemProductName**: `MacBookPro16,2`

Using these exact values on a different unit (or even a different revision of the same model) will cause:
- SMBIOS mismatch errors in `About This Mac`
- Potential kernel panics during boot
- iMessage/FaceTime deactivation
- Invalid platform identification

Additionally, the `UseRawUuidEncoding` setting and NVRAM UUID variables must be properly configured for the target hardware.

## Solution
1. **Copy your original EFI folder** from your working OpenCore volume (e.g., `/Volumes/1401/EFI/`)
2. **Replace SMBIOS data** with your own unique values:
   - Generate a new SMBIOS using tools like `GenSMBIOS`, `Clover Configurator`, or `ProperTree`
   - Use your laptop's actual serial number (from Windows `wmic bios get serialnumber` or Dell/HP equivalent)
   - Generate a valid UUID (random or based on your hardware)
   - Set `MLB` to match your serial number format
3. **Enable `UseRawUuidEncoding`** in `config.plist` → `PlatformInfo` → `Generic`
4. **Set `UpdateSMBIOS` to `false`** after replacing data (or keep `true` and let OpenCore update on next boot)
5. **Clear NVRAM UUID variables** (`%01var`, `%02var`) or set them to empty
6. **Update `SystemProductName`** if needed for your specific BIOS revision

### Recommended SMBIOS for K513EA
Based on the original configuration, use `MacBookPro16,2` as the `SystemProductName`. However, verify your exact BIOS revision, as some K513EA units may require:
- `MacBookAir8,7`
- `MacBookPro16,1`
- Other valid SMBIOS types

Generate your own serial using: `C02[XXXXX][YOURSERIES]ML7H` format (adjust last characters)

## Files Included
- `Fixes/EFI/config.plist` - Sanitized config.plist with placeholder SMBIOS values
- `Fixes/EFI/README.md` - This documentation file
- `EFI_backup_original/` - Original EFI backup (never publish this publicly)

## Installation Instructions

### Step 1: Backup Your Original EFI
```bash
cp -R /Volumes/1401/EFI ~/efi_original_backup
```

### Step 2: Generate Your SMBIOS Data
Use one of these methods:
- **GenSMBIOS** (recommended): Run on a working macOS install or Windows
- **ProperTree**: Open the original config.plist and modify SMBIOS fields
- **Clover Configurator**: For advanced users

Required values to generate:
- `SystemProductName`: `MacBookPro16,2` (or your revision-specific type)
- `SystemSerialNumber`: `C02D6XY4ML7H` → replace with your own
- `MLB`: `C02032501J9P8PG1F` → replace with your own
- `SystemUUID`: `385339AC-3465-458E-A503-F45B10FD9287` → replace with your own

### Step 3: Replace Config.plist
1. Copy the sanitized `config.plist` from `Fixes/EFI/` to your EFI/OC directory
2. Replace placeholder values with your generated SMBIOS data:
   - `<key>MLB</key>` → your MLB value
   - `<key>ROM</key>` → your ROM value  
   - `<key>SystemSerialNumber</key>` → your serial number
   - `<key>SystemUUID</key>` → your UUID

### Step 4: Configure PlatformInfo
Ensure these settings in `config.plist` → `PlatformInfo` → `Generic`:
- `UseRawUuidEncoding` = `True`
- `UpdateSMBIOS` = `False` (after data replacement) or `True` (let OpenCore update)

### Step 5: Clear NVRAM UUIDs
In `config.plist` → `NVRAM`:
- `%01var` = (empty)
- `%02var` = (empty)

Or remove these entries entirely.

### Step 6: Generate SSDTs (if needed)
The included SSDTs are for the specific K513EA revision. You may need to:
- Recompile SSDTs for your exact BIOS version
- Modify brightness SSDTs for your display panel
- Update USB SSDTs for your port mapping

## Source Repository and Credits
- **Original Repository**: https://github.com/Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh
- **Original Author**: Kaushal-snap
- **Upstream References**:
  - NullMoth - 1401 boot/EFI creation workflow
  - zep245 - IrisXe-macos-driver for Intel Iris Xe acceleration
- **SMBIOS Data**: Original values from user's working configuration

## License
This fix documentation is part of the ASUS-VivoBook-K15-OLED-Hackintosh project. 
The original repository is licensed under MIT. 
See `LICENSE` in the root repository for full license terms.

**Do not publish the original EFI folder publicly** without sanitizing SMBIOS data first.

## Testing Procedure
1. Replace all placeholder SMBIOS values with your generated data
2. Keep a backup of your original working EFI
3. Boot with `-v` flag for verbose mode
4. Check `About This Mac` → verify correct SMBIOS is displayed
5. Verify iMessage/FaceTime status (should be active with valid SMBIOS)
6. Run `system_profiler SPHardwareDataType` to confirm UUIDs match
7. Test sleep/wake cycle
8. Verify graphics acceleration with IrisXe Manager

## Verification Status
- **Experimental**: SMBIOS data replacement untested on other K513EA revisions
- **Known Working**: Original configuration on author's unit (K513EA with i5-1135G7)
- **Requires**: User-specific SMBIOS generation and testing

## Important Notes
- Each K513EA unit may have different BIOS revisions, requiring different SSDTs
- Wi-Fi model (AX201) may vary between laptop revisions
- Audio layout ID may need adjustment for different RAM configurations
- Never publish original SMBIOS serials, MLB, or ROM values publicly
- Keep your original EFI backup before making any changes