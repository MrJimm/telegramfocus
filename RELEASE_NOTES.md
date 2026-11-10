# Telegram Focus 1.3.1

Based on Telegram Desktop 7.3.3. Release tag: `v7.3.3_focus1.3.1`.

## Highlights

- Added Windows x64 installer and portable ZIP builds based on Telegram Desktop 7.3.3.
- Updated the Windows uninstaller to preserve local data (`tdata`).
- Kept Windows Focus profiles and optional link handlers separate from the official Telegram Desktop client.
- Updated Qt to 6.11.3 for Windows.
- Restored WebM/VP9 playback and VP9 encoding for video sticker creation on macOS.
- Included the Allowlist fix that preserves pins for hidden chats.

## Changes

- Fixed missing WebM/VP9 emoji, video-sticker and video playback in the macOS 1.3.0 build.
- Restored the VP9 encoder required to create video stickers.
- Made libvpx build metadata relocatable while preserving private-path removal in the packaged binary.
- Added a build check that rejects FFmpeg configurations with disabled explicitly requested components.
- Includes the upstream update, Allowlist behavior and pin-preservation fix from 1.3.0. Wallet features remain outside Allowlist.

## Downloads

- `Telegram-Focus-v7.3.3_focus1.3.1-macOS-universal.dmg` — macOS application for Apple Silicon and Intel.
- `TelegramFocus-7.3.3-x64-setup.exe` — Windows x64 installer.
- `TelegramFocus-7.3.3-x64-portable.zip` — Windows x64 portable package.
- `SHA256SUMS.txt` — SHA-256 checksums for the release files.
- `BUILD_INFO.txt` — source revision and verification scope for each platform.

## Build sources

The macOS DMG was built from [`09d511cfd3`](https://github.com/MrJimm/telegramfocus/commit/09d511cfd34fb96e4e48a3f13dc1021a28a2c817). The Windows packages were built from the working tree later committed as [`a96c322c70`](https://github.com/MrJimm/telegramfocus/commit/a96c322c70f1b98b692f41ba7d57db581535c1e7), which the release tag points to.

## Install or update on macOS

1. Fully quit Telegram Focus and the regular Telegram Desktop client.
2. Open the DMG and drag **Telegram Focus.app** into **Applications**. Choose **Replace** when updating.
3. Launch **Telegram Focus** from **Applications**. Keep your existing `tdata`; deleting it is not part of an update.
4. If macOS blocks the first launch, open **System Settings → Privacy & Security**, click **Open Anyway**, and confirm.

The application requires macOS 12 or later. It is ad hoc signed and is not notarized by Apple.

## Install or update on Windows

Close Telegram Focus before updating. To upgrade an installed copy of Focus 1.2.0, run the new installer over the existing installation without running the old uninstaller. The installer is unsigned.

For the portable version, extract the whole ZIP and run `TelegramFocus.exe`. Keep the `TelegramForcePortable` folder when updating an existing portable copy.

## Verification and limitations

- We inspected the installer. Upgrading from Focus 1.2.0 and then uninstalling still needs testing on Windows.
- Media checks and their exact scope are recorded in `BUILD_INFO.txt`. No real account or funds are used in the isolated checks.
- The included pin-preservation code matches the earlier isolated Debug run with 273 passing assertions across 51 stages.
- The macOS application is universal (`arm64` and `x86_64`). Startup checks cover native Apple Silicon and Intel code through Rosetta, not physical Intel hardware.
- Automatic updates and crash reporting are disabled. Install future releases manually.
- On macOS, Focus shares the regular Telegram Desktop local data directory. Use only one of these desktop clients at a time.
- Allowlist is a client-side productivity filter. A queued notification reply or bot-button action can still execute after a chat is removed from Allowlist; this known limitation is not fixed in 1.3.1.
- The macOS dependency preparation script still uses predictable shared temporary files. Build on a trusted machine; hardening that build-time path is outside this release's media fix.
