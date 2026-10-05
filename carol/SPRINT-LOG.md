# SPRINT-LOG

## Sprint 3: Release Lane — macOS pkg, Windows NSIS (x64, arm64), gh Upload ✅

**Date:** 2026-10-05
**Duration:** part of one session (cast sprint `release-lane` is the primary record)
**Plan:** `dev/cast/PLAN-release-lane.md` (locked)

### Files Modified
- `project-info.md` — `## pack`, `## release notes`, `## cmake` `repository`, `installerResourceDirectory`, `installDirectoryWindows`; `## toolchain` `cmake -P gh.cmake`.
- `cast/cmake.cast`, `cast/installer.cast` (new, no license lines: the project has no LICENSE), `cast/spell.md`, `cast/signing.md`, `cast/installer/resources/` (placeholders); generated `CMakeLists.txt`, `gh.cmake`, `RELEASE.md`, `cast/installer/{mac,win}/*`.

### State for Continuation
- Debug build green. The GitHub repository `jrengmusic/stamp` is empty: push the first commit before the first `gh release create`.

### Debts Paid
- None

### Debts Deferred
- None

## Sprint 2: `cast/signing.md`; Generated `entitlements.plist`; Own Repository ✅

**Date:** 2026-10-04
**Duration:** part of one session (jam Sprint 150 is the primary record)
**Plan:** `dev/jam/PLAN-signing.md` (locked)

### Decisions (ARCHITECT)
1. *"init stamp repo"*, remote `git@github.com:jrengmusic/stamp.git`; *"add ignore just like other project"*.
2. *"now let's fix the entitlements generation for ALL project"*; lane switch **"Own file per chain (Recommended)"**; keys **"Rows in ## signing"**, **"type column"**.

### Files Modified
- `.git` — `git init`, `origin` = `git@github.com:jrengmusic/stamp.git` (before: stamp sat inside the `dev/` repository, whose `origin` is `eve.git`).
- `.gitignore` — NEW, the four lines of `dev/cast/.gitignore` (`Builds/`, `docs/`, `*.log`, `.project`).
- `cast/signing.md` — NEW. `## signing` moved from `project-info.md`; no entitlement rows.
- `cast/spell.md` — index `@signing`, `@Entitlements`; `- [list]: @signing:signing`; output group `@code:[xml]entitlements` → `@Entitlements`; `CMakeLists.txt` brief names `cast/signing.md`.
- `CMakeLists.txt:21` — the brief line. `entitlements.plist` — now generated.

### Problems Solved
- Oracle (scratch mirror, `## toolchain` removed): `entitlements.plist` = the previous file plus the banner; `CMakeLists.txt` and the generated headers otherwise byte-identical.

### State for Continuation
- Not built. The repository has no commit yet.
- In jreng-filter-strip, `stamp` is no longer wired (jfs Sprint 106).

### Debts Paid
- None

### Debts Deferred
- None

## Sprint 1: STAMP — Simply Turns Any Markdown into PDF ✅

**Date:** 2026-10-04
**Duration:** one session (shared with jam Sprint 149, eve sprint "STAMP conformance", jreng-filter-strip Sprint 105)
**Plan:** `../jam/PLAN-stamp.md` (locked; steps 2, 3, 22 and the audit are this project's part)

### Agents Participated
- COUNSELOR: fable-5 — plan, delegations, validation by read, audit triage, this log
- Engineer: sonnet-5 — scaffold, `Main.cpp`, page table, audit fixes
- Auditor: opus-5 — one sweep over jam, stamp, eve, jfs, archetype (finding 64 is this project's)

### Decisions (ARCHITECT)
1. *"start new jam project as cli, for cast to wire, to produce User Manual.pdf from given md (including embeded images)"*.
2. *"STAMP: Simply Turns Any Markdown into PDF"*.
3. *"build with established pattern using cast. you probably can use cast init with archetype"*.
4. PDF: **"Real text"**, **"Fixed pages"**. First consumer: **"Headless manual CLI"**.

### Files Modified (all new)
- Scaffold from `archetype/cli/` through `cast INIT.md`: `project-info.md`, `cast/spell.md`, `cast/cmake.cast`, `CMakeLists.txt`, `entitlements.plist`.
- `project-info.md` — modules include `jam_whelmed` and `jam_pdf`; binary resources `mermaid.css`, `whelmed.css`, four fonts, `HELP.md`; `## page` table (`:44-57`, A4 and Letter: width, height, margin) → generated `Source/generated/Page.h` (`map::Page`, `map::pageWidths`, `map::pageHeights`, `map::pageMargins`).
- `Source/Main.cpp` — `stamp <input.md> <output.pdf> [page]`. `main` checks the argument count and runs `writeDocument (arguments)` inside `juce::ConsoleApplication::invokeCatchingFailures`. `writeDocument (inputFile, outputFile, page)` makes the engine with the GPU off (`VulkanEngine::getOrCreate`), parses the markdown, registers the two style sheets, and calls `WhelmedComponent::saveToFile`. No window opens.
- `Source/HELP.md` — the usage text.

### Alignment Check
- [x] BLESSED — the project holds no rendering code; it tells jam
- [x] NAMES.md
- [x] MANIFESTO.md — page sizes are data; an unknown page name fails at once

### Problems Solved
- `cast cast/spell.md --debug` builds and installs `~/.local/bin/stamp`.
- No argument, a missing input file, and an unknown page name each print usage or an error and return 1.
- The 7-page fixture renders to A4 and Letter with six embedded fonts (`emb yes`, `uni yes`).

### State for Continuation
- Every Debug run prints `JUCE Assertion failure in juce_Identifier.cpp:61`. The source is in jam (`jam_web/css/jam_Css.cpp:383-384`, `resources/mermaid.css:444`); see jam Sprint 149.
- `PLAN-stamp.md` step 22 names `stamp/cast` for the `## page` table; it is in `project-info.md:44-57`.
- The project has no git repository of its own; the directory is untracked in `/Users/jreng/Documents/Poems/dev`.

### Debts Paid
- None

### Debts Deferred
- None
