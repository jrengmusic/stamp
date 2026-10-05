## index

+---------------+-------------------------------------------------+
| alias         | symbol                                          |
+===============+=================================================+
| @code         | ../../jam/cast/code.cast                        |
| @cmake        | cmake.cast                                      |
| @project-info | ../project-info.md                              |
| @signing      | signing.md                                      |
| @ProjectInfo  | ../Source/generated/ProjectInfo.h               |
| @Page         | ../Source/generated/Page.h                      |
| @CMakeLists   | ../CMakeLists.txt                               |
| @Entitlements | ../entitlements.plist                           |
| @semicolon    | ;                                               |
| @bimap        | jam::Bimap<int>                                 |
| @pageTable    | jam::LookupTable<int, double, Page::letter + 1> |
| @installer    | installer.cast                                  |
| @Distribution | installer/mac/distribution.xml                  |
| @Installer    | installer/win/installer.nsi                     |
| @Gh           | ../gh.cmake                                     |
| @Release      | ../RELEASE.md                                   |
+---------------+-------------------------------------------------+

## headers

+----------------+------------------------------------------------------------------------------------------+-------------------------------------------+
| file           | brief                                                                                    | description                               |
+================+==========================================================================================+===========================================+
| ProjectInfo.h  | ```                                                                                      | Product name, version, and source commit. |
|                | @file ProjectInfo.h                                                                      |                                           |
|                | @brief Project metadata — the generated ProjectInfo namespace.                           |                                           |
|                | ```                                                                                      |                                           |
+----------------+------------------------------------------------------------------------------------------+-------------------------------------------+
| Page.h         | ```                                                                                      |                                           |
|                | @file Page.h                                                                             |                                           |
|                | @brief Page sizes — the generated map namespace of page lookups.                         |                                           |
|                | ```                                                                                      |                                           |
+----------------+------------------------------------------------------------------------------------------+-------------------------------------------+
| CMakeLists.txt | ```                                                                                      |                                           |
|                | @file CMakeLists.txt                                                                     |                                           |
|                | @brief STAMP build manifest — a self-sufficient JUCE console project.                    |                                           |
|                |                                                                                          |                                           |
|                | Generated from project-info.md and cast/signing.md; every value traces to one table row. |                                           |
|                | Edit the table, run cast, then configure.                                                |                                           |
|                | ```                                                                                      |                                           |
+----------------+------------------------------------------------------------------------------------------+-------------------------------------------+

## output

+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| list                                          | separator                 | structure                                     | file          |
+===============================================+===========================+===============================================+===============+
| - [list]: @project-info:cmake                 | - [list]: @semicolon      | @cmake:cmake                                  | @CMakeLists   |
|                                               |                           | - [description]: @headers:brief               |               |
| - [list]: @project-info:project info          |                           |                                               |               |
| - [list]: @signing:signing                    |                           |                                               |               |
| - [list]: @project-info:architecture          | - [list]: @semicolon      | - [list]: @cmake:value                        |               |
| - [list]: @project-info:release:stage=        | - [list]: @semicolon      | - [list]: @cmake:mac                          |               |
| - [list]: @project-info:release:stage=linker  | - [list]: @semicolon      | - [list]: @cmake:mac                          |               |
| - [list]: @project-info:debug:stage=          | - [list]: @semicolon      | - [list]: @cmake:mac                          |               |
| - [list]: @project-info:release:stage=        | - [list]: @semicolon      | - [list]: @cmake:win                          |               |
| - [list]: @project-info:release:stage=linker  | - [list]: @semicolon      | - [list]: @cmake:win                          |               |
| - [list]: @project-info:debug:stage=          | - [list]: @semicolon      | - [list]: @cmake:win                          |               |
| - [list]: @project-info:patch                 |                           | - [list]: @cmake:patch                        |               |
| - [list]: @project-info:user module           |                           | - [list]: @cmake:module                       |               |
| - [list]: @project-info:source glob           |                           | - [list]: @cmake:glob-pattern                 |               |
| - [list]: @project-info:define                |                           | - [list]: @cmake:value                        |               |
| - [list]: @project-info:include               |                           | - [list]: @cmake:value                        |               |
| - [list]: @project-info:juce module           |                           | - [list]: @cmake:value                        |               |
| - [list]: @project-info:user module           |                           | - [list]: @cmake:link                         |               |
| - [list]: @project-info:binary                |                           | - [list]: @cmake:value                        |               |
| - [list]: @project-info:pack                  |                           | - [list]: @cmake:pack-value                   |               |
|                                               |                           | - strip: @cmake:strip                         |               |
|                                               |                           | - codesign: @cmake:codesign                   |               |
|                                               |                           | - notarize: @cmake:notarize                   |               |
|                                               |                           | - install-directory: @cmake:install-directory |               |
|                                               |                           | - install-copy: @cmake:install-copy           |               |
|                                               |                           | - install-rename: @cmake:install-rename       |               |
|                                               |                           | - postinstall: @cmake:postinstall             |               |
|                                               |                           | - xattr: @cmake:xattr                         |               |
|                                               |                           | - verify: @cmake:verify                       |               |
|                                               |                           | - pkg-staging: @cmake:pkg-staging             |               |
|                                               |                           | - pkgbuild: @cmake:pkgbuild                   |               |
|                                               |                           | - productbuild: @cmake:productbuild           |               |
|                                               |                           | - productsign: @cmake:productsign             |               |
|                                               |                           | - makensis: @cmake:makensis                   |               |
|                                               |                           | - staple: @cmake:staple                       |               |
|                                               |                           | - qa-directory: @cmake:qa-directory           |               |
|                                               |                           | - qa-copy: @cmake:qa-copy                     |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| > - [list]: @signing:signing:type=entitlement |                           | @code:[xml]entitlements                       | @Entitlements |
|                                               |                           | > - [list]: @code:entitlement                 |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| > - [list]: @project-info:project info        |                           | @code:namespace                               | @ProjectInfo  |
|                                               |                           | - macro: #pragma once                         |               |
|                                               |                           | - name: ProjectInfo                           |               |
|                                               |                           | - [description]: @headers:brief               |               |
|                                               |                           | > - [list]: @code:constant                    |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| > > > - [list]: @project-info:page            | - [list]: @code:linebreak | @code:namespace                               | @Page         |
|                                               |                           | - macro: #pragma once                         |               |
| > > - [list]: @project-info:page              |                           | - name: map                                   |               |
|                                               |                           | - [description]: @headers:brief               |               |
|                                               |                           |                                               |               |
|                                               |                           | @code:bimap                                   |               |
|                                               |                           | - name: Page                                  |               |
|                                               |                           | - [description]: Page names.                  |               |
|                                               |                           | - base: @bimap                                |               |
|                                               |                           | - keyType: int                                |               |
|                                               |                           | - valueType: juce::String                     |               |
|                                               |                           | > > > - [list]: @code:name-entry              |               |
|                                               |                           | > > - [list]: @code:enum-entry                |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| > > - [list]: @project-info:page              | - [list]: @code:linebreak | @code:namespace                               | @Page         |
|                                               |                           | - name: map                                   |               |
| > > - [list]: @project-info:page:key          | > > - [list]: @code:comma |                                               |               |
| > > - [list]: @project-info:page:width        |                           |                                               |               |
|                                               |                           | @code:lookup-table                            |               |
|                                               |                           | - type: @pageTable                            |               |
|                                               |                           | - name: pageWidths                            |               |
|                                               |                           | > > - [list]: @code:entry                     |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| > > - [list]: @project-info:page              | - [list]: @code:linebreak | @code:namespace                               | @Page         |
|                                               |                           | - name: map                                   |               |
| > > - [list]: @project-info:page:key          | > > - [list]: @code:comma |                                               |               |
| > > - [list]: @project-info:page:height       |                           |                                               |               |
|                                               |                           | @code:lookup-table                            |               |
|                                               |                           | - type: @pageTable                            |               |
|                                               |                           | - name: pageHeights                           |               |
|                                               |                           | > > - [list]: @code:entry                     |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| > > - [list]: @project-info:page              | - [list]: @code:linebreak | @code:namespace                               | @Page         |
|                                               |                           | - name: map                                   |               |
| > > - [list]: @project-info:page:key          | > > - [list]: @code:comma |                                               |               |
| > > - [list]: @project-info:page:margin       |                           |                                               |               |
|                                               |                           | @code:lookup-table                            |               |
|                                               |                           | - type: @pageTable                            |               |
|                                               |                           | - name: pageMargins                           |               |
|                                               |                           | > > - [list]: @code:entry                     |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| - [list]: @project-info:cmake                 |                           | @installer:distribution                       | @Distribution |
| - [list]: @project-info:project info          |                           |                                               |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| - [list]: @project-info:project info          |                           | @installer:installer                          | @Installer    |
| - [list]: @project-info:cmake                 |                           |                                               |               |
| - [list]: @signing:signing                    |                           |                                               |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| - [list]: @project-info:cmake                 |                           | @cmake:gh                                     | @Gh           |
| - [list]: @project-info:project info          |                           | - [list]: @cmake:pack-value                   |               |
| - [list]: @project-info:pack                  |                           |                                               |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
| - [list]: @project-info:cmake                 |                           | @installer:[no-banner]release                 | @Release      |
| - [list]: @project-info:release notes         |                           | - [list]: @installer:note                     |               |
+-----------------------------------------------+---------------------------+-----------------------------------------------+---------------+
