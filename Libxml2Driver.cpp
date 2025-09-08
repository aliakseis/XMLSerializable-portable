#include "XSPlatform.h"
#include "SAXObjectContentHandlerImpl.h"
#include "PortableSAX.h"

#if !0
#include <libxml/parser.h>
#include <libxml/xmlstring.h>
#include <string>
//#include <codecvt>
//#include <locale>

struct DriverCtx {
    CSAXContentHandlerImpl* handler{};
    std::string textBuffer; // UTF-8 buffer
};

// Helper to convert UTF-8 string to std::string
/*
static std::string utf8_to_wstring(const std::string& s) {
    if (s.empty()) return {};
    std::wstring_convert<std::codecvt_utf8_utf16<char>> conv;
    return conv.from_bytes(s);
}
*/

static void startElementNs(void *userData,
                           const xmlChar *localname,
                           const xmlChar *prefix,
                           const xmlChar *URI,
                           int nb_namespaces,
                           const xmlChar **namespaces,
                           int nb_attributes,
                           int nb_defaulted,
                           const xmlChar **attributes)
{
    (void)prefix; (void)URI; (void)nb_namespaces; (void)namespaces; (void)nb_defaulted;
    DriverCtx* ctx = static_cast<DriverCtx*>(userData);

    PortableAttributes attrs;
    // attributes array layout: for each attribute i: localname, prefix, URI, valueBegin, valueEnd
    for (int i = 0; i < nb_attributes; ++i) {
        const xmlChar* lname = attributes[i*5 + 0];
        const xmlChar* value = attributes[i*5 + 3];
        if (!lname) continue;
        PortableAttribute pa;
        pa.localName = reinterpret_cast<const char*>(lname);
        if (value) pa.value = reinterpret_cast<const char*>(value);
        attrs.items.push_back(std::move(pa));
    }

    std::string local_utf8;
    if (localname) local_utf8 = reinterpret_cast<const char*>(localname);
    //std::string wlocal = utf8_to_wstring(local_utf8);

    const char* rawName = nullptr;
    if (!local_utf8.empty()) rawName = (const char*)local_utf8.c_str();

    ctx->handler->startElement(rawName, (int)local_utf8.size(), &attrs);
    ctx->textBuffer.clear();
}

static void endElementNs(void *userData,
                         const xmlChar *localname,
                         const xmlChar *prefix,
                         const xmlChar *URI)
{
    (void)prefix; (void)URI;
    DriverCtx* ctx = static_cast<DriverCtx*>(userData);

    std::string local_utf8;
    if (localname) local_utf8 = reinterpret_cast<const char*>(localname);
    //std::string wlocal = utf8_to_wstring(local_utf8);

    // Flush any buffered text
    if (!ctx->textBuffer.empty()) {
        //std::string wtxt = utf8_to_wstring(ctx->textBuffer);
        const char* txt = (const char*)ctx->textBuffer.c_str();
        ctx->handler->characters(txt, (int)ctx->textBuffer.size());
        ctx->textBuffer.clear();
    }

    const char* rawName = nullptr;
    if (!local_utf8.empty()) rawName = (const char*)local_utf8.c_str();

    ctx->handler->endElement(rawName, (int)local_utf8.size());
}

static void characters(void *userData, const xmlChar *ch, int len)
{
    DriverCtx* ctx = static_cast<DriverCtx*>(userData);
    if (ch && len > 0) {
        ctx->textBuffer.append(reinterpret_cast<const char*>(ch), len);
    }
}

int ParseXmlWithLibxml2(const char* utf8text, CSAXObjectContentHandlerImpl* handler)
{
    xmlSAXHandler sax = {};
    sax.initialized = XML_SAX2_MAGIC;
    sax.startElementNs = startElementNs;
    sax.endElementNs = endElementNs;
    sax.characters = characters;

    DriverCtx ctx; ctx.handler = handler;
    if (!utf8text) return -1;
    int ret = xmlSAXUserParseMemory(&sax, &ctx, utf8text, (int)strlen(utf8text));
    return ret == 0 ? 0 : -1;
}
#endif
