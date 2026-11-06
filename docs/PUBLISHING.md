# Publishing workflow

Selective, sanitized, attributed publishing for fixes to this repo.

## Principles (from `AGENTS.md`)

1. **Never publish the whole working tree.** Only files tied to the completed issue.
2. **Never publish secrets or identifiers.** SMBIOS serials, MLB, ROM, UUIDs, tokens,
   passwords and personal Windows usernames are blocked by an automated scan.
3. **Always attribute.** Every adopted / modified / referenced source is recorded in
   `CREDITS.md` from the fix manifest. Credits are never invented.
4. **Never force-push, never discard unrelated work.**
5. **Never claim a fix is verified when it is not.**

## Verification statuses

`Implemented` → `Build Verified` → `Boot Verified` → `Hardware Tested` →
`Confirmed Working`. Anything unverified is `Experimental`; anything reverted is
`Failed / Reverted`. Only `Confirmed Working` means it passed on real hardware.

## Layout

```
Fixes/<Category>/<slug>/
  README.md     # the fix document (problem → root cause → solution → credits → testing)
  fix.json      # machine-readable manifest
  <assets>      # only redistributable files
```

Categories: `Graphics`, `Audio`, `WiFi-Bluetooth`, `AirDrop`, `Battery`, `Keyboard`,
`ACPI`, `Boot`.

## Steps

```sh
# 1. scaffold
scripts/new-fix.sh AirDrop ax201-airdrop "Improve AX201 AirDrop compatibility"

# 2. fill in README.md (document) and fix.json (manifest)

# 3. preview: scan + show exactly what would be staged (nothing is committed)
scripts/publish-fix.sh Fixes/AirDrop/ax201-airdrop --dry-run

# 4. publish: scan, update CHANGELOG.md + CREDITS.md, commit, push
scripts/publish-fix.sh Fixes/AirDrop/ax201-airdrop
```

Build a fix but do not publish yet:

```sh
scripts/publish-fix.sh Fixes/AirDrop/ax201-airdrop --no-push
```

## What `publish-fix.sh` does, in order

1. Reads `fix.json`; validates required fields.
2. Builds the publish set = manifest `files` + everything under the fix folder +
   `CHANGELOG.md` + `CREDITS.md`.
3. Runs `scripts/scan-sensitive.sh` over the set — **hard abort** on any finding.
4. Inserts an entry under `## [Unreleased]` in `CHANGELOG.md`.
5. Appends new credits to `CREDITS.md` (de-duplicated by URL).
6. `git add` **only** those files, then commits
   `fix(<scope>): <title>` (type/scope come from the manifest).
7. `git push origin main` (never `--force`). On failure the commit is kept and the
   failure is reported.

## Manifest reference (`fix.json`)

```json
{
  "title": "Improve AX201 AirDrop compatibility",
  "category": "AirDrop",
  "status": "Hardware Tested",
  "commit_type": "fix",
  "commit_scope": "airdrop",
  "summary": "One-line summary for the changelog.",
  "files": ["config/example.plist"],
  "credits": [
    {
      "repo": "OpenIntelWireless/itlwm",
      "url": "https://github.com/OpenIntelWireless/itlwm",
      "author": "zxystd",
      "commit": "abcdef1",
      "license": "GPL-2.0",
      "usage": "modified upstream code",
      "files": ["AirportItlwm.kext/Contents/Info.plist"]
    }
  ]
}
```

`usage` should read like one of: `directly adopted upstream code`,
`modified upstream code`, `community patch`, `independent implementation`,
`inspired by <repo>`, `experimental fix`.

## Privacy scan

```sh
scripts/scan-sensitive.sh path/to/file [more files]
```

Blocks serial/SMBIOS keys, GitHub/API tokens, private keys, Windows user paths and
password assignments. Exit `0` = clean, `1` = findings.

## Git identity

Commits in this repo use the repository's existing author identity
(`Kaushal-snap <kl682002@gmail.com>`). Authentication uses the GitHub CLI at
`$HOME/bin/gh` via `gh auth setup-git`.
