#pragma once

#include "PortableSAX.h"

// Parser-independent callbacks. Returning false aborts parsing.
class XmlContentHandler
{
public:
    virtual ~XmlContentHandler() = default;

    virtual bool startElement(const char* localName, std::size_t nameLength,
                              const PortableAttributes* attributes) = 0;
    virtual bool endElement(const char* localName, std::size_t nameLength) = 0;
    virtual bool characters(const char* text, std::size_t length) = 0;
    virtual bool processingInstruction(const char* target, std::size_t targetLength,
                                       const char* data, std::size_t dataLength)
    {
        (void)target; (void)targetLength; (void)data; (void)dataLength;
        return true;
    }
};
