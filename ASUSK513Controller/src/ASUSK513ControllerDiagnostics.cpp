//
//  ASUSK513ControllerDiagnostics.cpp
//  ASUSK513Controller
//
//  Diagnostic mode driver for ASUSK513Controller.
//  Logs detected devices and supported features without changing hardware state.
//  Does NOT send ACPI queries or modify any hardware registers.
//

#include "ASUSK513ControllerDiagnostics.h"
#include "ASUSK513ControllerDefs.h"
#include <libkern/c++/OSBoolean.h>

#define super IOService
OSDefineMetaClassAndStructors(ASUSK513ControllerDiagnostics, IOService)

bool ASUSK513ControllerDiagnostics::init(OSDictionary *dictionary)
{
    if (!super::init(dictionary)) {
        return false;
    }

    fACPIProvider = nullptr;
    fDiagnosticsComplete = false;

    IOLog("[ASUSK513-Diag] Diagnostic mode initializing\n");
    return true;
}

void ASUSK513ControllerDiagnostics::free(void)
{
    if (fACPIProvider) {
        fACPIProvider->release();
        fACPIProvider = nullptr;
    }

    super::free();
}

IOService *ASUSK513ControllerDiagnostics::probe(IOService *provider, SInt32 *score)
{
    fACPIProvider = OSDynamicCast(IOACPIPlatformDevice, provider);
    if (!fACPIProvider) {
        return nullptr;
    }

    *score = 9999;
    return this;
}

bool ASUSK513ControllerDiagnostics::start(IOService *provider)
{
    if (!super::start(provider)) {
        return false;
    }

    IOLog("[ASUSK513-Diag] Diagnostic mode started\n");
    generateDiagnosticReport();
    fDiagnosticsComplete = true;

    return true;
}

void ASUSK513ControllerDiagnostics::stop(IOService *provider)
{
    IOLog("[ASUSK513-Diag] Diagnostic mode stopping\n");
    super::stop(provider);
}

void ASUSK513ControllerDiagnostics::generateDiagnosticReport(void) const
{
    IOLog("[ASUSK513-Diag] =========================================\n");
    IOLog("[ASUSK513-Diag] ASUSK513Controller Diagnostic Report\n");
    IOLog("[ASUSK513-Diag] Hardware: ASUS VivoBook K15 OLED K513EA\n");
    IOLog("[ASUSK513-Diag] CPU: Intel Core i5-1135G7 Tiger Lake-U\n");
    IOLog("[ASUSK513-Diag] GPU: Intel Iris Xe 8086:9A49\n");
    IOLog("[ASUSK513-Diag] OS: macOS Tahoe 26.7.1\n");
    IOLog("[ASUSK513-Diag] =========================================\n");

    logDeviceTree();
    logACPIMethods();

    IOLog("[ASUSK513-Diag] Diagnostic report complete\n");
    IOLog("[ASUSK513-Diag] =========================================\n");
}

void ASUSK513ControllerDiagnostics::logDeviceTree(void) const
{
    IOLog("[ASUSK513-Diag] --- ACPI Device Tree ---\n");

    const char *devices[] = {
        kACPI_ATKD,
        kACPI_ATKQ,
        kACPI_ATKP,
        kACPI_SLKB,
        kACPI_GLKB,
        nullptr
    };

    for (int i = 0; devices[i] != nullptr; i++) {
        bool exists = methodExists(devices[i]);
        IOLog("[ASUSK513-Diag]   %s: %s\n", devices[i], exists ? "FOUND" : "NOT FOUND");
    }

    IOLog("[ASUSK513-Diag] ---------------------------\n");
}

void ASUSK513ControllerDiagnostics::logACPIMethods(void) const
{
    IOLog("[ASUSK513-Diag] --- ACPI Query Methods ---\n");

    const char *methods[] = {
        kACPI_Q14,
        kACPI_Q15,
        kACPI_QD7,
        kACPI_QD8,
        kACPI_Q0A,
        kACPI_Q0B,
        kACPI_Q0C,
        kACPI_Q0D,
        kACPI_Q0E,
        kACPI_Q0F,
        kACPI_Q10,
        kACPI_Q11,
        kACPI_Q13,
        kACPI_Q16,
        kACPI_Q17,
        kACPI_Q18,
        nullptr
    };

    for (int i = 0; methods[i] != nullptr; i++) {
        bool exists = methodExists(methods[i]);
        IOLog("[ASUSK513-Diag]   %s: %s\n", methods[i], exists ? "FOUND" : "NOT FOUND");
    }

    IOLog("[ASUSK513-Diag] ---------------------------\n");
}

void ASUSK513ControllerDiagnostics::scanACPIMethods(const char *basePath) const
{
    // Reserved for future expansion - do not add speculative methods
}

bool ASUSK513ControllerDiagnostics::methodExists(const char *methodPath) const
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
