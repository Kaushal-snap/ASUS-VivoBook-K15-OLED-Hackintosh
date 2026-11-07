//
//  ASUSK513Controller.h
//  ASUSK513Controller
//
//  Main kernel extension for ASUS VivoBook K15 OLED K513EA.
//  Provides ACPI ATK device handling and Fn key event processing.
//  Does NOT modify display brightness or keyboard backlight directly.
//  Those are handled by separate modules.
//

#ifndef ASUSK513Controller_h
#define ASUSK513Controller_h

#include <IOKit/IOService.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include "ASUSK513ControllerDefs.h"

class ASUSK513Controller : public IOService
{
    OSDeclareDefaultStructors(ASUSK513Controller)

public:
    virtual bool init(OSDictionary *dictionary) override;
    virtual void free(void) override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

    // ACPI device access
    IOACPIPlatformDevice *getACPIProvider(void) const;
    bool evaluateACPIMethod(const char *methodName, uint64_t *result) const;
    bool notifyObservers(const char *notificationName) const;

    // Diagnostic mode
    bool isDiagnosticsEnabled(void) const;
    void setDiagnosticsEnabled(bool enabled);

private:
    IOACPIPlatformDevice *fACPIProvider;
    bool fDiagnosticsEnabled;
    bool fATKDFound;
    bool fATKQFound;
    bool fATKPFound;

    bool discoverATKDevices(void);
    void logDiscoveredDevices(void) const;
    bool checkACPIMethodExists(const char *methodPath) const;
};

#endif /* ASUSK513Controller_h */
