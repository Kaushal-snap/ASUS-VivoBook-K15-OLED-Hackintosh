#!/usr/bin/env bash
# buymeacoffee.com/kl06
# publish-fix.sh — selective, sanitized, attributed publishing workflow.
#
# Usage:
#   scripts/publish-fix.sh Fixes/<Category>/<slug> [--dry-run] [--no-push]
#
# Reads Fixes/<Category>/<slug>/fix.json (manifest) and .../README.md (fix document).
# Stages ONLY the files named in the manifest plus the fix document, CHANGELOG.md and
# CREDITS.md. Scans every candidate file for secrets first and aborts if any are found.
# Never force-pushes. On push failure the local commit is retained and reported.
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
GH_BIN="${GH_BIN:-$HOME/bin/gh}"
cd "$REPO_ROOT"

FIXDIR=""
DRY_RUN=0
NO_PUSH=0
for arg in "$@"; do
  case "$arg" in
    --dry-run) DRY_RUN=1 ;;
    --no-push) NO_PUSH=1 ;;
    -h|--help) sed -n '2,10p' "$0"; exit 0 ;;
    -*) echo "error: unknown option $arg" >&2; exit 2 ;;
    *) FIXDIR="$arg" ;;
  esac
done

[ -n "$FIXDIR" ] || { echo "usage: $0 Fixes/<Category>/<slug> [--dry-run] [--no-push]" >&2; exit 2; }
[ -d "$FIXDIR" ] || { echo "error: fix directory not found: $FIXDIR" >&2; exit 2; }
MANIFEST="$FIXDIR/fix.json"
DOC="$FIXDIR/README.md"
[ -f "$MANIFEST" ] || { echo "error: missing manifest $MANIFEST" >&2; exit 2; }
[ -f "$DOC" ] || { echo "error: missing fix document $DOC" >&2; exit 2; }
command -v jq >/dev/null || { echo "error: jq is required" >&2; exit 2; }

title=$(jq -r '.title // empty' "$MANIFEST")
category=$(jq -r '.category // empty' "$MANIFEST")
status=$(jq -r '.status // "Implemented"' "$MANIFEST")
commit_type=$(jq -r '.commit_type // "fix"' "$MANIFEST")
commit_scope=$(jq -r '.commit_scope // empty' "$MANIFEST")
summary=$(jq -r '.summary // empty' "$MANIFEST")
[ -n "$title" ] || { echo "error: manifest missing non-empty .title" >&2; exit 2; }
[ -n "$category" ] || { echo "error: manifest missing non-empty .category" >&2; exit 2; }

# ── collect the exact files to publish ────────────────────────────────────
files=()
while IFS= read -r f; do
  [ -n "$f" ] || continue
  if [ ! -e "$f" ]; then
    echo "error: manifest file not found: $f" >&2
    exit 2
  fi
  files+=("$f")
done < <(jq -r '.files[]? // empty' "$MANIFEST")

while IFS= read -r f; do files+=("$f"); done < <(find "$FIXDIR" -type f | sort)
files+=("CHANGELOG.md" "CREDITS.md")

# de-duplicate
deduped=()
for f in "${files[@]}"; do
  seen=0
  for d in "${deduped[@]:-}"; do [ "$d" = "$f" ] && seen=1 && break; done
  [ "$seen" -eq 0 ] && deduped+=("$f")
done
files=("${deduped[@]}")

echo "== files selected for publication (${#files[@]}) =="
printf '  %s\n' "${files[@]}"

# ── secret scan (hard gate) ───────────────────────────────────────────────
echo "== scanning for sensitive data =="
if ! "$REPO_ROOT/scripts/scan-sensitive.sh" "${files[@]}"; then
  echo "ABORTED: sensitive data detected. Nothing staged, committed or pushed." >&2
  exit 3
fi

# ── update CHANGELOG.md ───────────────────────────────────────────────────
if [ "$DRY_RUN" -eq 1 ]; then
  echo "== DRY RUN: CHANGELOG.md not modified =="
else
  entry="- **${category}** — ${title} — _${status}_"
  [ -n "$summary" ] && entry="${entry}: ${summary}"
  entry="${entry} ([details](${FIXDIR}/README.md))"

  tmp=$(mktemp)
  if grep -q '^## \[Unreleased\]' CHANGELOG.md; then
    awk -v e="$entry" '{ print } /^## \[Unreleased\]/ && !done { print ""; print e; done=1 }' CHANGELOG.md > "$tmp"
  else
    { printf '# Changelog\n\n## [Unreleased]\n\n%s\n\n' "$entry"; cat CHANGELOG.md; } > "$tmp"
  fi
  mv "$tmp" CHANGELOG.md
  echo "== CHANGELOG.md updated =="
fi

# ── update CREDITS.md ─────────────────────────────────────────────────────
if [ "$DRY_RUN" -eq 1 ]; then
  echo "== DRY RUN: CREDITS.md not modified =="
else
ncredits=$(jq -r '(.credits // []) | length' "$MANIFEST")
i=0
while [ "$i" -lt "$ncredits" ]; do
  repo=$(jq -r ".credits[$i].repo // empty" "$MANIFEST")
  url=$(jq -r ".credits[$i].url // empty" "$MANIFEST")
  author=$(jq -r ".credits[$i].author // empty" "$MANIFEST")
  csha=$(jq -r ".credits[$i].commit // empty" "$MANIFEST")
  lic=$(jq -r ".credits[$i].license // empty" "$MANIFEST")
  usage=$(jq -r ".credits[$i].usage // empty" "$MANIFEST")
  srcf=$(jq -r '(.credits['"$i"'].files // []) | join(", ")' "$MANIFEST")
  i=$((i + 1))
  [ -n "$repo" ] || continue
  line="- **${repo}**"
  [ -n "$url" ] && line="${line} ([source](${url}))"
  [ -n "$usage" ] && line="${line} — ${usage}"
  [ -n "$author" ] && line="${line}. Contributor: ${author}"
  [ -n "$csha" ] && line="${line} (commit \`${csha}\`)"
  [ -n "$lic" ] && line="${line}. License: ${lic}"
  [ -n "$srcf" ] && line="${line}. Source files: ${srcf}"
  line="${line}."
  if [ -n "$url" ] && grep -qF "$url" CREDITS.md; then
    echo "   credit already present, skipping: $repo"
  else
    printf '%s\n' "$line" >> CREDITS.md
    echo "   credit added: $repo"
  fi
done
fi

# ── commit ────────────────────────────────────────────────────────────────
msg="${commit_type}"
[ -n "$commit_scope" ] && msg="${msg}(${commit_scope})"
msg="${msg}: ${title}"

git add -- "${files[@]}"
echo "== staged =="
git diff --cached --name-only

if [ "$DRY_RUN" -eq 1 ]; then
  echo "== DRY RUN: un-staging; not committing or pushing =="
  git reset -q
  exit 0
fi

git commit -m "$msg"
sha=$(git rev-parse --short HEAD)
echo "commit: $sha"

if [ "$NO_PUSH" -eq 1 ]; then
  echo "== --no-push: stopping after commit $sha =="
  exit 0
fi

if git push origin main; then
  echo "pushed: $sha"
else
  echo "PUSH FAILED — local commit $sha retained; fix it and run: git push origin main" >&2
  exit 4
fi

