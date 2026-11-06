#!/usr/bin/env bash
# buymeacoffee.com/kl06
# scan-sensitive.sh — refuse to publish secrets or personal identifiers.
#
# Usage: scripts/scan-sensitive.sh <file> [file ...]
# Exit:  0 = clean | 1 = findings (do NOT publish) | 2 = usage error
set -u

if [ "$#" -eq 0 ]; then
  echo "usage: $0 <file> [file ...]" >&2
  exit 2
fi

# SMBIOS / platform identifiers — must never leave the machine.
ID_PATTERNS='<key>(SystemSerialNumber|BoardSerialNumber|MLB|ROM|SystemUUID|SmUUID|PlatformUUID|HardwareUUID|SerialNumber)</key>'
ID_PATTERNS="$ID_PATTERNS"'|Serial Number:|SerialNumber=|SystemSerial'

# Credentials and API tokens.
SECRET_PATTERNS='gho_[A-Za-z0-9]{20,}|ghp_[A-Za-z0-9]{20,}|ghs_[A-Za-z0-9]{20,}|ghr_[A-Za-z0-9]{20,}'
SECRET_PATTERNS="$SECRET_PATTERNS"'|github_pat_[A-Za-z0-9_]{10,}|AKIA[0-9A-Z]{16}|-----BEGIN [A-Z ]*PRIVATE KEY-----'
SECRET_PATTERNS="$SECRET_PATTERNS"'|xox[baprs]-[A-Za-z0-9-]{5,}|sk-[A-Za-z0-9]{20,}|AIza[0-9A-Za-z_-]{20,}'

# Personal Windows user profile paths.
WIN_PATTERNS='[A-Za-z]:\\+Users\\+[A-Za-z0-9._-]+'

# Generic password assignments.
PW_PATTERNS='(password|passwd|secret|api[_-]?key)[[:space:]]*[=:][[:space:]]*[^[:space:]]'

# Resolve this script's own path so it never scans itself: its pattern list
# literally contains the strings it hunts for.
SELF="$(cd "$(dirname "$0")" && pwd)/$(basename "$0")"

findings=0

report() {
  # $1 = label, $2 = ERE pattern, $3 = file
  out=$(grep -IEn "$2" "$3" 2>/dev/null | head -3)
  if [ -n "$out" ]; then
    echo "  [!] $1 in $3"
    printf '%s\n' "$out" | sed 's/^/      /'
    findings=1
  fi
}

for f in "$@"; do
  [ -f "$f" ] || continue
  [ "$(cd "$(dirname "$f")" && pwd)/$(basename "$f")" = "$SELF" ] && continue
  report "serial/SMBIOS identifier" "$ID_PATTERNS" "$f"
  report "credential/token"          "$SECRET_PATTERNS" "$f"
  report "Windows user path"         "$WIN_PATTERNS" "$f"
  report "password assignment"       "$PW_PATTERNS" "$f"
done

if [ "$findings" -ne 0 ]; then
  echo "RESULT: sensitive data detected — do not publish." >&2
  exit 1
fi
echo "RESULT: clean — no sensitive data detected."
exit 0

