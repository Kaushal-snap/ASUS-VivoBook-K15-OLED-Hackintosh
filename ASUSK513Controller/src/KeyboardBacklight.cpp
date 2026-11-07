//
//  KeyboardBacklight.cpp
//  ASUSK513Controller
//
//  Keyboard backlight control for ASUS VivoBook K15 OLED K513EA.
//  Only activates after verifying ACPI _QD7/_QD8 methods and SLKB/GLKB devices.
//  Does NOT control display brightness - that is separate.
//

#include "KeyboardBacklight.h"
#include "ASUSK513ControllerDefs.h"
#include <libkern/c++/OSBoolean.h>

#define super IOService
OSDefineMetaClassAndStructors(KeyboardBacklight, IOService)

bool KeyboardBacklight::init(OSDictionary *dictionary)
{
    if (!super::init(dictionary)) {
        return false;
    }

    fACPIProvider = nullptr;
    fCurrentLevel = kKeyboardBacklightDefault;
    fBacklightSupported = false;
    fHardwareVerified = false;

    return true;
}

void KeyboardBacklight::free(void)
{
    if (fACPIProvider) {
        fACPIProvider->release();
        fACPIProvider = nullptr;
    }

    super::free();
}

IOService *KeyboardBacklight::probe(IOService *provider, SInt32 *score)
{
    fACPIProvider = OSDynamicCast(IOACPIPlatformDevice, provider);
    if (!fACPIProvider) {
        return nullptr;
    }

    // Check for SLKB device (keyboard backlight)
    OSString *slkbStr = OSString::withCString(kACPI_SLKB);
    if (!slkbStr) {
        return nullptr;
    }

    OSObject *slkbObj = fACPIProvider->getProperty(slkbStr);
    slkbStr->release();

    if (!slkbObj) {
        // SLKB not found on this provider - may still work via GLKB
        // Don't reject outright, just lower score
        *score = 5000;
    } else {
        *score = 9000;
    }

    return this;
}

bool KeyboardBacklight::start(IOService *provider)
{
    if (!super::start(provider)) {
        return false;
    }

    // Verify hardware support before enabling backlight control
    fHardwareVerified = verifyHardwareSupport();

    if (!fHardwareVerified) {
        K513_LOG("Keyboard backlight hardware not verified - disabling backlight control");
        fBacklightSupported = false;
        return true; // Still start, but backlight control disabled
    }

    fBacklightSupported = true;
    fCurrentLevel = readBacklightLevel();

    K513_LOG("Keyboard backlight initialized at level %d", fCurrentLevel);
    return true;
}

void KeyboardBacklight::stop(IOService *provider)
{
    K513_LOG("Keyboard backlight stopping");
    super::stop(provider);
}

bool KeyboardBacklight::verifyHardwareSupport(void)
{
    if (!fACPIProvider) {
        return false;
    }

    // Check for SLKB device (keyboard backlight device)
    OSString *slkbStr = OSString::withCString(kACPI_SLKB);
    bool hasSLKB = false;
    if (slkbStr) {
        hasSLKB = fACPIProvider->getProperty(slkbStr) != nullptr;
        slkbStr->release();
    }

    // Check for GLKB method (get keyboard backlight level)
    OSString *glkbStr = OSString::withCString(kACPI_GLKB);
    bool hasGLKB = false;
    if (glkbStr) {
        hasGLKB = fACPIProvider->getProperty(glkbStr) != nullptr;
        glkbStr->release();
    }

    // Check for _QD7/_QD8 methods (keyboard backlight toggle)
    OSString *qd7Str = OSString::withCString(kACPI_QD7);
    bool hasQD7 = false;
    if (qd7Str) {
        hasQD7 = fACPIProvider->getProperty(qd7Str) != nullptr;
        qd7Str->release();
    }

    OSString *qd8Str = OSString::withCString(kACPI_QD8);
    bool hasQD8 = false;
    if (qd8Str) {
        hasQD8 = fACPIProvider->getProperty(qd8Str) != nullptr;
        qd8Str->release();
    }

    K513_LOG("Hardware verification: SLKB=%d GLKB=%d QD7=%d QD8=%d",
             hasSLKB, hasGLKB, hasQD7, hasQD8);

    // Require at least SLKB and one of QD7/QD8
    return hasSLKB && (hasQD7 || hasQD8);
}

bool KeyboardBacklight::readBacklightLevel(void)
{
    if (!fACPIProvider || !fHardwareVerified) {
        return kKeyboardBacklightDefault;
    }

    // Note: evaluateMethod API not available in this SDK version.
    // Reading backlight level requires proper ACPI method evaluation which
    // requires the correct API surface not available in this SDK.
    // For now, return default level.
    (void)fACPIProvider;
    (void)kACPI_GLKB;

    return kKeyboardBacklightDefault;
}

bool KeyboardBacklight::writeBacklightLevel(int level)
{
    if (!fACPIProvider || !fHardwareVerified) {
        return false;
    }

    if (level < kKeyboardBacklightMin || level > kKeyboardBacklightMax) {
        return false;
    }

    // In a real implementation, we'd write the level to the appropriate ACPI method
    // For now, just log the intended change
    K513_LOG("Setting keyboard backlight level to %d (not yet implemented)", level);

    fCurrentLevel = level;
    return true;
}

bool KeyboardBacklight::setBacklightLevel(int level)
{
    if (!fBacklightSupported) {
        return false;
    }

    return writeBacklightLevel(level);
}

int KeyboardBacklight::getBacklightLevel(void) const
{
    return fCurrentLevel;
}

bool KeyboardBacklight::toggleBacklight(void)
{
    if (!fBacklightSupported || !fHardwareVerified) {
        return false;
    }

    // Toggle between min and max
    int newLevel = (fCurrentLevel == kKeyboardBacklightMin) ?
                   kKeyboardBacklightMax : kKeyboardBacklightMin;

    return setBacklightLevel(newLevel);
}

bool KeyboardBacklight::handleACPIEvent(uint32_t eventCode)
{
    if (!fBacklightSupported) {
        return false;
    }

    switch (eventCode) {
    case 0xD7: // _QD7 - keyboard backlight toggle
        return toggleBacklight();
    case 0xD8: // _QD8 - keyboard backlight toggle
        return toggleBacklight();
    default:
        return false;
    }
}

bool KeyboardBacklight::sendACPIEvent(uint32_t eventCode)
{
    if (!fACPIProvider) {
        return false;
    }

    // In a real implementation, we'd send the ACPI event to the appropriate device
    K513_LOG("Sending ACPI event 0x%02X", eventCode);
    return true;
}
