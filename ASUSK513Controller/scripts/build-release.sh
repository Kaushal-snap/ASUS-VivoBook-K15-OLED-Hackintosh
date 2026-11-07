#!/bin/bash
# =============================================================================
# ASUSK513Controller - Release Build Script
# =============================================================================
# Builds RELEASE version of ASUSK513Controller.kext
# Only run after DEBUG testing is complete and verified
# =============================================================================

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build/Release"

echo "========================================================================"
echo "ASUSK513Controller - RELEASE Build"
echo "========================================================================"
echo ""

# Verify DEBUG was built first
DEBUG_KEXT="$PROJECT_DIR/build/Debug/ASUSK513Controller.kext"
if [ ! -d "$DEBUG_KEXT" ]; then
    echo "ERROR: DEBUG build not found at $DEBUG_KEXT"
    echo "Build DEBUG version first: ./scripts/build-debug.sh"
    exit 1
fi

echo "DEBUG build found. Proceeding with RELEASE build..."
echo ""

# Verify Xcode is available
if ! command -v xcodebuild &> /dev/null; then
    echo "ERROR: xcodebuild not found. Install Xcode from the App Store."
    exit 1
fi

# Create build directory
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "Building RELEASE configuration..."
echo "  Project: $PROJECT_DIR/ASUSK513Controller.xcodeproj"
echo "  Build dir: $BUILD_DIR"
echo ""

# Build
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
    MACOSX_DEPLOYMENT_TARGET=26.0 \
    GCC_DEBUGGING_SYMBOLS=no \
    GCC_OPTIMIZATION_LEVEL=s

echo ""
echo "========================================================================"
echo "RELEASE Build Complete"
echo "========================================================================"
echo "Kext location: $BUILD_DIR/ASUSK513Controller.kext"
echo ""
echo "WARNING: Do not enable in production EFI until verified."
echo "         Run TEST-MATRIX.md tests first."
echo "========================================================================"
