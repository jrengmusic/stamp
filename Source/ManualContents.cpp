#include <JuceHeader.h>
#include "Manual.h"

namespace manual
{
static juce::String getPageLink (const juce::String& text, int pageNumber)
{
    return juce::String::charToString (Chars::openBracket) + text + juce::String::charToString (Chars::closeBracket)
         + juce::String::charToString (Chars::openParen) + juce::String::charToString (Chars::hash) + juce::String (pageNumber)
         + juce::String::charToString (Chars::closeParen);
}

static juce::String getIndexEntry (const juce::String& term, const jam::Array<int>& numbers)
{
    juce::String text;

    text << Chars::dash << Chars::space << term << Chars::colon;

    for (int index { 0 }; index < numbers.size(); ++index)
    {
        if (index > 0)
            text << Chars::comma;

        text << Chars::space << getPageLink (juce::String (numbers.at (index)), numbers.at (index));
    }

    return text << Chars::newline;
}

juce::String getCoverText (const juce::NamedValueSet& properties)
{
    juce::String text;

    if (properties.contains (Id::title))
    {
        if (properties.contains (Id::coverImage))
            text << Chars::exclamation << Chars::openBracket << Chars::closeBracket << Chars::openParen
                 << properties[Id::coverImage].toString() << Chars::closeParen << Chars::newline << Chars::newline;

        text << Chars::hash << Chars::space << properties[Id::title].toString() << Chars::newline << Chars::newline
             << properties[Id::subtitle].toString();
    }

    return text;
}

juce::String getContentsText (const juce::NamedValueSet& properties,
                              const jam::Array<const jam::Document::Element*>& headings,
                              const jam::HashMap<const jam::Document::Element*, int>& pageNumbers)
{
    static constexpr int defaultDepth { 3 };
    static constexpr int indentWidth { 3 };
    juce::String text;

    if (jam::MarkdownDocument::isEnabled (properties, Id::toc))
    {
        const auto depth { static_cast<int> (properties.getWithDefault (Id::tocDepth, defaultDepth)) };
        auto minimumLevel { std::numeric_limits<int>::max() };

        for (const auto* heading : headings)
            minimumLevel = juce::jmin (minimumLevel, heading->get<juce::String> (Id::level)->getIntValue());

        text << Chars::hash << Chars::space << properties.getWithDefault (Id::tocTitle, text::English::tableOfContents).toString() << Chars::newline << Chars::newline;

        for (const auto* heading : headings)
        {
            const auto level { heading->get<juce::String> (Id::level)->getIntValue() };

            if (level <= depth and pageNumbers.contains (heading) and not heading->get<jam::Document::Identifiers> (Id::classes)->contains (Id::unlisted))
                text << juce::String::repeatedString (juce::String::charToString (Chars::space), indentWidth * (level - minimumLevel)) << Chars::dash << Chars::space
                     << getPageLink ((heading->get<juce::String> (Id::number, juce::String {}) + juce::String::charToString (Chars::space) + *heading->get<juce::String> (Id::id)).trim(), pageNumbers.at (heading)) << Chars::space << pageNumbers.at (heading) << Chars::newline;
        }
    }

    return text;
}

jam::HashMap<juce::String, jam::Array<int>> getTermPages (const juce::NamedValueSet& properties,
                                                          const jam::Document::Element& root,
                                                          const jam::MarkdownComponent& body,
                                                          const jam::HashMap<int, int>& rowPages)
{
    jam::HashMap<juce::String, jam::Array<int>> termPages;

    if (const auto* terms { properties[Id::index].getArray() })
        for (const auto& term : *terms)
        {
            auto& numbers { termPages[term.toString()] };

            root.applyFunctionRecursively ([&numbers, &term, &body, &rowPages] (const jam::Document::Element& block)
            {
                if (block.contains (Id::type)
                    and (*block.get<int> (Id::type) == map::BlockType::paragraph or *block.get<int> (Id::type) == map::BlockType::heading)
                    and block.getAllSubText().containsIgnoreCase (term.toString()))
                {
                    const auto row { body.getRowNumber (block) };

                    if (rowPages.contains (row))
                        numbers.addIfNotAlreadyThere (rowPages.at (row));
                }
            });

            numbers.sort();
        }

    return termPages;
}

juce::String getIndexText (const jam::HashMap<juce::String, jam::Array<int>>& termPages)
{
    jam::Strings terms;
    juce::String text;
    auto group { Chars::nullCharacter };

    for (const auto& [term, numbers] : termPages)
        terms.add (term);

    terms.sort (true);

    if (terms.size() > 0)
        text << Chars::hash << Chars::space << text::English::index << Chars::newline << Chars::newline;

    for (const auto& term : terms)
    {
        const auto letter { term.toUpperCase()[0] };

        if (letter != group)
            text << Chars::newline << Chars::hash << Chars::hash << Chars::space << letter << Chars::newline << Chars::newline;

        group = letter;
        text << getIndexEntry (term, termPages.at (term));
    }

    return text;
}
}// namespace manual
