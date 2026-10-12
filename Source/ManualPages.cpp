#include <JuceHeader.h>
#include "Manual.h"

namespace manual
{
static juce::var getBoxProperty (const juce::ValueTree& specific,
                                 const juce::ValueTree& base,
                                 const juce::Identifier& key)
{
    return specific.getProperty (key, base.getProperty (key));
}

static juce::var getBoxProperty (const juce::ValueTree& specific,
                                 const juce::ValueTree& base,
                                 const juce::Identifier& key,
                                 const juce::var& defaultValue)
{
    return specific.getProperty (key, base.getProperty (key, defaultValue));
}

static jam::Array<int> getChapterStarts (const jam::Document::Element& root,
                                         const jam::MarkdownComponent& body,
                                         const juce::ValueTree& elementStyle)
{
    jam::Array<int> rows;
    auto row { body.getNumRows() };

    for (const auto* block : root)
        if (block->contains (Id::type) and *block->get<int> (Id::type) != map::BlockType::metadataBlock)
        {
            row = body.getRowNumber (*block);
            break;
        }

    rows.add (row);

    for (const auto* block : root)
        if (block->contains (Id::type) and *block->get<int> (Id::type) == map::BlockType::heading)
        {
            const auto text { elementStyle.getChildWithName (Id::toType (block->id)).getProperty (Id::breakBefore, Id::none.toString()).toString() };

            if (juce::Identifier { text } == Id::page)
                rows.add (body.getRowNumber (*block));
        }

    return rows;
}

static juce::String getHeadingString (const juce::Identifier& tag,
                                      const jam::Array<const jam::Document::Element*>& headings,
                                      const jam::MarkdownComponent& body,
                                      juce::Range<int> rows)
{
    juce::String text;

    for (const auto* heading : headings)
    {
        if (Id::toType (heading->id) == tag)
        {
            const auto row { body.getRowNumber (*heading) };

            if (row < rows.getEnd())
                text = *heading->get<juce::String> (Id::id);

            if (row >= rows.getStart())
                break;
        }
    }

    return text;
}

static juce::String getBoxText (const juce::ValueTree& box,
                                int pageNumber,
                                const juce::NamedValueSet& strings)
{
    static const auto contentText {
        []
        {
            jam::Function::Map<juce::Identifier, juce::String> contentText;

            contentText.add<const juce::var&, int&, const juce::NamedValueSet&> (Id::string, [] (const juce::var& argument, int, const juce::NamedValueSet& strings)
                                                                         { return strings[juce::Identifier { argument.toString() }].toString(); });
            contentText.add<const juce::var&, int&, const juce::NamedValueSet&> (Id::counter, [] (const juce::var&, int number, const juce::NamedValueSet&)
                                                                         { return juce::String (number); });

            return contentText;
        }()
    };

    const auto content { box.getProperty (Id::content) };

    if (content.isArray())
        return contentText.get (juce::Identifier { content[0].toString() }, content[1], pageNumber, strings);

    if (content.toString().isNotEmpty() and juce::Identifier { content.toString() } != Id::none)
        return content.toString();

    return {};
}

static void paintBoxBorder (juce::Graphics& g,
                            const juce::ValueTree& specific,
                            const juce::ValueTree& base,
                            const juce::Identifier& widthKey,
                            const juce::Identifier& colourKey,
                            juce::Rectangle<float> edge)
{
    const auto thickness { static_cast<float> (getBoxProperty (specific, base, widthKey, 0.0)) };

    if (thickness > 0.0f)
    {
        g.setColour (jam::StyleManager::getInstance()->getColour (juce::Identifier { getBoxProperty (specific, base, colourKey).toString() }));
        g.fillRect (edge.withHeight (thickness));
    }
}

jam::Array<juce::Range<int>> getChapterRows (const jam::Document::Element& root,
                                             const jam::MarkdownComponent& body,
                                             const juce::ValueTree& elementStyle)
{
    auto rows (getChapterStarts (root, body, elementStyle));
    jam::Array<juce::Range<int>> chapters;

    rows.add (body.getNumRows());

    for (int index { 1 }; index < rows.size(); ++index)
    {
        const juce::Range<int> chapter { rows.at (index - 1), rows.at (index) };

        if (not chapter.isEmpty())
            chapters.add (chapter);
    }

    return chapters;
}

juce::NamedValueSet getPageStrings (const juce::NamedValueSet& properties,
                                    const jam::Array<const jam::Document::Element*>& headings,
                                    const jam::MarkdownComponent& body,
                                    const juce::ValueTree& elementStyle,
                                    juce::Range<int> rows)
{
    juce::NamedValueSet strings;

    for (const auto& record : elementStyle)
    {
        if (record.hasProperty (Id::stringSet))
        {
            const juce::Identifier name { record.getProperty (Id::stringSet).toString() };

            if (record.getType() == Id::toType (Id::title))
                strings.set (name, properties[Id::title]);
            else
                strings.set (name, getHeadingString (record.getType(), headings, body, rows));
        }
    }

    return strings;
}

void paintMarginBoxes (juce::Graphics& g,
                       const juce::ValueTree& pageStyle,
                       const juce::Identifier& pageType,
                       juce::Rectangle<float> page,
                       float margin,
                       int pageNumber,
                       const juce::NamedValueSet& strings)
{
    for (const auto& [index, name] : map::MarginBoxType::getInstance()->get())
    {
        const auto base { pageStyle.getChildWithName (Id::toType (juce::Identifier { name })) };
        const auto specific { pageStyle.getChildWithName (Id::toType (pageType)).getChildWithName (Id::toType (juce::Identifier { name })) };
        const auto box { specific.hasProperty (Id::content) ? specific : base };
        const auto area { map::bottomMarginBoxes[index] ? page.reduced (margin, 0.0f).withTop (page.getHeight() - margin) : page.reduced (margin, 0.0f).withHeight (margin) };
        auto text { getBoxText (box, pageNumber, strings) };

        if (text.isNotEmpty())
        {
            if (juce::Identifier { getBoxProperty (specific, base, Id::textTransform, Id::none.toString()).toString() } == Id::uppercase)
                text = text.toUpperCase();

            g.setFont (juce::Font { jam::StyleManager::getInstance()->getFont (getBoxProperty (specific, base, Id::fontFamily).toString()).withPointHeight (static_cast<float> (getBoxProperty (specific, base, Id::fontSize))) });
            g.setColour (jam::StyleManager::getInstance()->getColour (juce::Identifier { getBoxProperty (specific, base, Id::color).toString() }));
            g.drawText (text, area, juce::Justification { map::marginBoxJustifications[index] }, false);

            paintBoxBorder (g, specific, base, Id::borderBottomWidth, Id::borderBottomColor, area.withY (area.getBottom()));
            paintBoxBorder (g, specific, base, Id::borderTopWidth, Id::borderTopColor, area);
        }
    }
}
}// namespace manual
