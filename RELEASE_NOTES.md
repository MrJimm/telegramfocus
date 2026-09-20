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
- `Telegram-Focus-v7.1.2_focus1.2.0-macOS-universal.dmg` - universal macOS build for Apple Silicon and Intel.
- `SHA256SUMS.txt` - SHA-256 checksums for all release packages.
- `build-info.txt` - Windows build configuration and verification details.

## Verification

- Installer and portable executables were smoke-tested on Windows x64.
- The installed and portable executable hashes match the verified build output.
- The runtime taskbar icon was checked from both installed and portable processes.
- Portable data was created only under `TelegramForcePortable\tdata`.
- Silent installer installation and complete uninstallation were verified.
- The macOS application is rebuilt from the release commit as a universal `arm64` and `x86_64` bundle.
- The macOS application bundle and DMG pass structural, signature, and checksum verification.

## Important notes

- The Windows binaries are unsigned. Windows SmartScreen may display an "Unknown publisher" warning.
- The macOS application is ad hoc signed and is not notarized by Apple. Gatekeeper may require opening it through Finder with Control-click, then **Open**.
- The macOS build requires macOS 12 or later.
- Automatic updates and crash reporting are disabled in this build.
- Telegram Focus behavior on macOS is unchanged in this release; the macOS package is rebuilt so every published artifact corresponds to the same release source.
