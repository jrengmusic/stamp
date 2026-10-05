# stamp v0.1.0

Simply Turns Any Markdown into PDF

## What's New

- Release lane: a signed macOS pkg and Windows x64 and arm64 installers, uploaded to the GitHub release.
- stamp input.md output.pdf [page] turns markdown into PDF with real text, fixed A4 or Letter pages, and embedded fonts.
- Signing data lives in cast/signing.md, and entitlements.plist is generated from it.

## Platforms

| Platform | Architecture       | Format                            |
| -------- | ------------------ | --------------------------------- |
| macOS    | universal          | .pkg (signed, notarized)          |
| Windows  | x64                | .exe installer                    |
| Windows  | arm64              | .exe installer                    |

## Installation

macOS: open the .pkg. It installs `stamp` on your PATH.

Windows: run the installer. It installs `stamp.exe` and adds its folder to your user PATH.
