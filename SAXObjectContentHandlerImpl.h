#pragma once
#include "SAXContentHandlerImpl.h"
#include "PortableSAX.h"

#include <map>
#include <stack>

class CXmlSerializable;

class CSAXObjectContentHandlerImpl :
	public CSAXContentHandlerImpl
{
public:
	CSAXObjectContentHandlerImpl();
	~CSAXObjectContentHandlerImpl();

	void AddRootObject(const char* pwszName, CXmlSerializable* pObject); 

protected:
    HRESULT startElement( 
        /* [in] */ const char*pwchRawName,
        /* [in] */ int cchRawName,
        /* [in] */ const PortableAttributes* pAttributes) override;
    
    HRESULT endElement( 
        /* [in] */ const char*pwchRawName,
        /* [in] */ int cchRawName) override;
    
    HRESULT characters( 
        /* [in] */ const char*pwchChars,
        /* [in] */ int cchChars) override;

	HRESULT processingInstruction( 
            /* [in] */ const char*pwchTarget,
            /* [in] */ int cchTarget,
            /* [in] */ const char*pwchData,
            /* [in] */ int cchData) override;

private:
	typedef std::map<std::string, CXmlSerializable*> CMapObjects;
	CMapObjects m_mapRootObjects;

	std::stack<bool> m_stObjPushedFlags;
	std::stack<CXmlSerializable*> m_stObjects;
};
