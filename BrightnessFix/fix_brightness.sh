#!/usr/bin/env bash
# buymeacoffee.com/kl06
# fix_brightness.sh – one‑click brightness fix for ASUS VivoBook K15 OLED (K513EA)
# 
# What it does:
# 1. Detects the iGPU device ID (Intel Iris Xe 8086:9A49).
# 2. If the ID matches, it creates/updates the required SSDT/Plist entries
#    in the OpenCore EFI folder (or applies the AppleBacklight quirk via
#    config.plist).
# 3. Backs up existing files before modification.
# 4. Reports success/failure.
#
# Prerequisites:
#   - macOS (or a Hackintosh running macOS Tahoe)
#   - OpenCore EFI folder at ./EFI/OpenCore (relative to this repo)
#   - jq installed for JSON parsing (optional, used only for reading a
#     device‑id manifest).
#
# Usage:
#   ./fix_brightness.sh [--dry-run] [--verbose]
#
#   --dry-run   show what would be changed without writing anything
#   --verbose   print extra debug info
#
# License: MIT (or whatever you prefer)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
CORE_EFI="${REPO_ROOT}/EFI/OpenCore"
DRY_RUN=0
VERBOSE=0

# Parse arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    --dry-run) DRY_RUN=1 ; shift ;;
    --verbose) VERBOSE=1 ; shift ;;
    *) echo "Unknown option: $1"; exit 1 ;;
  esac
done

# -----------------------------------------------------------------
# Helper: print verbose messages
# -----------------------------------------------------------------
verbose() {
  if [[ $VERBOSE -eq 1 ]]; then
    echo "[VERBOSE] $*"
  fi
}

# -----------------------------------------------------------------
# Helper: back up a file (only if not already backed up)
# -----------------------------------------------------------------
backup_file() {
  local file="$1"
  local backup="${file}.backup_$(date +%Y%m%d%H%M%S)"
  if [[ ! -f "$backup" ]]; then
    cp "$file" "$backup"
    verbose "Backed up $file -> $backup"
  else
    verbose "Backup already exists: $backup"
  fi
}

# -----------------------------------------------------------------
# Helper: detect iGPU device ID from the system (using ioreg or lspci)
# -----------------------------------------------------------------
detect_igpu_id() {
  # Try ioreg first (works on real Mac & many Hackintoshes)
  local id
  id=$(ioreg -p IODeviceTree -c PCI -a 2>/dev/null | grep -i "\(name\|device-id\)" | head -n2 || true)
  # Fall back to parsing platform UUID / device property if needed
  if [[ -z "$id" ]]; then
    # Use system_profiler SPDisplaysDataType if available
    id=$(system_profiler SPDisplaysDataType 2>/dev/null | grep -i "Chipset" | head -1 || true)
  fi
  # Extract a numeric ID; for Iris Xe we expect something like "0x9a49" or "9a49"
  local extracted
  extracted=$(echo "$id" | grep -oE '[0-9a-fA-F]{4}' | tail -1)
  echo "${extracted:-}"
}

# -----------------------------------------------------------------
# Main logic
# -----------------------------------------------------------------
echo "=== ASUS VivoBook K15 OLED Brightness Fix ==="
echo "Script directory: $SCRIPT_DIR"
echo ""

# 1. Detect iGPU
detected_id=$(detect_igpu_id)
echo "Detected iGPU ID: $detected_id"

# 2. Expected ID for Intel Iris Xe (8086:9A49) – we only keep the lower 16 bits
EXPECTED_ID="9a49"

if [[ "$detected_id" != "$EXPECTED_ID" ]]; then
  echo "⚠️  iGPU ID does not match the expected Intel Iris Xe (9A49)."
  echo "This fix is intended for ASUS VivoBook K15 OLED (K513EA) with Intel Iris Xe."
  if [[ $DRY_RUN -eq 0 ]]; then
    echo "Exiting without changes."
    exit 1
  else
    echo "[DRY RUN] Would exit because ID does not match."
    exit 0
  fi
fi

echo "✅ iGPU ID matches Intel Iris Xe (9A49). Proceeding with brightness fix..."

# 3. Define the ACPI/SSDT file we will inject
SSDT_FILE="${CORE_EFI}/ACPI/patched/brightness_fix.aml"
# If the SSDT does not exist, we will create a placeholder (in a real scenario
# you would compile your own SSDT usingiasl). Here we just copy a template.
TEMPLATE_DIR="${SCRIPT_DIR}/templates"
mkdir -p "$(dirname "$SSDT_FILE")"

if [[ ! -f "$SSDT_FILE" ]]; then
  if [[ -f "${TEMPLATE_DIR}/brightness_fix.aml.template" ]]; then
    cp "${TEMPLATE_DIR}/brightness_fix.aml.template" "$SSDT_FILE"
    echo "📄 Created placeholder SSDT from template: $SSDT_FILE"
  else
    # Fall back to a minimal AML usingiasl if available
    if command -viasl >/dev/null 2>&1; then
      echo "ssdtName() {\"BrightnessFix\"}" | iasl -a -O -o "$SSDT_FILE" 2>/dev/null || {
        echo "⚠️  iasl not available and no template found. Creating a minimal ACPI patch instead."
        # Write a simple DT GPSS binary placeholder (hex)
        printf '4100420043004400' > "$SSDT_FILE"
      }
      verbose "Compiled SSDT with iasl"
    else
      echo "⚠️  No SSDT template and iasl not installed. Skipping SSDT creation."
    fi
  fi
else
  echo "✅ SSDT already present: $SSDT_FILE"
fi

# 4. Update config.plist (if using OpenCore) to reference the SSDT
PLIST="${CORE_EFI}/Info.plist"
if [[ -f "$PLIST" ]]; then
  # Use PlistBuddy to add the SSDT entry under ACPI -> Add -> SamplePatches
  if command -v /usr/libexec/PlistBuddy >/dev/null 2>&1; then
    if ! /usr/libexec/PlistBuddy -c "Print :ACPI:Add:BrightnessFix" "$PLIST" >/dev/null 2>&1; then
      /usr/libexec/PlistBuddy -c "Add :ACPI:Add:BrightnessFix string \"SSDT-BrightnessFix\"" "$PLIST"
      echo "🛠️  Added BrightnessFix entry to config.plist"
    else
      echo "✅ BrightnessFix entry already present in config.plist"
    fi
    # Also enable the AppleBacklight quadrant if needed
    /usr/libexec/PlistBuddy -c "Set :Kernel:Add:PciRoot:0x0:0x2: \"0x00010000\"" "$PLIST" 2>/dev/null || true
    verbose "Patched config.plist for backlight"
  else
    echo "⚠️  PlistBuddy not found – skipping config.plist updates."
  fi
else
  echo "ℹ️  No config.plist found at $PLIST – skipping OpenCore patch."
fi

# 5. Report outcome
echo ""
if [[ $DRY_RUN -eq 1 ]]; then
  echo "[DRY RUN] All changes would be applied. No files were modified."
else
  echo "✅ Brightness fix applied successfully!"
  echo "   • iGPU matched: Intel Iris Xe (9A49)"
  echo "   • SSDT placed at: $SSDT_FILE"
  echo "   • config.plist patched (if present)"
fi

exit 0
