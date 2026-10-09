#include <JuceHeader.h>
#include <Page.h>

static constexpr int maximumArgumentCount { 3 };

static void printUsage()
{
    static const juce::String helpFileName { "HELP.md" };

    std::cout << BinaryData::getString (helpFileName) << std::endl;
}

static bool writeDocument (const juce::File& inputFile, const juce::File& outputFile, int page)
{
    static constexpr bool gpuDisabled { false };
    const juce::File noCacheFile;

    jam::VulkanEngine::getOrCreate (
        jam::VulkanEngine::getPrimaryDisplayExtent(), ProjectInfo::projectName, jam::VulkanEngine::getFrameBudget(), noCacheFile, gpuDisabled);

    jam::SharedInstance<jam::SharedDocuments> documents { std::in_place };
    juce::ValueTree config { Id::toType (Id::config) };

    for (const auto& styleSheet : { files::markdownStyleSheet, files::mermaidStyleSheet })
        for (const auto& child : jam::Css::getOrCreate (juce::Identifier { styleSheet }).getValueTree (Id::toType (Id::config), juce::Identifier { juce::File::createFileWithoutCheckingPath (styleSheet).getFileNameWithoutExtension() }))
            config.appendChild (child.createCopy(), nullptr);

    jam::StyleManager styleManager { { BinaryData::fetcher }, config };
    jam::StyleMarkdown styleMarkdown;
    juce::LookAndFeel::setDefaultLookAndFeel (&styleMarkdown);

    jam::MarkdownComponent component;
    component.setDocument (jam::MarkdownDocument::parse (inputFile.loadFileAsString(), inputFile.getFullPathName()));

    return component.saveToFile (outputFile, map::pageWidths[page], map::pageHeights[page], map::pageMargins[page]);
}

static bool writeDocument (const juce::ArgumentList& arguments)
{
    static constexpr int inputArgumentIndex { 0 };
    static constexpr int outputArgumentIndex { 1 };
    static constexpr int pageArgumentIndex { 2 };
    static const juce::String defaultPageName { "a4" };

    const auto pageName { arguments.size() == maximumArgumentCount ? arguments[pageArgumentIndex].text : defaultPageName };
    const auto* pages { map::Page::getInstance() };
    auto isWritten { false };

    if (pages->contains (pageName))
        isWritten = writeDocument (arguments[inputArgumentIndex].resolveAsExistingFile(), arguments[outputArgumentIndex].resolveAsFile(), pages->get (pageName));
    else
        juce::ConsoleApplication::fail ("Unknown page: " + pageName);

    return isWritten;
}

int main (int argc, char* argv[])
{
    static constexpr int minimumArgumentCount { 2 };
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    jam::SharedInstance<map::Page> pages { std::in_place };
    const juce::ArgumentList arguments { argc, argv };
    auto exitCode { EXIT_FAILURE };

    if (arguments.size() >= minimumArgumentCount and arguments.size() <= maximumArgumentCount)
        exitCode = juce::ConsoleApplication::invokeCatchingFailures ([&arguments]
        {
            return writeDocument (arguments) ? EXIT_SUCCESS : EXIT_FAILURE;
        });
    else
        printUsage();

    return exitCode;
}
