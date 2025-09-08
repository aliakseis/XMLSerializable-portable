// File: XSInternal.h

#pragma once

#include <string>
#include <ostream>

inline bool IsSerializatonNeeded(...)
{
    return true;
}


template<typename T> 
inline bool IsSerializatonNeeded(const std::basic_string<T>& str)
{
    return !str.empty();
}

//inline 
bool IsSerializatonNeeded(const GUID& rGuid);


typedef bool (* SET_DATA_FUNC)(CXmlSerializable*, const char*, size_t);
typedef CXmlSerializable* (* ADD_NEW_OBJECT_FUNC)(CXmlSerializable*);


template<typename T>
inline bool Convert(const char* pwsz, size_t nLength, T& out)
{
    if (0 == nLength)
    {
        return true;
    }
    return !(std::wistringstream(std::string(pwsz, nLength)) >> out).fail();
}


template<>
inline bool Convert(const char* pwsz, size_t nLength, GUID& out)
{
    if (!pwsz) return false;
    // Parse formats: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx or with braces
    std::string s(pwsz, nLength);
    // remove braces
    if (!s.empty() && s.front() == L'{') s = s.substr(1);
    if (!s.empty() && s.back() == L'}') s = s.substr(0, s.size()-1);
    // expected 36 chars with hyphens
    if (s.size() != 36) return false;
    auto hex = [&](char c)->int {
        if (c>='0' && c<='9') return c-'0';
        if (c>='a' && c<='f') return 10 + (c-'a');
        if (c>='A' && c<='F') return 10 + (c-'A');
        return -1;
    };
    auto parse_hex = [&](int start, int len)->uint64_t {
        uint64_t v = 0;
        for (int i=0;i<len;i++){
            int h = hex(s[start+i]); if (h<0) return (uint64_t)-1;
            v = (v<<4) | (uint64_t)h;
        }
        return v;
    };
    uint64_t a = parse_hex(0,8);
    uint64_t b = parse_hex(9,4);
    uint64_t c = parse_hex(14,4);
    uint64_t d = parse_hex(19,4);
    uint64_t e = parse_hex(24,12);
    if (a==(uint64_t)-1 || b==(uint64_t)-1 || c==(uint64_t)-1 || d==(uint64_t)-1 || e==(uint64_t)-1) return false;
    out.Data1 = (uint32_t)a;
    out.Data2 = (uint16_t)b;
    out.Data3 = (uint16_t)c;
    uint64_t ee = e;
    // fill Data4: first two bytes from d, remaining 6 bytes from e
    out.Data4[0] = (uint8_t)((d >> 8) & 0xFF);
    out.Data4[1] = (uint8_t)(d & 0xFF);
    for (int i=0;i<6;i++) {
        out.Data4[2+i] = (uint8_t)((ee >> ((5-i)*8)) & 0xFF);
    }
    return true;
}

template<>
inline bool Convert(const char* pwsz, size_t nLength, std::string& out)
{
    out.assign(pwsz, nLength);
    return true;
}


template<typename T> 
inline bool ReadField(CXmlSerializable* , T& field            
            , const char* pwszData, size_t nLength)
{
    return Convert(pwszData, nLength, field);                                
}


template<typename T> 
inline void WriteField(std::ostream& s, const T& data)
{
    std::ostringstream ostr;
    ostr << data;

    // TODO escape
    s << ostr.str();
}


template<> 
inline void WriteField(std::ostream& s, const long& data)
{
    s << data;
}


template<> 
//inline 
void WriteField(std::ostream& s, const GUID& data);

#pragma intrinsic(memcmp)

// STRING_DATA construction should be re-enterable 
struct STRING_DATA
{
    const char* pwsz;
    int nLength;

    template<typename T> STRING_DATA& operator = (const T& src)
    {
        // Use cast to ensure that we only allow character arrays
        (static_cast<const char[sizeof(src) / sizeof(src[0])]>(src));
        pwsz = src;
        nLength = sizeof(src) / sizeof(src[0]) - 1;
        return *this;
    }

    bool operator == (const STRING_DATA& other) const
    {
        return nLength == other.nLength 
            && !memcmp(pwsz, other.pwsz, nLength * sizeof(char));
    }
};

#pragma function(memcmp)

template<typename T>
inline bool operator == (const std::pair<STRING_DATA, T>& left, const STRING_DATA& right)
{
    return left.first == right;
}


template<typename T> struct STRING_FUNC_PAIR
{
    STRING_DATA first;
    T second;
};

template<typename T> 
bool operator == (const STRING_FUNC_PAIR<T>& left, const STRING_DATA& right)
{
    return left.first == right;
}

typedef STRING_FUNC_PAIR<SET_DATA_FUNC> SET_DATA_FUNC_PAIR;
typedef STRING_FUNC_PAIR<ADD_NEW_OBJECT_FUNC> ADD_OBJ_FUNC_PAIR;


// CMetaInfo construction should be re-enterable 
// Inilialized in descendants
struct CMetaInfo
{
    SET_DATA_FUNC_PAIR* m_pSetDataFunc;
    int m_nSetDataFuncSize;

    ADD_OBJ_FUNC_PAIR* m_pAddObjFunc;
    int m_nAddObjFuncSize;
};

template<typename T, int size>
class CMetaInfoData
{
    SET_DATA_FUNC_PAIR m_arrSetDataFunc[size];    
public:
    CMetaInfoData()
    {
        static_cast<T*>(this)->m_pSetDataFunc = m_arrSetDataFunc;
        static_cast<T*>(this)->m_nSetDataFuncSize = size;
    }
};

template<typename T>
class CMetaInfoData<T, 0>
{
public:
    CMetaInfoData()
    {
        static_cast<T*>(this)->m_pSetDataFunc = NULL;
        static_cast<T*>(this)->m_nSetDataFuncSize = 0;
    }
};


template<typename T, int size>
class CMetaInfoObj
{
    ADD_OBJ_FUNC_PAIR    m_arrAddObjFunc[size];
public:
    CMetaInfoObj()
    {
        static_cast<T*>(this)->m_pAddObjFunc = m_arrAddObjFunc;
        static_cast<T*>(this)->m_nAddObjFuncSize = size;
    }
};

template<typename T>
class CMetaInfoObj<T, 0>
{
public:
    CMetaInfoObj()
    {
        static_cast<T*>(this)->m_pAddObjFunc = NULL;
        static_cast<T*>(this)->m_nAddObjFuncSize = 0;
    }
};

template<typename T>
inline T* Append(std::deque<T>& v)
{
    size_t nSize = v.size();                                            
    v.resize(nSize + 1);                                            
    T* pChild = &v[nSize];
    return pChild;
}


#define META_PROP_INTERNAL(name)                                                \
    private:                                                                    \
        static bool ClassSet##name(CXmlSerializable* pObj                       \
            , const char* pwszData, size_t nLength)                          \
        {                                                                       \
            return ReadField(pObj, static_cast<TheClass*>(pObj)->m_##name       \
                , pwszData, nLength);                                           \
        }                                                                       \
        template<int id> struct CDescriptor<id, __COUNTER__-ID_REFERENCE_POINT> \
        {                                                                       \
            enum                                                                \
            {                                                                   \
                ESetDataFuncPos = CDescriptor<id-1>::ESetDataFuncPos + 1,       \
                EAddObjFuncPos = CDescriptor<id-1>::EAddObjFuncPos              \
            };                                                                  \
            static void Setup(CMetaInfo* pInfo)                                 \
            {                                                                   \
                CDescriptor<id-1>::Setup(pInfo);                                \
                pInfo->m_pSetDataFunc[ESetDataFuncPos].first = #name;          \
                pInfo->m_pSetDataFunc[ESetDataFuncPos].second = ClassSet##name; \
            }                                                                   \
            static void SaveAttributes(const TheClass* pObj                     \
                , std::ostream& s)                                             \
            {                                                                   \
                CDescriptor<id-1>::SaveAttributes(pObj, s);                     \
                if (IsSerializatonNeeded(pObj->m_##name))                       \
                {                                                               \
                    s << " " #name "=\"";                                    \
                    WriteField(s, pObj->m_##name);                              \
                    s << "\"";                                                 \
                }                                                               \
            }                                                                   \
            static void SaveElements(const TheClass* pObj                       \
                , std::ostream& s)                                             \
            {                                                                   \
                CDescriptor<id-1>::SaveElements(pObj, s);                       \
            }                                                                   \
        };

#define META_OBJECTS_INTERNAL(name)                                             \
    private:                                                                    \
        static CXmlSerializable* ClassAddNew##name(CXmlSerializable* pObj)      \
        {                                                                       \
            auto& v = static_cast<TheClass*>(pObj)->m_v##name;                  \
            return Append(v);                                                   \
        }                                                                       \
        template<int id> struct CDescriptor<id, __COUNTER__-ID_REFERENCE_POINT> \
        {                                                                       \
            enum                                                                \
            {                                                                   \
                ESetDataFuncPos = CDescriptor<id-1>::ESetDataFuncPos,           \
                EAddObjFuncPos = CDescriptor<id-1>::EAddObjFuncPos + 1          \
            };                                                                  \
            static void Setup(CMetaInfo* pInfo)                                 \
            {                                                                   \
                CDescriptor<id-1>::Setup(pInfo);                                \
                pInfo->m_pAddObjFunc[EAddObjFuncPos].first = #name;            \
                pInfo->m_pAddObjFunc[EAddObjFuncPos].second = ClassAddNew##name;\
            }                                                                   \
            static void SaveAttributes(const TheClass* pObj                     \
                , std::ostream& s)                                             \
            {                                                                   \
                CDescriptor<id-1>::SaveAttributes(pObj, s);                     \
            }                                                                   \
            static void SaveElements(const TheClass* pObj                       \
                , std::ostream& s)                                             \
            {                                                                   \
                CDescriptor<id-1>::SaveElements(pObj, s);                       \
                for (const auto& v : pObj->m_v##name)                           \
                    v.ToXml(s, #name);                                         \
            }                                                                   \
        };

#define BEGIN_META_MAP(classname)                                               \
        template<int id, int> friend struct CDescriptor;                        \
    private:                                                                    \
        enum { ID_REFERENCE_POINT = __COUNTER__ };                              \
        typedef classname TheClass;                                             \
        template<int id, int=id> struct CDescriptor                             \
        {                                                                       \
            enum                                                                \
            {                                                                   \
                ESetDataFuncPos = CDescriptor<id-1>::ESetDataFuncPos,           \
                EAddObjFuncPos = CDescriptor<id-1>::EAddObjFuncPos              \
            };                                                                  \
            static void Setup(CMetaInfo* pInfo)                                 \
            {                                                                   \
                CDescriptor<id-1>::Setup(pInfo);                                \
            }                                                                   \
            static void SaveAttributes(const TheClass* pObj                     \
                , std::ostream& s)                                             \
            {                                                                   \
                CDescriptor<id-1>::SaveAttributes(pObj, s);                     \
            }                                                                   \
            static void SaveElements(const TheClass* pObj                       \
                , std::ostream& s)                                             \
            {                                                                   \
                CDescriptor<id-1>::SaveElements(pObj, s);                       \
            }                                                                   \
        };                                                                      \
        template<int id> struct CDescriptor<id, 0>                              \
        {                                                                       \
            enum                                                                \
            {                                                                   \
                ESetDataFuncPos = -1,                                           \
                EAddObjFuncPos = -1                                             \
            };                                                                  \
            static void Setup(CMetaInfo*) {}                                    \
            static void SaveAttributes(const TheClass*, std::ostream&) {}      \
            static void SaveElements(const TheClass*, std::ostream&) {}        \
        };

#define END_META_MAP                                                            \
    private:                                                                    \
        typedef CDescriptor<__COUNTER__ - ID_REFERENCE_POINT - 1> CFinalDescr;  \
        struct CFinalMetaInfo : public CMetaInfo                                \
        , public CMetaInfoData<CFinalMetaInfo, CFinalDescr::ESetDataFuncPos + 1>\
        , public CMetaInfoObj<CFinalMetaInfo, CFinalDescr::EAddObjFuncPos + 1>  \
        {                                                                       \
            CFinalMetaInfo()                                                    \
            {                                                                   \
                CFinalDescr::Setup(this);                                       \
            }                                                                   \
        };                                                                      \
        XS_API const CMetaInfo* GetMetaInfo() const override                    \
        {                                                                       \
            static const CFinalMetaInfo g_metaInfo;                             \
            return &g_metaInfo;                                                 \
        }                                                                       \
        static void ToXml(const TheClass* pObj, std::ostream& s                \
            , const char* pwszTag)                                                  \
        {                                                                       \
            s << "<" << pwszTag;                                               \
            CFinalDescr::SaveAttributes(pObj, s);                               \
            if (CFinalDescr::EAddObjFuncPos > -1)                               \
            {                                                                   \
                s << ">";                                                      \
                CFinalDescr::SaveElements(pObj, s);                             \
                s << "</" << pwszTag << ">";                                  \
            }                                                                   \
            else                                                                \
                s << "/>";                                                     \
        }                                                                       \
    public:                                                                     \
        XS_API void ToXml(std::ostream& s                                      \
            , const char* pwszRoot = NULL) const override                           \
        {                                                                       \
            ToXml(this, s, (NULL == pwszRoot)? "object" : pwszRoot);           \
        }
