# Fixes

One folder per issue. Each folder contains:

- `README.md` — the fix document (all required sections, see
  [`templates/FIX_TEMPLATE.md`](../templates/FIX_TEMPLATE.md)).
- `fix.json` — the manifest consumed by `scripts/publish-fix.sh`
  (title, category, status, files, credits).
- Any fix assets that are safe to redistribute.

Categories:

| Folder | Covers |
|---|---|
| [`Graphics/`](Graphics/) | Iris Xe / NMX / framebuffer / backlight configuration |
| [`Audio/`](Audio/) | Realtek ALC0256 / AppleALC layouts |
| [`WiFi-Bluetooth/`](WiFi-Bluetooth/) | Intel AX201 Wi-Fi and Bluetooth |
| [`AirDrop/`](AirDrop/) | AWDL / AirDrop / AirPlay continuity |
| [`Battery/`](Battery/) | ACPI battery and power management |
| [`Keyboard/`](Keyboard/) | Keyboard and keyboard backlight |
| [`ACPI/`](ACPI/) | SSDT source, patches and device handling |
| [`Boot/`](Boot/) | OpenCore boot configuration |

## Adding a fix

```sh
scripts/new-fix.sh AirDrop ax201-airdrop "Improve AX201 AirDrop compatibility"
# edit Fixes/AirDrop/ax201-airdrop/README.md and fix.json
scripts/publish-fix.sh Fixes/AirDrop/ax201-airdrop --dry-run   # preview
scripts/publish-fix.sh Fixes/AirDrop/ax201-airdrop             # commit + push
```

See [`docs/PUBLISHING.md`](../docs/PUBLISHING.md) for the full workflow.
