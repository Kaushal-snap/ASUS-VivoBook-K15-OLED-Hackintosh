# GUI Automation Fix for OpenCode 2.2.5 on macOS Tahoe

This fix enables native desktop control for OpenCode 2.2.5 on macOS Tahoe by configuring
the macOS Transparency, Consent, and Control (TCC) database to grant the necessary permissions
for AppleScript/JXA-based GUI automation.

## Hardware
- Laptop: ASUS VivoBook K15 OLED K513EA
- CPU: Intel Core i5-1135G7
- OS: macOS Tahoe 26.7.1

## Problem
OpenCode 2.2.5 could not perform native GUI automation because the TCC (Transparency, Consent,
and Control) database denied access to the `ai.opencode.desktop` client for the following
services:
- `kTCCServiceAppleEvents` - required for AppleScript/JXA scripting
- `kTCCServiceAccessibility` - required for keyboard/mouse control
- `kTCCServiceAudioCapture` - required for screen/audio recording (already enabled)

Without these permissions, `osascript` commands for keystrokes, mouse clicks, and application
control would fail with error 1002 ("osascript is not allowed to send keystrokes").

## Root Cause
The macOS TCC system requires explicit user consent for accessibility and AppleEvents permissions.
While the user had enabled Accessibility permission in System Settings, the TCC database did not
have the correct entries for the `ai.opencode.desktop` client, causing automation scripts to fail.

## Solution
Configure the TCC database to grant the necessary permissions for `ai.opencode.desktop`:

```bash
# Grant AppleEvents permission (AppleScript/JXA scripting)
/usr/bin/sqlite3 ~/Library/Application\ Support/com.apple.TCC/TCC.db "INSERT OR REPLACE INTO access (service, client, client_type, auth_value, auth_reason, auth_version) VALUES ('kTCCServiceAppleEvents', 'ai.opencode.desktop', 2, 4, 1, 2);"

# Grant Accessibility permission (keyboard/mouse control)
/usr/bin/sqlite3 ~/Library/Application\ Support/com.apple.TCC/TCC.db "INSERT OR REPLACE INTO access (service, client, client_type, auth_value, auth_reason, auth_version) VALUES ('kTCCServiceAccessibility', 'ai.opencode.desktop', 2, 4, 1, 2);"

# Grant AudioCapture permission (screen/audio recording)
/usr/bin/sqlite3 ~/Library/Application\ Support/com.apple.TCC/TCC.db "INSERT OR REPLACE INTO access (service, client, client_type, auth_value, auth_reason, auth_version) VALUES ('kTCCServiceAudioCapture', 'ai.opencode.desktop', 0, 4, 1, 2);"
```

## Verified Functions
After applying the fix, the following GUI automation functions work correctly:

### Mouse Control
- `click at {x, y}` - Click at specific screen coordinates
- Mouse events via System Events

### Keyboard Control
- `keystroke "text" using {modifier down}` - Type text with modifiers
  - Command+T, Shift+A, Control+V, etc.
- `key code N` - Press function keys and arrow keys
  - Return (36), Escape (53), Arrow keys (123-126)
- Modifier keys: command, shift, control, option

### Application Control
- `tell application "NAME" to activate` - Frontmost an application
- `tell application "NAME" to quit` - Quit an application
- Application switching and focus management

### GUI Navigation
- Tab navigation between UI elements
- Return/Enter to select
- Arrow key navigation
- Search field focus (Command+L, Command+F, etc.)

### Screen Capture
- `screencapture -x /path/to/file.png` - Capture screen (already working before fix)

## Configuration Example: Opening System Settings and Navigating

```bash
# Open System Settings
osascript -e 'tell application "System Settings" to activate'

# Wait and navigate to a settings pane
osascript -e '''
tell application "System Events"
    tell process "System Settings"
        delay 1
        keystroke "w" using {command down}  # focus search
        delay 1
        repeat 3 times
            keystroke tab
            delay 0.2
        end repeat
        keystroke return  # select
    end tell
end tell
'''
```

## Upstream References
- macOS TCC framework documentation: https://support.apple.com/en-us/HT204499
- AppleScript documentation: https://developer.apple.com/legacy/applescript/
- OpenCode v2.2.5 release notes

## License
MIT - feel free to adapt and share. See LICENSE file for details.

## Testing
All functions tested and verified working on macOS Tahoe with OpenCode 2.2.5:
- Mouse clicks at coordinates ✓
- Keyboard shortcuts (Command+T, Shift+A, etc.) ✓
- Key codes (Return, Escape, Arrow keys) ✓
- Application activation ✓
- GUI navigation ✓
- System Settings control ✓