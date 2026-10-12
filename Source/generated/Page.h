/*******************************************************************************
                        Codegen Annotated Source of Truth
————————————————————————————————————————————————————————————————————————————————

            ░░████████████░░████████████░░████████████░░████████████
            ░░████  ░░████░░████  ░░████░░████  ░░████    ░░████
            ░░████        ░░████  ░░████░░████            ░░████
            ░░████        ░░████████████░░████████████    ░░████
            ░░████        ░░████  ░░████        ░░████    ░░████
            ░░████  ░░████░░████  ░░████░░████  ░░████    ░░████
            ░░████████████░░████  ░░████░░████████████    ░░████

————————————————————————————————————————————————————————————————————————————————
                         FOR YOUR EYES ONLY, DO NOT EDIT
********************************************************************************/

/**
 * @file Page.h
 * @brief Page sizes, page types and margin box lookups — the generated map namespace.
 */

#pragma once

namespace map
{
/*_____________________________________________________________________________*/

/**
 * @brief Page sizes — the CSS size keywords a4 and letter, in points.
 *
 * Each row is a CSS size keyword. The key indexes the width and height lookups.
 */
struct Page : public jam::Bimap<int>
{
    Page() : jam::Bimap<int> { {
            { a4,     juce::String::fromUTF8 ("a4") },
            { letter, juce::String::fromUTF8 ("letter") },
    } } {}

    enum value : int
    {
        a4     = 0,///< ISO A4
        letter = 1,///< US Letter
    };

    static Page* getInstance() noexcept
    {
        return jam::SharedInstance<Page>::getInstance();
    }
};

/**______________________________END OF NAMESPACE______________________________*/
}// namespace map

//==============================================================================

namespace map
{
/*_____________________________________________________________________________*/

/**
 * @brief Page sizes — the CSS size keywords a4 and letter, in points.
 *
 * Each row is a CSS size keyword. The key indexes the width and height lookups.
 */
inline constexpr jam::LookupTable<int, double, Page::letter + 1> pageWidths {
    {
        { 0, 595.276 },
        { 1, 612 },
    }
};

//==============================================================================

/**
 * @brief Page sizes — the CSS size keywords a4 and letter, in points.
 *
 * Each row is a CSS size keyword. The key indexes the width and height lookups.
 */
inline constexpr jam::LookupTable<int, double, Page::letter + 1> pageHeights {
    {
        { 0, 841.89 },
        { 1, 792 },
    }
};

//==============================================================================

/**
 * @brief Page types — the CSS named page types that stamp assigns.
 *
 * Each row is a page type that selects a named page rule. The key is the enum value and the value is the CSS name.
 */
struct PageType : public jam::Bimap<int>
{
    PageType() : jam::Bimap<int> { {
            { cover,     juce::String::fromUTF8 ("cover") },
            { toc,       juce::String::fromUTF8 ("toc") },
            { index,     juce::String::fromUTF8 ("index") },
            { backCover, juce::String::fromUTF8 ("back-cover") },
    } } {}

    enum value : int
    {
        cover     = 0,///< Cover page
        toc       = 1,///< Table-of-contents pages
        index     = 2,///< Index pages
        backCover = 3,///< Back cover page
    };

    static PageType* getInstance() noexcept
    {
        return jam::SharedInstance<PageType>::getInstance();
    }
};

//==============================================================================

/**
 * @brief Margin boxes — the alignment and the edge of each page-margin box.
 *
 * Each row is a page-margin box keyed by its map::MarginBoxType value. The justification is the horizontal alignment of css-page-3 Table 2 with the vertical alignment in the middle. The bottom column is true for a box in the bottom margin.
 */
inline constexpr jam::LookupTable<int, int, map::MarginBoxType::bottomRight + 1> marginBoxJustifications {
    {
        { 0, juce::Justification::centredLeft },
        { 1, juce::Justification::centred },
        { 2, juce::Justification::centredRight },
        { 3, juce::Justification::centredLeft },
        { 4, juce::Justification::centred },
        { 5, juce::Justification::centredRight },
    }
};

//==============================================================================

/**
 * @brief Margin boxes — the alignment and the edge of each page-margin box.
 *
 * Each row is a page-margin box keyed by its map::MarginBoxType value. The justification is the horizontal alignment of css-page-3 Table 2 with the vertical alignment in the middle. The bottom column is true for a box in the bottom margin.
 */
inline constexpr jam::LookupTable<int, bool, map::MarginBoxType::bottomRight + 1> bottomMarginBoxes {
    {
        { 0, false },
        { 1, false },
        { 2, false },
        { 3, true },
        { 4, true },
        { 5, true },
    }
};

/**______________________________END OF NAMESPACE______________________________*/
}// namespace map
