#!/usr/bin/env bash
# new-fix.sh — scaffold a new fix folder from the templates.
#
# Usage: scripts/new-fix.sh <Category> <slug> "<Title>"
#   e.g. scripts/new-fix.sh AirDrop ax201-airdrop "Improve AX201 AirDrop compatibility"
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT"

if [ "$#" -ne 3 ]; then
  echo "usage: $0 <Category> <slug> \"<Title>\"" >&2
  echo "categories: Graphics Audio WiFi-Bluetooth AirDrop Battery Keyboard ACPI Boot" >&2
  exit 2
fi

CATEGORY="$1"
SLUG="$2"
TITLE="$3"
DIR="Fixes/$CATEGORY/$SLUG"

if [ -e "$DIR" ]; then
  echo "error: $DIR already exists" >&2
  exit 1
fi

mkdir -p "$DIR"
subst() { sed -e "s|{{TITLE}}|$TITLE|g" -e "s|{{CATEGORY}}|$CATEGORY|g" -e "s|{{SLUG}}|$SLUG|g" "$1"; }
subst templates/FIX_TEMPLATE.md > "$DIR/README.md"
subst templates/fix.json      > "$DIR/fix.json"

echo "Created $DIR"
echo "Next:"
echo "  1. Edit $DIR/README.md and $DIR/fix.json"
echo "  2. scripts/publish-fix.sh $DIR --dry-run"
echo "  3. scripts/publish-fix.sh $DIR"
