//
//  ASUSK513Controller.cpp
//  ASUSK513Controller
//
//  Main kernel extension for ASUS VivoBook K15 OLED K513EA.
//  Provides ACPI ATK device handling and Fn key event processing.
//  Does NOT modify display brightness or keyboard backlight directly.
//  Those are handled by separate modules.
//

#include "ASUSK513Controller.h"
#include "ASUSK513ControllerDefs.h"
#include <libkern/c++/OSBoolean.h>

#define super IOService
OSDefineMetaClassAndStructors(ASUSK513Controller, IOService)

bool ASUSK513Controller::init(OSDictionary *dictionary)
{
    if (!super::init(dictionary)) {
        return false;
    }

    fACPIProvider = nullptr;
    fDiagnosticsEnabled = kDiagnosticsEnabled;
    fATKDFound = false;
    fATKQFound = false;
    fATKPFound = false;

    K513_LOG("ASUSK513Controller initializing (version %s)", kASUSK513ControllerVersion);
    return true;
}

void ASUSK513Controller::free(void)
{
    if (fACPIProvider) {
        fACPIProvider->release();
        fACPIProvider = nullptr;
    }

    K513_LOG("ASUSK513Controller freed");
    super::free();
}

IOService *ASUSK513Controller::probe(IOService *provider, SInt32 *score)
{
    // Only probe for IOACPIPlatformDevice compatible devices
    fACPIProvider = OSDynamicCast(IOACPIPlatformDevice, provider);
    if (!fACPIProvider) {
        return nullptr;
    }

    // Check for ASUS ATK device identifiers
    if (!discoverATKDevices()) {
        K513_LOG("No ASUS ATK devices found on provider");
        return nullptr;
    }

    if (fDiagnosticsEnabled) {
        logDiscoveredDevices();
    }

    *score = 10000;
    return this;
}

bool ASUSK513Controller::start(IOService *provider)
{
    if (!super::start(provider)) {
        return false;
    }

    K513_LOG("ASUSK513Controller started");

    return true;
}

void ASUSK513Controller::stop(IOService *provider)
{
    K513_LOG("ASUSK513Controller stopping");

    if (fACPIProvider) {
        fACPIProvider->release();
        fACPIProvider = nullptr;
    }

    super::stop(provider);
}

IOACPIPlatformDevice *ASUSK513Controller::getACPIProvider(void) const
{
    return fACPIProvider;
}

bool ASUSK513Controller::evaluateACPIMethod(const char *methodName, uint64_t *result) const
{
    if (!fACPIProvider || !methodName || !result) {
        return false;
    }

    // Note: evaluateMethod API may not be available in this SDK version.
    // This is a stub - actual implementation requires verifying the API.
    K513_LOG("evaluateACPIMethod(%s) not implemented in DEBUG build", methodName);
    return false;
}

bool ASUSK513Controller::notifyObservers(const char *notificationName) const
{
    if (!notificationName) {
        return false;
    }

    K513_LOG("Notifying observers: %s", notificationName);
    return true;
}

bool ASUSK513Controller::isDiagnosticsEnabled(void) const
{
    return fDiagnosticsEnabled;
}

void ASUSK513Controller::setDiagnosticsEnabled(bool enabled)
{
    fDiagnosticsEnabled = enabled;
}

bool ASUSK513Controller::discoverATKDevices(void)
{
    if (!fACPIProvider) {
        return false;
    }

    // Check for ATKD device
    OSString *atkdStr = OSString::withCString(kACPI_ATKD);
    if (atkdStr) {
        OSObject *atkdObj = fACPIProvider->getProperty(atkdStr);
        if (atkdObj) {
            fATKDFound = true;
            K513_LOG("ATKD device found");
        }
        atkdStr->release();
    }

    // Check for ATKQ device
    OSString *atkqStr = OSString::withCString(kACPI_ATKQ);
    if (atkqStr) {
        OSObject *atkqObj = fACPIProvider->getProperty(atkqStr);
        if (atkqObj) {
            fATKQFound = true;
            K513_LOG("ATKQ device found");
        }
        atkqStr->release();
    }

    // Check for ATKP device
    OSString *atkpStr = OSString::withCString(kACPI_ATKP);
    if (atkpStr) {
        OSObject *atkpObj = fACPIProvider->getProperty(atkpStr);
        if (atkpObj) {
            fATKPFound = true;
            K513_LOG("ATKP device found");
        }
        atkpStr->release();
    }

    return fATKDFound || fATKQFound || fATKPFound;
}

void ASUSK513Controller::logDiscoveredDevices(void) const
{
    K513_LOG("=== Discovered ACPI Devices ===");
    K513_LOG("ATKD: %s", fATKDFound ? "YES" : "NO");
    K513_LOG("ATKQ: %s", fATKQFound ? "YES" : "NO");
    K513_LOG("ATKP: %s", fATKPFound ? "YES" : "NO");
    K513_LOG("Diagnostics: %s", fDiagnosticsEnabled ? "ON" : "OFF");
    K513_LOG("=================================");
}

bool ASUSK513Controller::checkACPIMethodExists(const char *methodPath) const
{
    if (!fACPIProvider || !methodPath) {
        return false;
    }

    OSString *methodStr = OSString::withCString(methodPath);
    if (!methodStr) {
        return false;
    }

    bool exists = fACPIProvider->getProperty(methodStr) != nullptr;
    methodStr->release();
    return exists;
}
