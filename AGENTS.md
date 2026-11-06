# AGENTS.md — ASUS VivoBook K15 OLED Hackintosh Project Rules

> Permanent project-level rules. These apply automatically to every OpenCode session
> in this repository. Treat them as binding constraints: never sacrifice existing
> working functionality for unrelated fixes.

## 1. Project Hardware

- Laptop: ASUS VivoBook K15 OLED K513EA
- CPU: Intel Core i5-1135G7
- GPU: Intel Iris Xe 8086:9A49
- Wi-Fi: Intel AX201
- Audio: Realtek ALC0256
- OS: macOS Tahoe 26.7.1
- OpenCore EFI: 1401 (OC/1401-Mac)

## 2. Automatic Bug-Fix Workflow

For every reported issue:

1. Inspect the existing configuration and relevant files.
2. Identify the root cause.
3. Search relevant upstream repositories, documentation, commits and patches.
4. Determine whether the solution comes from upstream code, community contributions,
   independent modifications, or original development.
5. Implement the fix.
6. Validate all affected components.
7. Identify exactly which files belong to the fix.
8. Generate documentation and attribution.
9. Commit and publish the completed fix to the GitHub repository.
10. Report the published commit and files.

Do not ask the user to manually prepare changelogs, credits or file lists.

## 3. Upstream Repository Attribution

Always investigate these repositories when relevant:

- https://github.com/Kaushal-snap/ASUS-VivoBook-K15-OLED-Hackintosh
- https://github.com/zep245/IrisXe-macos-drive
- https://github.com/kgp-macPro
- https://github.com/dortania
- https://github.com/nullmoth

Also investigate other relevant repositories discovered during troubleshooting.

For every implemented fix, record:

- Original repository name.
- Original repository URL.
- Original author or contributor.
- Original commit hash, if identifiable.
- Relevant source files.
- License information.
- Exact modifications adopted.
- Additional modifications made for this laptop.

Never invent credits or authorship. Distinguish clearly between:

- Directly adopted upstream code.
- Modified upstream code.
- Community patches.
- Independent implementations.
- Experimental fixes.

If a fix is only inspired by another repository, describe it as **inspired by** that work,
not copied from it.

Respect the original repository license. Never republish third-party binaries, source
files or complete repositories without checking redistribution permissions. Link to
upstream assets when redistribution is not permitted.

## 4. Selective File Publishing

Never upload the entire working directory after every fix. Identify only the files
directly related to the completed issue.

- Graphics fix: relevant graphics configuration, required patches/scripts, graphics docs.
- Audio fix: relevant AppleALC configuration, required audio layout information, audio docs.
- AirDrop fix: relevant Wi-Fi/Bluetooth configuration, required patches, AirDrop docs.
- ACPI fix: relevant SSDT source, compiled AML if redistribution is permitted, build/install instructions.

Do not upload unrelated EFI, kexts, binaries, logs or personal files.

## 5. GitHub Documentation

Automatically maintain:

- `CHANGELOG.md`
- `CREDITS.md`
- `README.md`

Create a separate fix documentation file under `Fixes/`:

```
Fixes/
  Graphics/
  Audio/
  WiFi-Bluetooth/
  AirDrop/
  Battery/
  Keyboard/
  ACPI/
  Boot/
```

Every fix document must include:

- Problem description.
- Hardware affected.
- Root cause.
- Solution.
- Files included.
- Installation instructions.
- Source repository and contributor credits.
- License and redistribution notes.
- Testing procedure.
- Actual verification status.

## 6. Testing and Verification

Never mark an untested fix as confirmed working. Use these statuses:

- Implemented
- Build Verified
- Boot Verified
- Hardware Tested
- Confirmed Working
- Experimental
- Failed / Reverted

Only publish a fix as **Confirmed Working** after actual hardware verification.

## 7. Automatic Git Operations

After a completed and validated fix:

- Check `git status` and existing user changes.
- Stage only relevant files.
- Create a descriptive commit.
- Push to the configured GitHub repository.
- Never force-push.
- Never overwrite unrelated work.
- Never silently discard user changes.
- If authentication or network fails, retain the local commit and report the issue.

Use descriptive commit messages, for example:

```
fix(audio): enable Realtek ALC0256 output
fix(airdrop): improve AX201 AirDrop compatibility
fix(acpi): correct battery device handling
docs: add verified keyboard backlight fix
```

## 8. Security and Privacy

Before publishing, automatically scan for and never publish:

- SMBIOS serial numbers
- MLB
- ROM
- System UUIDs
- Passwords and tokens
- Personal Windows usernames
- Private diagnostic information

Keep the private working EFI separate from the sanitized public EFI.

## 9. Preserve Working Configuration

Current working graphics:

- 1920x1080
- 900p
- 810p
- 720p

Never casually modify:

- NMX
- NullMoth
- GPU DeviceProperties
- Framebuffer configuration
- Working graphics boot arguments

The keyboard backlight SSDT is **experimental** until verified.

Do not introduce unnecessary changes to unrelated working components.

## 10. Final Automation Report

After every published fix, report:

1. Issue fixed.
2. Root cause.
3. Solution implemented.
4. Files uploaded.
5. Upstream repositories used.
6. Contributor credits added.
7. Verification status.
8. Git commit hash.
9. GitHub push result.
10. Public documentation location.

buymeacoffee.com/kl06

## Final Rule

Build a reliable, properly credited, community-useful ASUS VivoBook K15 OLED Hackintosh
knowledge base. Work independently using terminal access, Git and available GitHub
authentication. Automate diagnosis, fixes, validation, documentation, attribution and
selective publishing. Never sacrifice existing working functionality for unrelated fixes.

