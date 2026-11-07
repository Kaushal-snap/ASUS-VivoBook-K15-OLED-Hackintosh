//
//  ASUSK513ControllerDiagnostics.h
//  ASUSK513Controller
//
//  Diagnostic mode driver for ASUSK513Controller.
//  Logs detected devices and supported features without changing hardware state.
//  Does NOT send ACPI queries or modify any hardware registers.
//

#ifndef ASUSK513ControllerDiagnostics_h
#define ASUSK513ControllerDiagnostics_h

#include <IOKit/IOService.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include "ASUSK513ControllerDefs.h"

class ASUSK513ControllerDiagnostics : public IOService
{
    OSDeclareDefaultStructors(ASUSK513ControllerDiagnostics)

public:
    virtual bool init(OSDictionary *dictionary) override;
    virtual void free(void) override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

    // Diagnostic reporting
    void logDeviceTree(void) const;
    void logACPIMethods(void) const;
    void generateDiagnosticReport(void) const;

private:
    IOACPIPlatformDevice *fACPIProvider;
    bool fDiagnosticsComplete;

    void scanACPIMethods(const char *basePath) const;
    bool methodExists(const char *methodPath) const;
};

#endif /* ASUSK513ControllerDiagnostics_h */
