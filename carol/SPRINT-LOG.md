# SPRINT-LOG

## Sprint 7: Build Against the jam Contract Audit; Page.h From cast; Margin-Box Justifications as Plain Cells ✅

**Date:** 2026-10-14
**Duration:** part of three sessions. `jam/carol/SPRINT-LOG.md` Sprint 163 is the primary record.

### Files Modified
- `Source/ManualContents.cpp:61, :67`: heading levels read as `int` (`get<int> (Id::level)`).
- `Source/Main.cpp`: the sheet identifiers are function-local accessors `getPageSheet()` and `getElementSheet()`. File-scope statics crashed before `main` (cross-unit static-init order).
- `Source/ManualPages.cpp`: includes `Page.h`. The file had never compiled against a cast-generated `Page.h`.
- `project-info.md:87-92`: the six justification cells are plain cells. Backtick cells are literals to cast (`cast/Source/Model.h:1608-1615`), so they generated quoted strings. The Sprint 6 `Page.h` was a hand mirror (`:57`), which hid this.
- `cast/spell.md`: the `## output` table is re-laid by a cast run (column 1 widened; cell text unchanged).
- `RELEASE.md`: one usage bullet lost the `[page]` token. The source of this edit was not traced this sprint.

### Verification
- `cast cast/spell.md --debug`: 0 errors, 0 warnings. `git diff HEAD -- Source/generated/Page.h` is empty.
- `stamp "installer/manual/JRENG! Filter Strip Manual.md"`: exit 0, 19 pages. `stamp "installer/JRENG! Filter Strip Manual.md"`: exit 0, 3 pages (it aborted before jam made an absent sheet return an empty tree).

## Sprint 6: Manual Composer — Cover, TOC, Margin Boxes, Index, Back Cover, Links; JRENG! Filter Strip Manual ✅

**Date:** 2026-10-12
**Duration:** about 4 hours, from 11:01 UTC
**Plan:** `~/.claude/plans/staged-sauteeing-tulip.md`: Phases A–G, Amendments 1–7. The approved PLAN ends with the log, so the run had no gate until this log.

This is the primary record. `jam/carol/SPRINT-LOG.md` Sprint 161 records the jam side: CSS `@page` and cascade, PDF links, pandoc header attributes and section numbers, inline SVG, and the paging API.

### Agents Participated
- **COUNSELOR** (opus-5.5): design discussion, plan, delegations, validation by read, audit triage, `Page.h` brief fix, RFC `~/.carol/RFC-subagent-background.md`, this log.
- **Engineer** (sonnet): stamp composer, jfs manual, audit fixes, doxygen pass.
- **Librarian** (haiku): pandoc metadata and numbering, css-page-3, GCPM, PDF links.
- **Pathfinder** (haiku): discovery, oracle PDF extraction, working-tree reads.
- **Auditor** (opus): one sweep, 34 findings, shared with jam.

### Decisions (ARCHITECT, verbatim)
See jam Sprint 161, Decisions 1–8. The stamp-specific rulings:
- *"so this is at project level? not at module? stamp exclusive?"* → **"stamp composes"**.
- *"extract the oracle PDF to markdown formatted as stamp should read and produce"*; *"use placeholder png/svg for any images you need … or can you extract from the PDF?"*
- **"Back cover after index"**.

### Design
- **CLI:** `stamp <input.md> <output.pdf>`. CSS `size` is the single source of the page size. The `[page]` argument is deleted.
- **Config cascade:** `CssDocument::applyTo` merges, in order:
  1. `page.css`, the user-agent default (`@page { size: A4; margin: 56.693pt; }`);
  2. `markdown.css`;
  3. `mermaid.css`;
  4. the front matter `css:` file, resolved beside the `.md`.
- **Composition** (`namespace manual`):
  - `ManualContents.cpp`: `getCoverText`, `getContentsText`, `getTermPages`, `getIndexText`, `getPageLink`, `getIndexEntry`.
  - `ManualPages.cpp`: `getBoxProperty`, `getChapterStarts`, `getHeadingString`, `getBoxText`, `paintBoxBorder`, `getChapterRows`, `getPageStrings`, `paintMarginBoxes`.
  - `Main.cpp`: `addPages`, `addSectionPages`, and the page geometry and row helpers.
- **Generated pages** are markdown, painted by a second `MarkdownComponent` with the same CSS-LAF. Links travel on `AttributedString::Link`.
- **Page order:** cover, TOC, body, index, back cover.
- **Destinations:** each body page gets `addDestination (String (pageNumber), pageIndex)`.
- **Margin boxes:**
  - Content is a string, `counter(page)` or `string(<name>)`, with `first` as the default (GCPM).
  - A named page style overrides the base `@page` style per property.
  - `content: none` paints nothing.
- **Cast tables** (`project-info.md`, `cast/spell.md`):
  - `## page`: `a4` and `letter`; the margin column is deleted.
  - `## page type`: cover, toc, index, back-cover.
  - `## margin box`: justification and bottom lookups.
  - `## files`: `help`, `pageStyleSheet`.
  - `## binary`: `pageStyleSheet`.

### Files Modified (stamp, 11)
- `project-info.md`: `## page`, `## page type`, `## margin box`, `## files`, `## binary`.
- `cast/spell.md`: the matching rows and aliases (`@Files`, `@justificationTable`, `@bottomTable`); `:39` is the `Page.h` brief.
- `CMakeLists.txt`: binary glob.
- `Source/page.css` (new).
- `Source/HELP.md`: the new usage line.
- `Source/Main.cpp`: CLI, cascade, page loop.
- `Source/Manual.h`, `Source/ManualContents.cpp`, `Source/ManualPages.cpp` (new).
- `Source/generated/Page.h`, `Source/generated/Files.h` (new): hand-mirrored. A cast run reconciles them.

### Files Modified (jreng-filter-strip, `Source/installer/manual/`, new)
- `JRENG! Filter Strip Manual.md`:
  - front matter: title, subtitle, cover-image, css, toc, toc-depth 2, toc-title, number-sections, and 26 index terms from the Functions labels;
  - chapters, with `{.unnumbered .unlisted}` subheadings;
  - the back cover `{.unnumbered .unlisted .back-cover}`;
  - the source typo "makebsure", kept verbatim.
- `JRENG! Filter Strip Manual.css`: `@page` margin boxes and borders; `@page cover` and `@page back-cover` with `content: none`; `h1` page break and `string-set`; `title` `string-set`; Helvetica `@font-face` overrides.
- `images/`: `cover.jpg`, `authorization-1..5.png`, `interface.jpg`, `high-pass.svg`, `low-pass.svg`, extracted from the oracle PDF.

### Alignment Check
- [x] BLESSED principles followed
- [x] NAMES.md adhered
- [x] MANIFESTO.md principles applied

### Problems Solved
- **Blocker 2.** `contentText` was registered with `juce::var`, but the call passes `const juce::var&` (`juce_Variant.h:233`) and `int&`. Now registered with `const juce::var&, int&, const juce::NamedValueSet&` (`ManualPages.cpp:82-94`).
- **Hand tables.** `marginBoxJustifications` and `bottomMarginBoxes` moved to cast.
- **DRY.** `getPageLink`, `getIndexEntry`, `getBoxProperty`.

### Audit
- Shared with jam Sprint 161: 34 findings, triage recorded there.

### Violations
- See jam Sprint 161. Specific to stamp:
  - The doxygen Engineer edited the generated `Page.h` brief, whose source is `cast/spell.md:39`. COUNSELOR set both lines to the same text.
  - The Engineer registered `contentText` twice with argument types that did not match the call (Blocker 2).

### State for Continuation
- **Not built.**
  1. ARCHITECT builds jam generation: `cast cast/spell.md` in jam.
  2. ARCHITECT builds stamp: `cast cast/spell.md --debug`. Cast reconciles `Page.h`, `Files.h` and `RELEASE.md`.
  3. ARCHITECT runs `stamp "…/manual/JRENG! Filter Strip Manual.md" out.pdf`.
- Verify `out.pdf`, read-only:
  - `pdfinfo`: A4 and the page count;
  - `pdftotext -layout`: cover, TOC numbers and page labels, uppercase headers, footer numbers, index letter groups, back cover with no header or footer;
  - `pdfimages -list`: raster images only;
  - `pdftocairo -svg`: the plot pages have paths and no images;
  - `/Annots`, `/Subtype /Link`, `/Dest` and `/URI` counts.
- Regression: a markdown file with no front matter renders as before, except for the `page.css` margins.
- `ninja doxygen` and its zero-warning check are not yet run.
- The images are extracted from the oracle. ARCHITECT replaces them when the source assets are ready.

### Debts Paid
None.

### Debts Deferred
None.

## Sprint 4: MarkdownComponent Replaces Whelmed ✅

**Date:** 2026-10-08
**Duration:** part of one session (jam Sprint 153 is the primary record)

### Files Modified
- `Source/Main.cpp` — registers the markdown and mermaid style sheets; `jam::StyleMarkdown` as the default LookAndFeel; `jam::MarkdownComponent::setDocument`, `saveToFile`.
- `project-info.md` — `@markdown-style` (`markdown.css`), user module `jam_markdown_graphics`, binary `markdownStyleSheet`; font descriptions.

### State for Continuation
- Builds: `--debug` and `--no-sign` 0/0.
- `stamp eve/markdown/markdown.md out.pdf` writes 50 pages; ARCHITECT validates against Obsidian.

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
