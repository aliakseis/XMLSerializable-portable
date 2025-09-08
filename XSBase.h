// File: XSBase.h

#pragma once

#include "XSPlatform.h"

#include <utility>
#include <deque>
#include <string>

//////////////////////////////////////////////////////////////////////////
// common part



typedef std::string CSTLString;

//////////////////////////////////////////////////////////////////////////
//    XmlSerializable

struct CMetaInfo;

class CXmlSerializable
{
friend class CSAXObjectContentHandlerImpl;

public:
    virtual ~CXmlSerializable() {}

    virtual void ToXml(std::ostream&, const char* /*pwszRoot*/ = NULL) const {}

private:
    virtual const CMetaInfo* GetMetaInfo() const { return NULL; }
};


#ifdef XS_BASE

#include "XSInternal.h"

#else// XS_BASE

#define META_PROP_INTERNAL(name)
#define META_OBJECTS_INTERNAL(name)
#define BEGIN_META_MAP(classname)
#define END_META_MAP                                                        \
    private:                                                                \
        XS_API const CMetaInfo* GetMetaInfo() const override;               \
    public:                                                                 \
        XS_API void ToXml(std::ostream&, const char* = NULL) const override;

#endif// XS_BASE


#define PROP_GETTER(type, name)                                             \
    public:                                                                 \
        type Get_##name() const                                             \
        { return m_##name; }

#define PROP_SETTER(type, name)                                             \
    public:                                                                 \
        void Set_##name(type value)                                         \
        { m_##name = std::move(value); }

#define META_PROP_READONLY(type, name)                                      \
    private:                                                                \
        type m_##name {};                                                   \
    public:                                                                 \
    PROP_GETTER(type, name)                                                 \
    META_PROP_INTERNAL(name)

#define META_PROP(type, name)                                               \
    PROP_SETTER(type, name)                                                 \
    META_PROP_READONLY(type, name)


#define META_OBJECTS(type, name)                                            \
    public:                                                                 \
        std::deque<type > m_v##name;                                        \
    META_OBJECTS_INTERNAL(name)

