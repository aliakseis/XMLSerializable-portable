#include "SAXObjectContentHandlerImpl.h"
#include "XSBase.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

void XmlObjectContentHandler::AddRootObject(const std::string& name,
                                            CXmlSerializable* object)
{
    m_rootObjects[name] = object;
}

bool XmlObjectContentHandler::startElement(const char* localName,
                                            std::size_t nameLength,
                                            const PortableAttributes* attributes)
{
    if (!localName && nameLength != 0)
        return false;

    const std::string name(localName ? localName : "", nameLength);
    CXmlSerializable* object = nullptr;

    if (m_objects.empty()) {
        const auto it = m_rootObjects.find(name);
        if (it != m_rootObjects.end())
            object = it->second;
    } else if (CXmlSerializable* parent = m_objects.top()) {
        if (const CMetaInfo* info = parent->GetMetaInfo()) {
            const STRING_DATA key{ name.c_str(), static_cast<int>(name.size()) };
            if (info->m_pAddObjFunc && info->m_nAddObjFuncSize > 0) {
                auto* end = info->m_pAddObjFunc + info->m_nAddObjFuncSize;
                auto* found = std::find(info->m_pAddObjFunc, end, key);
                if (found != end)
                    object = found->second(parent);
            }
        }
    }

    if (object) {
        m_objects.push(object);
        if (const CMetaInfo* info = object->GetMetaInfo(); info && attributes) {
            for (const auto& attr : attributes->items) {
                const STRING_DATA key{ attr.localName.c_str(),
                                       static_cast<int>(attr.localName.size()) };
                if (info->m_pSetDataFunc && info->m_nSetDataFuncSize > 0) {
                    auto* end = info->m_pSetDataFunc + info->m_nSetDataFuncSize;
                    auto* found = std::find(info->m_pSetDataFunc, end, key);
                    if (found != end &&
                        !found->second(object, attr.value.data(), attr.value.size()))
                        return false;
                }
            }
        }
    }

    m_objectPushed.push(object != nullptr);
    return true;
}

bool XmlObjectContentHandler::endElement(const char*, std::size_t)
{
    if (m_objectPushed.empty())
        return false;
    if (m_objectPushed.top()) {
        if (m_objects.empty())
            return false;
        m_objects.pop();
    }
    m_objectPushed.pop();
    return true;
}

bool XmlObjectContentHandler::characters(const char*, std::size_t)
{
    return true;
}

bool XmlObjectContentHandler::processingInstruction(const char* target,
                                                      std::size_t targetLength,
                                                      const char* data,
                                                      std::size_t dataLength)
{
    static constexpr char sqlError[] = "MSSQLError";
    static constexpr char hresultPrefix[] = "HResult=\"0x";

    if (target && targetLength == sizeof(sqlError) - 1 &&
        std::memcmp(target, sqlError, sizeof(sqlError) - 1) == 0) {
        if (data && dataLength > sizeof(hresultPrefix) - 1 &&
            std::memcmp(data, hresultPrefix, sizeof(hresultPrefix) - 1) == 0) {
            char* end = nullptr;
            const unsigned long value = std::strtoul(
                data + sizeof(hresultPrefix) - 1, &end, 16);
            if (end != data + sizeof(hresultPrefix) - 1 && value != 0)
                return false;
        }
        return false;
    }
    return true;
}
