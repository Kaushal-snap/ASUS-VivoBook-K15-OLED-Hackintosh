//
//  KeyboardBacklight.h
//  ASUSK513Controller
//
//  Keyboard backlight control for ASUS VivoBook K15 OLED K513EA.
//  Only activates after verifying ACPI _QD7/_QD8 methods and SLKB/GLKB devices.
//  Does NOT control display brightness - that is separate.
//

#ifndef KeyboardBacklight_h
#define KeyboardBacklight_h

#include <IOKit/IOService.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include "ASUSK513ControllerDefs.h"

class KeyboardBacklight : public IOService
{
    OSDeclareDefaultStructors(KeyboardBacklight)

public:
    virtual bool init(OSDictionary *dictionary) override;
    virtual void free(void) override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

    // Keyboard backlight control (only after hardware verification)
    bool setBacklightLevel(int level);
    int getBacklightLevel(void) const;
    bool toggleBacklight(void);

    // ACPI event handling
    bool handleACPIEvent(uint32_t eventCode);

private:
    IOACPIPlatformDevice *fACPIProvider;
    int fCurrentLevel;
    bool fBacklightSupported;
    bool fHardwareVerified;

    bool verifyHardwareSupport(void);
    bool readBacklightLevel(void);
    bool writeBacklightLevel(int level);
    bool sendACPIEvent(uint32_t eventCode);
};

#endif /* KeyboardBacklight_h */
