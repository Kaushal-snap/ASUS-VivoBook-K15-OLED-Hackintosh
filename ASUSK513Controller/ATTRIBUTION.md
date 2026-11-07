# Upstream Attribution

## Primary Upstream Repository

**Repository:** https://github.com/hieplpvip/AsusSMC  
**Author:** hieplpvip  
**Purpose:** VirtualSMC plugin for ASUS laptops providing ATK device emulation, keyboard backlight control, and Fn key event handling  
**License:** MIT (see upstream repository)  
**Usage in ASUSK513Controller:** Reference implementation for ACPI ATK device handling, keyboard backlight control methods, and VirtualSMC plugin architecture  
**Modifications:** Complete rewrite targeting ASUS VivoBook K15 OLED K513EA with macOS Tahoe 26.7.1 compatibility  

## Supporting References

**Repository:** https://github.com/acidanthera/BrightnessKeys  
**Author:** vit9696 (Acidanthera)  
**Purpose:** macOS kext for handling display brightness hotkeys via ACPI  
**License:** Check upstream repository  
**Usage:** Reference for BrightnessKeys event handling architecture  

**Repository:** https://github.com/Acidanthera/OpenCorePkg  
**Author:** Acidanthera  
**Purpose:** OpenCore bootloader reference for kernel extension development practices  
**License:** Check upstream repository  
**Usage:** Reference for KextInfo.plist structure and kernel extension development patterns  

**Repository:** https://github.com/Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh  
**Author:** Kaushal-snap  
**Purpose:** Reference EFI configuration for ASUS VivoBook K15 OLED K513EA Hackintosh  
**License:** MIT (see upstream repository)  
**Usage:** Reference configuration for DeviceProperties, boot arguments, and ACPI tables  

## Hardware-Specific References

- **Laptop:** ASUS VivoBook K15 OLED K513EA  
- **CPU:** Intel Core i5-1135G7 Tiger Lake-U  
- **GPU:** Intel Iris Xe 8086:9A49  
- **OS Target:** macOS Tahoe 26.7.1  

## Disclaimer

ASUSK513Controller is an independent project. It is not endorsed by or affiliated with ASUS, Apple, Intel, Acidanthera, or hieplpvip. All trademarks are the property of their respective owners.
