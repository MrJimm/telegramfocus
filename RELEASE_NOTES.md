# Telegram Focus 1.3.0

Based on Telegram Desktop 7.3.3. Release tag: `v7.3.3_focus1.3.0`.

## Changes

- Updated the upstream Telegram Desktop baseline from 7.1.2 to 7.3.3, including its IPC security fixes.
- Preserved Allowlist filtering for chats, stories, search, sharing and notifications.
- Fixed hidden chats losing their pinned state. Reordering visible pins now preserves the complete server-backed pin order, including hidden chats.
- Preserved pinned state when the archive entry is hidden, while keeping explicit unpinning and genuine removals functional.
- Kept wallet features outside Allowlist, with standard upstream recipient search and transfer behavior.

## Downloads

- `Telegram-Focus-v7.3.3_focus1.3.0-macOS-universal.dmg` — macOS application for Apple Silicon and Intel.
- `SHA256SUMS.txt` — SHA-256 checksums for the release files.
- `BUILD_INFO.txt` — source revision, dependency revisions, build configuration and verification scope.

This release package contains a macOS build. The Windows installer and portable packages from 1.2.0 remain available in that earlier release; they are not builds of this version.

## Install or update on macOS

1. Fully quit Telegram Focus and the regular Telegram Desktop client.
2. Open the DMG and drag **Telegram Focus.app** into **Applications**. Choose **Replace** when updating.
3. Launch **Telegram Focus** from **Applications**. Keep your existing `tdata`; deleting it is not part of an update.

The application requires macOS 12 or later. It is ad hoc signed and is not notarized by Apple, so macOS may require an explicit first-launch approval in Privacy & Security.

## Verification and limitations

- The pin-preservation fix passed 273 assertions across 51 stages in an isolated Debug run with synthetic users and intercepted application RPCs. No real account or funds were used.
- The release application is universal (`arm64` and `x86_64`). Startup checks cover native Apple Silicon and Intel code through Rosetta, not physical Intel hardware.
- Automatic updates and crash reporting are disabled. Install future releases manually.
- On macOS, Focus shares the regular Telegram Desktop local data directory. Use only one of these desktop clients at a time.
- Allowlist is a client-side productivity filter. A queued notification reply or bot-button action can still execute after a chat is removed from Allowlist; this known limitation is not fixed in 1.3.0.
- The macOS dependency preparation script still uses predictable shared temporary files. Build on a trusted machine; hardening that build-time path is outside this release's pin-preservation fix.
