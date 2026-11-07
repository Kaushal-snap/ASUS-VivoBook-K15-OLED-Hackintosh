#!/bin/bash
# =============================================================================
# ASUSK513Controller - Debug Build Script
# =============================================================================
# Builds DEBUG version of ASUSK513Controller.kext
# Does NOT install or enable in production EFI
# =============================================================================

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build/Debug"

echo "========================================================================"
echo "ASUSK513Controller - DEBUG Build"
echo "========================================================================"
echo ""

# Verify Xcode is available
if ! command -v xcodebuild &> /dev/null; then
    echo "ERROR: xcodebuild not found. Install Xcode from the App Store."
    exit 1
fi

# Verify macOS SDK
SDK_PATH=$(xcodebuild -showsdks 2>/dev/null | grep "macosx26" | head -1 || true)
if [ -z "$SDK_PATH" ]; then
    echo "WARNING: macOS 26.0 SDK not found. Build may fail."
    echo "Available SDKs:"
    xcodebuild -showsdks 2>/dev/null || true
fi

# Create build directory
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "Building DEBUG configuration..."
echo "  Project: $PROJECT_DIR/ASUSK513Controller.xcodeproj"
echo "  Build dir: $BUILD_DIR"
echo ""

# Build
xcodebuild \
    -project "$PROJECT_DIR/ASUSK513Controller.xcodeproj" \
    -scheme ASUSK513Controller \
    -configuration Debug \
    -sdk macosx26.5 \
    -arch x86_64 \
    BUILD_DIR="$BUILD_DIR" \
    CONFIGURATION_BUILD_DIR="$BUILD_DIR" \
    ONLY_ACTIVE_ARCH=NO \
    ENABLE_BITCODE=NO \
    MACOSX_DEPLOYMENT_TARGET=26.0 \
    GCC_PREPROCESSOR_DEFINITIONS='DEBUG=1' \
    GCC_SYMBOLS_PRIVATE_EXTERN=NO \
    GCC_DEBUGGING_SYMBOLS=full

echo ""
echo "========================================================================"
echo "DEBUG Build Complete"
echo "========================================================================"
echo "Kext location: $BUILD_DIR/ASUSK513Controller.kext"
echo ""
echo "IMPORTANT: Do not enable in production EFI until verified."
echo "           Run TEST-MATRIX.md tests first."
echo "========================================================================"
