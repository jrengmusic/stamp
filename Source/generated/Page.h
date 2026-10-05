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
 * @brief Page sizes — the generated map namespace of page lookups.
 */

#pragma once

namespace map
{
/*_____________________________________________________________________________*/

/**
 * @brief Page sizes — the size and the margin of one page, in points.
 *
 * Each row is a page name that the command line accepts. The key indexes the width, height and margin lookups.
 */
struct Page : public jam::Bimap<int>
{
    Page() : jam::Bimap<int> { {
            { a4,     juce::String::fromUTF8 ("a4") },
            { letter, juce::String::fromUTF8 ("letter") },
    } } {}

    enum value : int
    {
        a4     = 0,///< ISO A4, 20 mm margin
        letter = 1,///< US Letter, 20 mm margin
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
 * @brief Page sizes — the size and the margin of one page, in points.
 *
 * Each row is a page name that the command line accepts. The key indexes the width, height and margin lookups.
 */
inline constexpr jam::LookupTable<int, double, Page::letter + 1> pageWidths {
    {
        { 0, 595.276 },
        { 1, 612 },
    }
};

//==============================================================================

/**
 * @brief Page sizes — the size and the margin of one page, in points.
 *
 * Each row is a page name that the command line accepts. The key indexes the width, height and margin lookups.
 */
inline constexpr jam::LookupTable<int, double, Page::letter + 1> pageHeights {
    {
        { 0, 841.89 },
        { 1, 792 },
    }
};

//==============================================================================

/**
 * @brief Page sizes — the size and the margin of one page, in points.
 *
 * Each row is a page name that the command line accepts. The key indexes the width, height and margin lookups.
 */
inline constexpr jam::LookupTable<int, double, Page::letter + 1> pageMargins {
    {
        { 0, 56.693 },
        { 1, 56.693 },
    }
};

/**______________________________END OF NAMESPACE______________________________*/
}// namespace map
