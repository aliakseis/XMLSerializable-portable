#include "SAXObjectContentHandlerImpl.h"

#include <libxml/parser.h>
#include <cstring>
#include <string>
#include <utility>

namespace {
struct ParserContext {
    XmlContentHandler* handler = nullptr;
    std::string text;
    bool failed = false;
};

void startElementNs(void* userData, const xmlChar* localName,
                    const xmlChar*, const xmlChar*, int, const xmlChar**,
                    int attributeCount, int, const xmlChar** attributes)
{
    auto& ctx = *static_cast<ParserContext*>(userData);
    if (ctx.failed) return;

    PortableAttributes parsed;
    for (int i = 0; i < attributeCount; ++i) {
        const xmlChar* name = attributes[i * 5];
        const xmlChar* begin = attributes[i * 5 + 3];
        const xmlChar* end = attributes[i * 5 + 4];
        if (!name) continue;

        PortableAttribute attr;
        attr.localName = reinterpret_cast<const char*>(name);
        if (begin && end && end >= begin)
            attr.value.assign(reinterpret_cast<const char*>(begin),
                              static_cast<std::size_t>(end - begin));
        parsed.items.push_back(std::move(attr));
    }

    const char* name = localName
        ? reinterpret_cast<const char*>(localName) : "";
    ctx.failed = !ctx.handler->startElement(name, std::strlen(name), &parsed);
    ctx.text.clear();
}

void endElementNs(void* userData, const xmlChar* localName,
                  const xmlChar*, const xmlChar*)
{
    auto& ctx = *static_cast<ParserContext*>(userData);
    if (ctx.failed) return;

    if (!ctx.text.empty()) {
        ctx.failed = !ctx.handler->characters(ctx.text.data(), ctx.text.size());
        ctx.text.clear();
        if (ctx.failed) return;
    }

    const char* name = localName
        ? reinterpret_cast<const char*>(localName) : "";
    ctx.failed = !ctx.handler->endElement(name, std::strlen(name));
}

void characters(void* userData, const xmlChar* text, int length)
{
    auto& ctx = *static_cast<ParserContext*>(userData);
    if (!ctx.failed && text && length > 0)
        ctx.text.append(reinterpret_cast<const char*>(text),
                        static_cast<std::size_t>(length));
}
} // namespace

int ParseXmlWithLibxml2(const char* utf8Text, XmlContentHandler* handler)
{
    if (!utf8Text || !handler)
        return -1;

    xmlSAXHandler sax{};
    sax.initialized = XML_SAX2_MAGIC;
    sax.startElementNs = startElementNs;
    sax.endElementNs = endElementNs;
    sax.characters = characters;

    ParserContext context;
    context.handler = handler;
    const int result = xmlSAXUserParseMemory(
        &sax, &context, utf8Text, static_cast<int>(std::strlen(utf8Text)));
    return result == 0 && !context.failed ? 0 : -1;
}
