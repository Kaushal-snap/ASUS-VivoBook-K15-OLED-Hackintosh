# ASUSK513Controller Build Instructions

## Prerequisites

- macOS Tahoe 26.7.1 or later
- Xcode 16.0 or later
- macOS SDK 26.0
- Command Line Tools

## Build Configuration

| Configuration | SDK | Architecture | Purpose |
|--------------|-----|--------------|---------|
| DEBUG | macOS 26.0 | x86_64 | Development and testing |
| RELEASE | macOS 26.0 | x86_64 | Production (after verification) |

## Build Scripts

### DEBUG Build

```bash
cd /Users/mac/Documents/Default Project/ASUSK513Controller
./scripts/build-debug.sh
```

Produces: `build/Debug/ASUSK513Controller.kext`

### RELEASE Build

```bash
cd /Users/mac/Documents/Default Project/ASUSK513Controller
./scripts/build-release.sh
```

Produces: `build/Release/ASUSK513Controller.kext`

## Build Script Contents

### build-debug.sh

```bash
#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build/Debug"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

xcodebuild \
  -project "$PROJECT_DIR/ASUSK513Controller.xcodeproj" \
  -scheme ASUSK513Controller \
  -configuration Debug \
  -sdk macosx26.0 \
  -arch x86_64 \
  BUILD_DIR="$BUILD_DIR" \
  CONFIGURATION_BUILD_DIR="$BUILD_DIR" \
  ONLY_ACTIVE_ARCH=NO \
  ENABLE_BITCODE=NO \
  MACOSX_DEPLOYMENT_TARGET=26.0

echo "DEBUG build complete: $BUILD_DIR/ASUSK513Controller.kext"
```

### build-release.sh

```bash
#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build/Release"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

xcodebuild \
  -project "$PROJECT_DIR/ASUSK513Controller.xcodeproj" \
  -scheme ASUSK513Controller \
  -configuration Release \
  -sdk macosx26.0 \
  -arch x86_64 \
  BUILD_DIR="$BUILD_DIR" \
  CONFIGURATION_BUILD_DIR="$BUILD_DIR" \
  ONLY_ACTIVE_ARCH=NO \
  ENABLE_BITCODE=NO \
  MACOSX_DEPLOYMENT_TARGET=26.0

echo "RELEASE build complete: $BUILD_DIR/ASUSK513Controller.kext"
```

## Deployment (After Verification Only)

1. Build DEBUG version
2. Copy `ASUSK513Controller.kext` to `/Volumes/1401/EFI/OC/Kexts/`
3. Add entry to `config.plist` Kernel → Add
4. Boot macOS and verify loading with `kextstat`
5. Check diagnostic output in Console.app
6. If successful, build RELEASE version
7. Only then consider enabling in production EFI

## Verification Checklist

- [ ] DEBUG build compiles without errors
- [ ] DEBUG build loads without kernel panics
- [ ] Diagnostic output shows correct device detection
- [ ] Keyboard backlight responds to Fn+F7
- [ ] Display brightness responds to Fn+F4/F5
- [ ] No kernel panics during sleep/wake
- [ ] RELEASE build compiles without errors
- [ ] RELEASE build passes all TEST-MATRIX.md tests
