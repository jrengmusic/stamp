#include <JuceHeader.h>
#include <Files.h>
#include <Page.h>
#include "Manual.h"

static void printUsage()
{
    std::cout << BinaryData::getString (juce::String { files::help }) << std::endl;
}

static std::unique_ptr<jam::MarkdownComponent> getComponent (jam::MarkdownDocument&& markdown, juce::Rectangle<float> content)
{
    auto component { std::make_unique<jam::MarkdownComponent>() };

    component->setScrollBarsShown (false, false);
    component->setSize (static_cast<int> (content.getWidth()), static_cast<int> (content.getHeight()));
    component->setDocument (std::move (markdown));

    return component;
}

static std::unique_ptr<jam::MarkdownComponent> getComponent (const juce::String& text, const juce::File& inputFile, juce::Rectangle<float> content)
{
    return getComponent (jam::MarkdownDocument::parse (text, inputFile.getFullPathName()), content);
}

static juce::Rectangle<float> getPageArea (const juce::ValueTree& pageStyle)
{
    const auto page { map::Page::getInstance()->get (pageStyle.getProperty (Id::size).toString().toLowerCase()) };

    return { static_cast<float> (map::pageWidths[page]), static_cast<float> (map::pageHeights[page]) };
}

static jam::Array<juce::Range<int>> getSectionPages (const jam::MarkdownComponent& component, float contentHeight)
{
    return component.getPageRows (contentHeight, { 0, component.getNumRows() });
}

static jam::Array<juce::Range<int>> getBodyPages (const jam::MarkdownComponent& body,
                                                  const jam::Array<juce::Range<int>>& chapters,
                                                  const jam::Array<const jam::Document::Element*>& headings,
                                                  float contentHeight,
                                                  const juce::Identifier& pageType)
{
    jam::Array<juce::Range<int>> pages;

    for (const auto& chapter : chapters)
    {
        juce::Identifier name { Id::none };

        for (const auto* heading : headings)
            if (body.getRowNumber (*heading) == chapter.getStart())
                for (const auto& item : *heading->get<jam::Document::Identifiers> (Id::classes))
                    if (map::PageType::getInstance()->contains (item.toString()))
                        name = item;

        if (name == pageType)
            for (const auto& rows : body.getPageRows (contentHeight, chapter))
                pages.add (rows);
    }

    return pages;
}

static jam::HashMap<int, int> getRowPages (const jam::Array<juce::Range<int>>& pages, int pageNumber)
{
    jam::HashMap<int, int> rowPages;

    for (const auto& rows : pages)
    {
        for (int row { rows.getStart() }; row < rows.getEnd(); ++row)
            rowPages.addOrReplace (row, pageNumber);

        ++pageNumber;
    }

    return rowPages;
}

static jam::HashMap<const jam::Document::Element*, int> getPageNumbers (const jam::MarkdownComponent& body,
                                                                        const jam::Array<const jam::Document::Element*>& headings,
                                                                        const jam::HashMap<int, int>& rowPages)
{
    jam::HashMap<const jam::Document::Element*, int> pageNumbers;

    for (const auto* heading : headings)
    {
        const auto row { body.getRowNumber (*heading) };

        if (rowPages.contains (row))
            pageNumbers.addOrReplace (heading, rowPages.at (row));
    }

    return pageNumbers;
}

static std::unique_ptr<jam::MarkdownComponent> getContents (const juce::NamedValueSet& properties,
                                                            const jam::Array<const jam::Document::Element*>& headings,
                                                            const jam::MarkdownComponent& body,
                                                            const jam::Array<juce::Range<int>>& bodyPages,
                                                            int pageIndex,
                                                            const juce::File& inputFile,
                                                            juce::Rectangle<float> content)
{
    const auto component { getComponent (manual::getContentsText (properties, headings, getPageNumbers (body, headings, getRowPages (bodyPages, pageIndex + 1))), inputFile, content) };
    const auto pages (getSectionPages (*component, content.getHeight()));
    auto contents { getComponent (manual::getContentsText (properties, headings, getPageNumbers (body, headings, getRowPages (bodyPages, pageIndex + pages.size() + 1))), inputFile, content) };

    jassert (getSectionPages (*contents, content.getHeight()).size() == pages.size());

    return contents;
}

static const juce::Identifier& getPageSheet()
{
    static const juce::Identifier pageSheet { Id::toType (Id::page) };

    return pageSheet;
}

static const juce::Identifier& getElementSheet()
{
    static const juce::Identifier elementSheet { Id::toType (Id::element) };

    return elementSheet;
}

static int addSectionPages (jam::PdfDocument& document,
                            jam::MarkdownComponent& component,
                            const jam::Array<juce::Range<int>>& pages,
                            const juce::Identifier& pageType,
                            const juce::NamedValueSet& properties,
                            const jam::Array<const jam::Document::Element*>& headings,
                            juce::Rectangle<float> page,
                            float margin,
                            int pageIndex)
{
    for (const auto& rows : pages)
    {
        const auto pageNumber { pageIndex + 1 };

        {
            jam::PdfLowLevelGraphicsContext context { document, page.getWidth(), page.getHeight() };
            juce::Graphics g { context };

            g.fillAll (component.findColour (map::ColourId::markdownBackgroundColourId));
            manual::paintMarginBoxes (g, jam::StyleManager::getInstance()->getConfig (getPageSheet()), pageType, page, margin, pageNumber, manual::getPageStrings (properties, headings, component, jam::StyleManager::getInstance()->getConfig (getElementSheet()), rows));
            g.addTransform (juce::AffineTransform::translation (margin, margin));
            component.paintPage (g, rows);
        }

        document.addDestination (juce::String (pageNumber), pageIndex);
        ++pageIndex;
    }

    return pageIndex;
}

static void addPages (jam::PdfDocument& document,
                      jam::MarkdownDocument&& markdown,
                      const juce::NamedValueSet& properties,
                      const juce::File& inputFile)
{
    const auto area { getPageArea (jam::StyleManager::getInstance()->getConfig (getPageSheet())) };
    const auto margin { static_cast<float> (jam::StyleManager::getInstance()->getConfig (getPageSheet()).getProperty (Id::margin)) };
    const auto content { area.reduced (margin) };
    const juce::Identifier backCover { map::PageType::getInstance()->get (map::PageType::backCover) };
    jam::Array<const jam::Document::Element*> headings;

    for (const auto* block : markdown)
        if (block->contains (Id::type) and *block->get<int> (Id::type) == map::BlockType::heading)
            headings.add (block);

    const auto* root { markdown.getRoot() };
    const auto body { getComponent (std::move (markdown), content) };
    const auto chapters (manual::getChapterRows (*root, *body, jam::StyleManager::getInstance()->getConfig (getElementSheet())));
    const auto bodyPages (getBodyPages (*body, chapters, headings, content.getHeight(), Id::none));
    const auto cover { getComponent (manual::getCoverText (properties), inputFile, content) };
    const auto coverPages (getSectionPages (*cover, content.getHeight()));
    const auto contents { getContents (properties, headings, *body, bodyPages, coverPages.size(), inputFile, content) };
    const auto index { getComponent (manual::getIndexText (manual::getTermPages (properties, *root, *body, getRowPages (bodyPages, coverPages.size() + getSectionPages (*contents, content.getHeight()).size() + 1))), inputFile, content) };

    auto pageIndex { addSectionPages (document, *cover, coverPages, juce::Identifier { map::PageType::getInstance()->get (map::PageType::cover) }, properties, {}, area, margin, 0) };
    pageIndex = addSectionPages (document, *contents, getSectionPages (*contents, content.getHeight()), juce::Identifier { map::PageType::getInstance()->get (map::PageType::toc) }, properties, {}, area, margin, pageIndex);
    pageIndex = addSectionPages (document, *body, bodyPages, Id::none, properties, headings, area, margin, pageIndex);
    pageIndex = addSectionPages (document, *index, getSectionPages (*index, content.getHeight()), juce::Identifier { map::PageType::getInstance()->get (map::PageType::index) }, properties, {}, area, margin, pageIndex);
    addSectionPages (document, *body, getBodyPages (*body, chapters, headings, content.getHeight(), backCover), backCover, properties, headings, area, margin, pageIndex);
}

static bool writeDocument (const juce::File& inputFile, const juce::File& outputFile)
{
    static constexpr bool gpuDisabled { false };
    const juce::File noCacheFile;

    jam::VulkanEngine::getOrCreate (
        jam::VulkanEngine::getPrimaryDisplayExtent(), ProjectInfo::projectName, jam::VulkanEngine::getFrameBudget(), noCacheFile, gpuDisabled);

    jam::SharedInstance<jam::SharedDocuments> documents { std::in_place };
    auto markdown { jam::MarkdownDocument::parse (inputFile.loadFileAsString(), inputFile.getFullPathName()) };
    const auto properties (markdown.getProperties());
    juce::ValueTree config { Id::toType (Id::config) };

    jam::Css::getOrCreate (juce::Identifier { files::pageStyleSheet }).applyTo (config, Id::markdown);

    for (const auto& styleSheet : { files::markdownStyleSheet, files::mermaidStyleSheet })
        jam::Css::getOrCreate (juce::Identifier { styleSheet }).applyTo (config, juce::Identifier { juce::File::createFileWithoutCheckingPath (styleSheet).getFileNameWithoutExtension() });

    if (properties.contains (Id::css))
        jam::Css::parse (inputFile.getSiblingFile (properties[Id::css].toString()).loadFileAsString()).applyTo (config, Id::markdown);

    jam::StyleManager styleManager { { BinaryData::fetcher }, config };
    jam::StyleMarkdown styleMarkdown;
    juce::LookAndFeel::setDefaultLookAndFeel (&styleMarkdown);

    jam::PdfDocument document;
    addPages (document, std::move (markdown), properties, inputFile);

    return jam::PdfWriter {}.toFile (document, outputFile);
}

static bool writeDocument (const juce::ArgumentList& arguments)
{
    static constexpr int inputArgumentIndex { 0 };
    static constexpr int outputArgumentIndex { 1 };

    return writeDocument (arguments[inputArgumentIndex].resolveAsExistingFile(), arguments[outputArgumentIndex].resolveAsFile());
}

int main (int argc, char* argv[])
{
    static constexpr int argumentCount { 2 };
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    jam::SharedInstance<map::Page> pages { std::in_place };
    jam::SharedInstance<map::PageType> pageType { std::in_place };
    jam::SharedInstance<map::MarginBoxType> box { std::in_place };
    const juce::ArgumentList arguments { argc, argv };
    auto exitCode { EXIT_FAILURE };

    if (arguments.size() == argumentCount)
        exitCode = juce::ConsoleApplication::invokeCatchingFailures ([&arguments]
        {
            return writeDocument (arguments) ? EXIT_SUCCESS : EXIT_FAILURE;
        });
    else
        printUsage();

    return exitCode;
}
