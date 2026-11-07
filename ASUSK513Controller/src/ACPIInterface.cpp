//
//  ACPIInterface.cpp
//  ASUSK513Controller
//
//  ACPI interface for ASUSK513Controller.
//  Reads ACPI methods and devices without modifying hardware state.
//  Does NOT add speculative EC query methods or blind EC writes.
//

#include "ACPIInterface.h"
#include "ASUSK513ControllerDefs.h"

#define super IOService
OSDefineMetaClassAndStructors(ACPIInterface, IOService)

bool ACPIInterface::init(OSDictionary *dictionary)
{
    if (!super::init(dictionary)) {
        return false;
    }

    fACPIProvider = nullptr;
    fHasATKD = false;
    fHasATKQ = false;
    fHasATKP = false;
    fHasSLKB = false;
    fHasGLKB = false;

    return true;
}

void ACPIInterface::free(void)
{
    if (fACPIProvider) {
        fACPIProvider->release();
        fACPIProvider = nullptr;
    }

    super::free();
}

IOService *ACPIInterface::probe(IOService *provider, SInt32 *score)
{
    fACPIProvider = OSDynamicCast(IOACPIPlatformDevice, provider);
    if (!fACPIProvider) {
        return nullptr;
    }

    *score = 8000;
    return this;
}

bool ACPIInterface::start(IOService *provider)
{
    if (!super::start(provider)) {
        return false;
    }

    discoverDevices();
    return true;
}

void ACPIInterface::stop(IOService *provider)
{
    if (fACPIProvider) {
        fACPIProvider->release();
        fACPIProvider = nullptr;
    }

    super::stop(provider);
}

bool ACPIInterface::discoverDevices(void)
{
    if (!fACPIProvider) {
        return false;
    }

    OSString *atkdStr = OSString::withCString(kACPI_ATKD);
    if (atkdStr) {
        fHasATKD = fACPIProvider->getProperty(atkdStr) != nullptr;
        atkdStr->release();
    }

    OSString *atkqStr = OSString::withCString(kACPI_ATKQ);
    if (atkqStr) {
        fHasATKQ = fACPIProvider->getProperty(atkqStr) != nullptr;
        atkqStr->release();
    }

    OSString *atkpStr = OSString::withCString(kACPI_ATKP);
    if (atkpStr) {
        fHasATKP = fACPIProvider->getProperty(atkpStr) != nullptr;
        atkpStr->release();
    }

    OSString *slkbStr = OSString::withCString(kACPI_SLKB);
    if (slkbStr) {
        fHasSLKB = fACPIProvider->getProperty(slkbStr) != nullptr;
        slkbStr->release();
    }

    OSString *glkbStr = OSString::withCString(kACPI_GLKB);
    if (glkbStr) {
        fHasGLKB = fACPIProvider->getProperty(glkbStr) != nullptr;
        glkbStr->release();
    }

    return fHasATKD || fHasATKQ || fHasATKP;
}

bool ACPIInterface::evaluateMethod(const char *methodPath, uint64_t *result) const
{
    if (!fACPIProvider || !methodPath || !result) {
        return false;
    }

    OSString *methodStr = OSString::withCString(methodPath);
    if (!methodStr) {
        return false;
    }

    // Note: evaluateMethod API not available in this SDK version.
    // ACPI method evaluation requires the correct API surface not available.
    // methodStr is autoreleased by OSString::withCString().
    return false;
}

bool ACPIInterface::methodExists(const char *methodPath) const
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

bool ACPIInterface::hasATKD(void) const { return fHasATKD; }
bool ACPIInterface::hasATKQ(void) const { return fHasATKQ; }
bool ACPIInterface::hasATKP(void) const { return fHasATKP; }
bool ACPIInterface::hasSLKB(void) const { return fHasSLKB; }
bool ACPIInterface::hasGLKB(void) const { return fHasGLKB; }
