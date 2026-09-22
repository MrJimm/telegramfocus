# Telegram Focus - a productivity-focused modification of [Telegram Desktop][telegram_desktop]
<img width="1880" height="837" alt="image" src="https://github.com/user-attachments/assets/f718bc28-21bf-4c81-9bde-7299efa9fe8d" />

Telegram Focus is a modification of the official [Telegram Desktop][telegram_desktop] client. It shows only the chats and stories you have manually added to an allowlist.

The filtering happens only inside Telegram Focus. It does not unsubscribe you from chats, change your Telegram folders, or affect how chats appear in other Telegram clients.

In every other respect, it is the same fully functional Telegram Desktop client as the corresponding upstream version.

You can still doomscroll on your phone, while staying clean on your desktop!

## How to use it

1. Install Telegram Focus.
2. Create a chat folder named **Allowlist** in your Telegram account. Use your phone or another regular Telegram client to create and manage it.
3. Add the chats you want to see on desktop to that folder.
4. Open Telegram Focus. Only chats and stories from your allowlist will be shown.

#### FRIENDLY REMINDER: On macOS use only one Telegram desktop client at a time - either Telegram Focus or the regular Telegram Desktop client, since...
...the macOS build of Telegram Focus uses the same local tdata folder as the regular Telegram Desktop client. Running both applications at the same time may cause conflicts with your local Telegram session.

## Installation

- [macOS](#macos)
- [Windows](#windows)
- [Linux](#linux)

### macOS

Download the [universal macOS DMG][macos_download] for Apple Silicon and Intel, or visit the [Releases][releases] page.

1. Open the downloaded DMG in Finder.
2. Drag **Telegram Focus.app** into the **Applications** folder.
3. Open **Telegram Focus** from **Applications**.
4. If macOS blocks the first launch, Control-click the app, choose **Open**, and confirm the action.

I mainly develop for Linux and Windows, so I do not have an Apple Developer account or the tools needed to notarize the app yet. The DMG still installs normally, but macOS may ask for one extra confirmation before the first launch.

#### FRIENDLY REMINDER: On macOS use only one Telegram desktop client at a time, since...

...the macOS build of Telegram Focus uses the same local `tdata` folder as the regular Telegram Desktop client. Running both applications at the same time may cause conflicts with your local Telegram session.

### Windows

Telegram Focus is available for Windows x64 as both a regular installer and a portable package:

- [Download the Windows installer][windows_installer]
- [Download the portable Windows version][windows_portable]
- [View all releases][releases]

#### Installer

1. Download and run [TelegramFocus-7.1.2-x64-setup.exe][windows_installer] (or download it from [releases][releases]).
2. Follow the installation steps.
3. Launch **Telegram Focus** from the Start menu or desktop shortcut.

The Windows binaries are currently unsigned. If Windows SmartScreen displays an **Unknown publisher** warning, click **More info**, then **Run anyway**.

#### Portable version

1. Download [TelegramFocus-7.1.2-x64-portable.zip][windows_portable] (or download it from [releases][releases]).
2. Extract the entire archive into a folder. Do not run the application directly from the ZIP file.
3. Run `TelegramFocus.exe` from the extracted folder.

The Windows installer stores its local data under `%APPDATA%\Telegram Focus`. The portable version stores it inside `TelegramForcePortable\tdata`. Both locations are separate from the official Telegram Desktop installation.

### Linux

Package coming soon.

## Building from source

Follow the official build instructions for the [Telegram Desktop][telegram_desktop] client. Telegram Desktop is based on the [Telegram API][telegram_api] and the [MTProto][telegram_proto] secure protocol.

* [Windows (32-bit and 64-bit)][win]
* [macOS][mac]
* [GNU/Linux using Docker][linux]



The source code is published under GPLv3 with the OpenSSL exception. See the [license][license] for details.

## Third-party

* Qt 6 ([LGPL](http://doc.qt.io/qt-6/lgpl.html)) and Qt 5.15 ([LGPL](http://doc.qt.io/qt-5/lgpl.html)) slightly patched
* OpenSSL 3.2.1 ([Apache License 2.0](https://openssl-library.org/source/license/apache-license-2.0.txt))
* WebRTC ([New BSD License](https://github.com/desktop-app/tg_owt/blob/master/LICENSE))
* zlib ([zlib License](http://www.zlib.net/zlib_license.html))
* LZMA SDK 9.20 ([public domain](http://www.7-zip.org/sdk.html))
* liblzma ([public domain](http://tukaani.org/xz/))
* Google Breakpad ([License](https://chromium.googlesource.com/breakpad/breakpad/+/master/LICENSE))
* Google Crashpad ([Apache License 2.0](https://chromium.googlesource.com/crashpad/crashpad/+/master/LICENSE))
* GYP ([BSD License](https://github.com/bnoordhuis/gyp/blob/master/LICENSE))
* Ninja ([Apache License 2.0](https://github.com/ninja-build/ninja/blob/master/COPYING))
* OpenAL Soft ([LGPL](https://github.com/kcat/openal-soft/blob/master/COPYING))
* Opus codec ([BSD License](http://www.opus-codec.org/license/))
* FFmpeg ([LGPL](https://www.ffmpeg.org/legal.html))
* Guideline Support Library ([MIT License](https://github.com/Microsoft/GSL/blob/master/LICENSE))
* Range-v3 ([Boost License](https://github.com/ericniebler/range-v3/blob/master/LICENSE.txt))
* Open Sans font ([Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html))
* Vazirmatn font ([SIL Open Font License 1.1](https://github.com/rastikerdar/vazirmatn/blob/master/OFL.txt))
* Emoji alpha codes ([MIT License](https://github.com/emojione/emojione/blob/master/extras/alpha-codes/LICENSE.md))
* xxHash ([BSD License](https://github.com/Cyan4973/xxHash/blob/dev/LICENSE))
* QR Code generator ([MIT License](https://github.com/nayuki/QR-Code-generator#license))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))
* Hunspell ([LGPL](https://github.com/hunspell/hunspell/blob/master/COPYING.LESSER))
* Ada ([Apache License 2.0](https://github.com/ada-url/ada/blob/main/LICENSE-APACHE))


[//]: # (LINKS)
[telegram_desktop]: https://github.com/telegramdesktop/tdesktop
[telegram_api]: https://core.telegram.org
[telegram_proto]: https://core.telegram.org/mtproto
[license]: LICENSE
[releases]: https://github.com/MrJimm/telegramfocus/releases
[macos_download]: https://github.com/MrJimm/telegramfocus/releases/download/v7.1.2_focus1.2.0/Telegram-Focus-v7.1.2_focus1.2.0-macOS-universal.dmg
[windows_installer]: https://github.com/MrJimm/telegramfocus/releases/download/v7.1.2_focus1.2.0/TelegramFocus-7.1.2-x64-setup.exe
[windows_portable]: https://github.com/MrJimm/telegramfocus/releases/download/v7.1.2_focus1.2.0/TelegramFocus-7.1.2-x64-portable.zip
[win]: docs/building-win.md
[mac]: docs/building-mac.md
[linux]: docs/building-linux.md
[preview_image]: https://github.com/telegramdesktop/tdesktop/blob/dev/docs/assets/preview.png "Preview of Telegram Desktop"
[preview_image_url]: https://raw.githubusercontent.com/telegramdesktop/tdesktop/dev/docs/assets/preview.png
