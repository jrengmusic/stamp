/**
 * @file Manual.h
 * @brief Text and margin box functions that build the cover, contents, index and page furniture of a printed manual.
 */
#pragma once

/** @brief Functions that build the generated pages of a manual and paint the margin boxes of a page. */
namespace manual
{
/**
 * @brief Returns the markdown text of the cover page.
 *
 * The text holds an image line when the properties hold `cover-image`, then a level 1 heading
 * with the title, then the subtitle.
 *
 * @param properties  Properties of the metadata block of the manual.
 * @return The cover text; empty when the properties hold no `title`.
 */
juce::String getCoverText (const juce::NamedValueSet& properties);

/**
 * @brief Returns the markdown text of the table of contents.
 *
 * The text starts with a heading. Each listed heading adds one list item. The item shows the
 * section number, the text and the page number, as a link to that page. A heading is listed when
 * its level is not deeper than `toc-depth`. It also needs a known page number and no `unlisted` class.
 * The indent of an item follows the level of its heading, from the shallowest level.
 *
 * @param properties   Properties of the metadata block of the manual.
 * @param headings     Headings of the manual, in source order.
 * @param pageNumbers  Page number of each heading.
 * @return The contents text; empty unless the property `toc` is `true`.
 */
juce::String getContentsText (const juce::NamedValueSet& properties,
                              const jam::Array<const jam::Document::Element*>& headings,
                              const jam::HashMap<const jam::Document::Element*, int>& pageNumbers);

/**
 * @brief Returns the page numbers of each index term.
 *
 * A paragraph or heading holds a term when its text contains the term, ignoring case.
 * The pages of the blocks that hold a term are the pages of that term.
 *
 * @param properties  Properties of the metadata block; the list `index` gives the terms.
 * @param root        Root element of the document.
 * @param body        Component that holds the rows of the document.
 * @param rowPages    Page number of each row.
 * @return The page numbers of each term, in ascending order without repeats.
 */
jam::HashMap<juce::String, jam::Array<int>> getTermPages (const juce::NamedValueSet& properties,
                                                          const jam::Document::Element& root,
                                                          const jam::MarkdownComponent& body,
                                                          const jam::HashMap<int, int>& rowPages);

/**
 * @brief Returns the markdown text of the index.
 *
 * The text starts with a heading. The terms follow in sorted order, grouped under a heading of
 * their first letter. Each term lists its page numbers as links.
 *
 * @param termPages  Page numbers of each term.
 * @return The index text; empty when there is no term.
 */
juce::String getIndexText (const jam::HashMap<juce::String, jam::Array<int>>& termPages);

/**
 * @brief Returns the row range of each chapter.
 *
 * The first chapter starts at the first block that is not a metadata block. A heading starts a new
 * chapter when @p elementStyle gives its tag the `break-before` value `page`. Each chapter
 * ends where the next one starts. The last chapter ends at the last row. An empty range is dropped.
 *
 * @param root          Root element of the document.
 * @param body          Component that holds the rows of the document.
 * @param elementStyle  The element style tree of the stylesheet.
 * @return The half-open row range of each chapter.
 */
jam::Array<juce::Range<int>> getChapterRows (const jam::Document::Element& root,
                                             const jam::MarkdownComponent& body,
                                             const juce::ValueTree& elementStyle);

/**
 * @brief Returns the named strings that the margin boxes of one page show.
 *
 * Each element style record with a `string-set` property adds one string under that name.
 * The record for the title gives the title of the document. The record for a heading tag gives the
 * text of the first heading of that tag on the page. A page with no such heading takes the last
 * heading of that tag before the page. A page with neither takes an empty string.
 *
 * @param properties    Properties of the metadata block of the manual.
 * @param headings      Headings of the manual, in source order.
 * @param body          Component that holds the rows of the document.
 * @param elementStyle  The element style tree of the stylesheet.
 * @param rows          Half-open row range of the page.
 * @return The strings, keyed by name.
 */
juce::NamedValueSet getPageStrings (const juce::NamedValueSet& properties,
                                    const jam::Array<const jam::Document::Element*>& headings,
                                    const jam::MarkdownComponent& body,
                                    const juce::ValueTree& elementStyle,
                                    juce::Range<int> rows);

/**
 * @brief Paints the six page-margin boxes of one page.
 *
 * A top box fills the top margin and a bottom box fills the bottom margin. A box uses the style of
 * @p pageType when that style sets `content` for the box. Otherwise it uses the base page style.
 * A property that the specific style lacks falls back to the base style.
 * The content is a text, a `counter()` of the page number or a `string()` of @p strings.
 * A box with no text paints nothing. A box with text paints the text with its `text-transform`,
 * font, colour and justification. Then it paints the bottom border and the top border.
 *
 * @param g           Graphics context to paint into.
 * @param pageStyle   The page style tree of the stylesheet.
 * @param pageType    Name of the page type, the key of the specific page style.
 * @param page        Bounds of the page.
 * @param margin      Height of the top and bottom margin, and the side inset of each box.
 * @param pageNumber  Number of the page.
 * @param strings     Named strings of the page from getPageStrings().
 */
void paintMarginBoxes (juce::Graphics& g,
                       const juce::ValueTree& pageStyle,
                       const juce::Identifier& pageType,
                       juce::Rectangle<float> page,
                       float margin,
                       int pageNumber,
                       const juce::NamedValueSet& strings);
}// namespace manual
