# Telegram Focus 1.2.0

Based on Telegram Desktop 7.1.2.

## Highlights

- Added publishable Windows x64 installer and portable builds.
- Renamed the packaged Windows executable to `TelegramFocus.exe`.
- Changed the Windows product name to `Telegram Focus Desktop` and the publisher to `MrJimmsky`.
- Added the Telegram Focus three-triangle logo to the executable, installer, live taskbar icon, and system tray icon.
- Added the unique Windows AppUserModelID `MrJimmsky.TelegramFocusDesktop` so Telegram Focus is not grouped with official Telegram Desktop.
- Added a dedicated installer AppId and `%APPDATA%\Telegram Focus` installation directory to avoid collisions with official Telegram Desktop.
- Added the `TelegramForcePortable` marker to the portable package. Portable data is stored under `TelegramForcePortable\tdata`.

## Downloads

- `TelegramFocus-7.1.2-x64-setup.exe` - regular Windows x64 installer.
- `TelegramFocus-7.1.2-x64-portable.zip` - portable Windows x64 version.
- `SHA256SUMS.txt` - SHA-256 checksums for both Windows packages.
- `build-info.txt` - build configuration and verification details.

## SHA-256

```text
FD023AA302A1F26850EC529EE04C63515D73A3EEAD36DF3E181247B93732502C  TelegramFocus-7.1.2-x64-portable.zip
4DB86668A3FF4F31E5AC47F022C76C7F1F76BD1F22C6CE4E82201A6653A52763  TelegramFocus-7.1.2-x64-setup.exe
```

## Verification

- Installer and portable executables were smoke-tested on Windows x64.
- The installed and portable executable hashes match the verified build output.
- The runtime taskbar icon was checked from both installed and portable processes.
- Portable data was created only under `TelegramForcePortable\tdata`.
- Silent installer installation and complete uninstallation were verified.

## Important notes

- The Windows binaries are unsigned. Windows SmartScreen may display an "Unknown publisher" warning.
- Automatic updates and crash reporting are disabled in this build.
- Existing Telegram Focus macOS builds do not need to be rebuilt for these Windows-only identity and packaging changes.
