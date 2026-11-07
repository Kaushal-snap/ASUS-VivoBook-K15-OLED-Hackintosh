# Credits and attribution

This file records the upstream and community work used, adopted, modified or referenced
by fixes published in this repository. It is maintained automatically by
`scripts/publish-fix.sh` from each fix's `fix.json` manifest.

Attribution categories used throughout this repository:

- **Directly adopted** — code/configuration used unchanged from upstream.
- **Modified upstream** — upstream work changed for this laptop.
- **Community patch** — a patch published by another contributor.
- **Independent implementation** — original work for this machine.
- **Experimental** — not yet verified on hardware.
- **Inspired by** — approach informed by another project; not copied.

No credit in this file is invented. If a source is unknown, it is left unlisted rather
than guessed.

## Project

- **Kaushal-snap** — Repository owner, hardware owner and tester for the ASUS VivoBook
  K15 OLED (K513EA) Tiger Lake-U Hackintosh.

## Upstream and community sources

<!-- entries appended automatically by scripts/publish-fix.sh -->


[Buy Me a Coffee](https://buymeacoffee.com/KL06)
- **acidanthera/BrightnessKeys** ([source](https://github.com/acidanthera/BrightnessKeys)) — Reference for brightness hotkey event handling architecture. Contributor: vit9696 (Acidanthera) (commit `BrightnessKeys.kext 1.0.4 reference`). License: Check upstream repository.
- **Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh** ([source](https://github.com/Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh)) — Reference config.plist, DeviceProperties, and ACPI tables for K513EA. Contributor: Kaushal-snap (commit `reference EFI configuration`). License: MIT.
- **hieplpvip/AsusSMC** ([source](https://github.com/hieplpvip/AsusSMC)) — Reference for ACPI ATK device handling and keyboard backlight control. Contributor: hieplpvip (commit `AsusSMC VirtualSMC plugin reference`). License: MIT.
