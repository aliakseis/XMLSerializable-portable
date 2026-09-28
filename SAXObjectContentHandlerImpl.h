#pragma once

#include "SAXContentHandlerImpl.h"
#include <map>
#include <stack>

class CXmlSerializable;

class XmlObjectContentHandler final : public XmlContentHandler
{
public:
    void AddRootObject(const std::string& name, CXmlSerializable* object);

    bool startElement(const char* localName, std::size_t nameLength,
                      const PortableAttributes* attributes) override;
    bool endElement(const char* localName, std::size_t nameLength) override;
    bool characters(const char* text, std::size_t length) override;
    bool processingInstruction(const char* target, std::size_t targetLength,
                               const char* data, std::size_t dataLength) override;

private:
    std::map<std::string, CXmlSerializable*> m_rootObjects;
    std::stack<bool> m_objectPushed;
    std::stack<CXmlSerializable*> m_objects;
};

int ParseXmlWithLibxml2(const char* utf8Text, XmlContentHandler* handler);
