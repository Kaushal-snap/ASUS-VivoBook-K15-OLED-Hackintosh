# Graphics Fix for ASUS VivoBook K15 OLED (K513EA) - Intel Iris Xe 8086:9A49

This fix provides the proper Graphics configuration and DeviceProperties injection
required for native QE/QI support, correct resolution, and backlight control on the
Intel Iris Xe iGPU (device ID 8086:9A49) used in the ASUS VivoBook K15 OLED K513EA.

## Hardware
- **Laptop:** ASUS VivoBook K15 OLED K513EA
- **CPU:** Intel Core i5-1135G7
- **iGPU:** Intel Iris Xe (device ID `8086:9A49`)

## Problem
Without proper Graphics configuration, the system may experience:
- Wrong resolution or limited resolution support
- No QE/QI (Quartz Extreme Infrastructure) acceleration
- Missing or incorrect framebuffer configuration
- Backlight control issues (may be partially handled by separate BrightnessFix)

## Root Cause
The OpenCore `config.plist` requires proper `DeviceProperties.Add` entries to:
1. Set the correct `AAPL,ig-platform-id` for Intel Iris Xe (0x9A49)
2. Provide proper `framebuffer` properties for the built-in display
3. Enable backlight control via `APPL,backlight-control`
4. Inject necessary property overrides for the iGPU framebuffer

The existing config.plist already has some of these settings but may need
enhancement for complete functionality.

## Solution
Add/Enhance the `DeviceProperties.Add` section in `config.plist` with the
following entries for Intel Iris Xe 8086:9A49:

### Required DeviceProperties

```xml
<key>DeviceProperties</key>
<dict>
    <key>Add</key>
    <dict>
        <key>PciRoot(0x0)/Pci(0x2,0x0)</key>
        <dict>
            <key>AAPL,ig-platform-id</key>
            <data>
                AA000000
            </data>
            <!-- 0x9A49 in little-endian: 9A49 0000 -->
            
            <key>framebuffer-con0-framebuffer-width</key>
            <data>
                AA000000
            </data>
            <!-- 1920 in little-endian -->
            
            <key>framebuffer-con0-framebuffer-height</key>
            <data>
                A80A0000
            </data>
            <!-- 1080 in little-endian -->
            
            <key>framebuffer-con0-alpha-length</key>
            <data>
                20000000
            </data>
            <!-- 8 bits alpha -->
            
            <key>framebuffer-con0-pixel-rate</key>
            <data>
                5C400000
            </data>
            <!-- 148.5 MHz pixel clock -->
            
            <key>framebuffer-stolenmem</key>
            <data>
                02000000
            </data>
            <!-- 2 MB stolen memory -->
            
            <key>APPL,backlight-control</key>
            <data>
                01000000
            </data>
            <!-- 1 = enable -->
            
            <key>device-id</key>
            <data>
                05000000
            </data>
            <!-- 0x00005A49 -->
        </dict>
    </dict>
    <key>Delete</key>
    <dict/>
</dict>
```

### Also Add to config.plist Graphics Section

The following Graphics kext properties can help with compatibility:

- `ProvideConsoleGOP` = true (already present)
- `ReconnectGraphicsOnConnect` = true (recommended)
- `IgPlatform` kext properties via Lilu or WhateverGreen

## Files Included

- `Fixes/Graphics/` - Directory for Graphics configuration fixes
- This README.md document

## Installation Instructions

1. **Backup your existing config.plist** before making changes:
   ```bash
   cp EFI/OC/config.plist EFI/OC/config.plist.backup-$(date +%Y%m%d)
   ```

2. **Edit config.plist** and add/Enhance the `DeviceProperties.Add` entry under the
   `DeviceProperties` dictionary, targeting `PciRoot(0x0)/Pci(0x2,0x0)` (the iGPU).

3. **Use PlistBuddy** or a plist editor to add the entries. Example using PlistBuddy:
   ```bash
   /usr/libexec/PlistBuddy -c "Add :DeviceProperties:Add:PciRoot\(0x0\)\/Pci\(0x2,0x0\):AAPL,ig-platform-id data \"AA000000\"" config.plist
   /usr/libexec/PlistBuddy -c "Add :DeviceProperties:Add:PciRoot\(0x0\)\/Pci\(0x2,0x0\):framebuffer-con0-framebuffer-width data \"AA000000\"" config.plist
   /usr/libexec/PlistBuddy -c "Add :DeviceProperties:Add:PciRoot\(0x0\)\/Pci\(0x2,0x0\):framebuffer-con0-framebuffer-height data \"A80A0000\"" config.plist
   /usr/libexec/PlistBuddy -c "Add :DeviceProperties:Add:PciRoot\(0x0\)\/Pci\(0x2,0x0\):APPL,backlight-control data \"01000000\"" config.plist
   /usr/libexec/PlistBuddy -c "Add :DeviceProperties:Add:PciRoot\(0x0\)\/Pci\(0x2,0x0\):device-id data \"05000000\"" config.plist
   ```

4. **Verify** the changes with:
   ```bash
   /usr/local/bin/node /Users/mac/rea/bin/rea doctor
   ```

5. **Reboot** the system to apply changes.

## Verification

After reboot, verify graphics are working correctly:

- Check `system_profiler SPDisplaysDataType` shows Intel Iris Xe with correct resolution
- Verify QE/QI is enabled in `About This Mac → More Info → System Report → Graphics`
- Test brightness control (may require separate BrightnessFix)
- Verify correct refresh rate and color depth

## Upstream References

- Original configuration from Kaushal-snap repository:
  https://github.com/Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh
- Intel Iris Xe platform-id: 0x9A49 (from Apple's ig-platform-ids database)
- OpenCore ACPI and DeviceProperties documentation

## License

MIT – feel free to adapt and share. See LICENSE file for details.

## Contributing

1. Fork this repository (the `Fixes/` directory structure).
2. Improve the DeviceProperties entries or add framebuffer properties for other resolutions.
3. Test on actual hardware and report results.
4. Open a pull request with tested changes.

---

**Note:** This fix focuses on Graphics configuration. The Brightness/Backlight control
is handled separately by the BrightnessFix script in `BrightnessFix/`. Both fixes
work together for full iGPU functionality on the VivoBook K15 OLED.