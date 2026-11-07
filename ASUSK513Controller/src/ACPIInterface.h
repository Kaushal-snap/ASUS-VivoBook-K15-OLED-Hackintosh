//
//  ACPIInterface.h
//  ASUSK513Controller
//
//  ACPI interface for ASUSK513Controller.
//  Reads ACPI methods and devices without modifying hardware state.
//  Does NOT add speculative EC query methods or blind EC writes.
//

#ifndef ACPIInterface_h
#define ACPIInterface_h

#include <IOKit/IOService.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include "ASUSK513ControllerDefs.h"

class ACPIInterface : public IOService
{
    OSDeclareDefaultStructors(ACPIInterface)

public:
    virtual bool init(OSDictionary *dictionary) override;
    virtual void free(void) override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

    // ACPI method evaluation (read-only)
    bool evaluateMethod(const char *methodPath, uint64_t *result) const;
    bool methodExists(const char *methodPath) const;

    // Device discovery
    bool hasATKD(void) const;
    bool hasATKQ(void) const;
    bool hasATKP(void) const;
    bool hasSLKB(void) const;
    bool hasGLKB(void) const;

private:
    IOACPIPlatformDevice *fACPIProvider;
    bool fHasATKD;
    bool fHasATKQ;
    bool fHasATKP;
    bool fHasSLKB;
    bool fHasGLKB;

    bool discoverDevices(void);
};

#endif /* ACPIInterface_h */
