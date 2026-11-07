//
//  ASUSK513ControllerDefs.h
//  ASUSK513Controller
//
//  Kernel extension for ASUS VivoBook K15 OLED K513EA
//  Provides ACPI ATK device handling, keyboard backlight control,
//  and Fn key event processing for macOS Tahoe 26.7.1
//

#ifndef ASUSK513ControllerDefs_h
#define ASUSK513ControllerDefs_h

#include <libkern/libkern.h>
#include <IOKit/IOService.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>


// Module version
#define kASUSK513ControllerVersion "0.1.0-debug"

// Kext bundle identifiers
#define kASUSK513ControllerBundleID "com.github.asusk513controller.ASUSK513Controller"
#define kASUSK513DiagnosticsBundleID "com.github.asusk513controller.ASUSK513ControllerDiagnostics"
#define kASUSK513KeyboardBacklightBundleID "com.github.asusk513controller.ASUSK513KeyboardBacklight"

// ACPI device names (from DSDT analysis)
#define kACPI_ATKD "_SB_ATKD"
#define kACPI_ATKQ "ATKQ"
#define kACPI_ATKP "ATKP"
#define kACPI_SLKB "SLKB"
#define kACPI_GLKB "GLKB"
#define kACPI_KBLC "KBLC"
#define kACPI_KBLD "KBLD"
#define kACPI_KBLU "KBLU"
#define kACPI_BLCT "BLCT"
#define kACPI_EC "__"

// ACPI query method names (verified present in DSDT, must not add new ones)
#define kACPI_Q14 "_Q14"   // Display brightness down
#define kACPI_Q15 "_Q15"   // Display brightness up
#define kACPI_QD7 "_QD7"   // Keyboard backlight toggle
#define kACPI_QD8 "_QD8"   // Keyboard backlight toggle
#define kACPI_Q0A "_Q0A"
#define kACPI_Q0B "_Q0B"
#define kACPI_Q0C "_Q0C"
#define kACPI_Q0D "_Q0D"
#define kACPI_Q0E "_Q0E"
#define kACPI_Q0F "_Q0F"
#define kACPI_Q10 "_Q10"
#define kACPI_Q11 "_Q11"
#define kACPI_Q13 "_Q13"
#define kACPI_Q16 "_Q16"
#define kACPI_Q17 "_Q17"
#define kACPI_Q18 "_Q18"

// ACPI method return values (observed patterns, must verify on hardware)
#define kACPIStatusSuccess 0x0B
#define kACPIStatusFailure 0x00

// Keyboard backlight levels
#define kKeyboardBacklightMin 0
#define kKeyboardBacklightMax 3
#define kKeyboardBacklightDefault 1

// Diagnostic mode flag
#define kDiagnosticsEnabled true

// Notification names
#define kASUSK513NotificationBrightnessChanged "com.github.asusk513controller.brightnessChanged"
#define kASUSK513NotificationKeyboardBacklightChanged "com.github.asusk513controller.keyboardBacklightChanged"
#define kASUSK513NotificationFnLockChanged "com.github.asusk513controller.fnLockChanged"

// Debug logging macro (only active in DEBUG builds)
#ifdef DEBUG
#define K513_LOG(fmt, ...) IOLog("[ASUSK513] %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define K513_LOG(fmt, ...) /* no-op in release */
#endif

#endif /* ASUSK513ControllerDefs_h */
