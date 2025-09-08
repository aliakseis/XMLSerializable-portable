#include "XSPlatform.h"
//#include <codecvt>
//#include <locale>

#include "SAXObjectContentHandlerImpl.h"
#include "XSBase.h"

/*
#define RETURN_FAILED_HR(statement)	\
{									\
	HRESULT hr = (statement);		\
	if (FAILED(hr))					\
	{								\
		ATLASSERT(FALSE);			\
		return hr;					\
	}								\
}
*/

#ifndef E_FAIL
#define E_FAIL (-1)
#endif

CSAXObjectContentHandlerImpl::CSAXObjectContentHandlerImpl()
{
}

CSAXObjectContentHandlerImpl::~CSAXObjectContentHandlerImpl()
{
}

#if 1
HRESULT CSAXObjectContentHandlerImpl::startElement( 
    /* [in] */ const char* pwchLocalName,
    /* [in] */ int cchLocalName,
    /* [in] */ const PortableAttributes* pAttributes)
{
	CXmlSerializable* pObject = NULL;

	if (m_stObjects.empty())
	{
		std::string strLocalName(pwchLocalName, pwchLocalName + cchLocalName);
		CMapObjects::iterator iter = m_mapRootObjects.find(strLocalName);
		if (iter != m_mapRootObjects.end())
			pObject = iter->second;
	}
	else
	{
		CXmlSerializable* pParent = m_stObjects.top();
		if (pParent != NULL)
		{
			// Embedded objects
			const CMetaInfo* pMetaInfo = pParent->GetMetaInfo();
			if (pMetaInfo != NULL)
			{
				STRING_DATA stringData;
				stringData.pwsz = (const char*) pwchLocalName;
				stringData.nLength = cchLocalName;

				ADD_OBJ_FUNC_PAIR* pEnd
					= pMetaInfo->m_pAddObjFunc + pMetaInfo->m_nAddObjFuncSize;
				ADD_OBJ_FUNC_PAIR* pFind
					= std::find(pMetaInfo->m_pAddObjFunc, pEnd, stringData);
				if (pFind != pEnd)
				{
					pObject = pFind->second(pParent);
				}
			}
		}
	}

	if (pObject != NULL)
	{
		m_stObjects.push(pObject);

		const CMetaInfo* pMetaInfo = pObject->GetMetaInfo();
		//int nLength = 0;
		if (pMetaInfo != NULL && pAttributes != NULL
			//&& (nLength = pAttributes->getLength())
            )
		{	// Object properties
			for (const auto& attr : pAttributes->items)
			{
                STRING_DATA stringData{ attr.localName.c_str(), attr.localName.length() };  //{0};

				//RETURN_FAILED_HR(pAttributes->getLocalName(
				//	i, (const char**)&stringData.pwsz, &stringData.nLength));

				SET_DATA_FUNC_PAIR* pEnd 
					= pMetaInfo->m_pSetDataFunc + pMetaInfo->m_nSetDataFuncSize;
				SET_DATA_FUNC_PAIR* pFind 
					= std::find(pMetaInfo->m_pSetDataFunc, pEnd, stringData);
				if (pFind != pEnd)
				{
					const char*pwchValue = attr.value.c_str();
					int cchValue = attr.value.length();
					//RETURN_FAILED_HR(pAttributes->getValue(i, &pwchValue, &cchValue));
					if (!pFind->second(pObject, (const char*) pwchValue, cchValue))
					{
						return E_FAIL;
					}
				}
			}
		}
	}

	m_stObjPushedFlags.push(pObject != NULL);

    return S_OK;
}
#endif   
       
HRESULT CSAXObjectContentHandlerImpl::endElement( 
            /* [in] */ const char*,
            /* [in] */ int )
{
	if (m_stObjPushedFlags.top())
		m_stObjects.pop();
	m_stObjPushedFlags.pop();

    return S_OK;
}
        
HRESULT CSAXObjectContentHandlerImpl::characters( 
            /* [in] */ const char*,
            /* [in] */ int )
{
    return S_OK;
}
        
void CSAXObjectContentHandlerImpl::AddRootObject(const char* pwszName, CXmlSerializable* pObject)
{
	m_mapRootObjects[pwszName] = pObject;
}

const char wszMSSQLError[] = "MSSQLError";
const char wszHResult[] = "HResult=\"0x";

HRESULT CSAXObjectContentHandlerImpl::processingInstruction( 
            /* [in] */ const char*pwchTarget,
            /* [in] */ int cchTarget,
            /* [in] */ const char*pwchData,
            /* [in] */ int cchData)
{
	//MSSQLError
	if (cchTarget == sizeof(wszMSSQLError) / sizeof(wszMSSQLError[0]) - 1 
		&& !memcmp(wszMSSQLError, pwchTarget, sizeof(wszMSSQLError) - sizeof(char)))
	{
		if (cchData > sizeof(wszHResult) / sizeof(wszHResult[0]) - 1 
			&& !memcmp(wszHResult, pwchData, sizeof(wszHResult) - sizeof(char)))
		{
			long hr = strtoul((const char*)pwchData + sizeof(wszHResult) / sizeof(wszHResult[0]) - 1, NULL, 16);
			if (hr < 0)
				return hr;
		}
		return E_FAIL;
	}
    return S_OK;
}

#if 0
// Portable startElement: build STRING_DATA from PortableAttributes
HRESULT CSAXObjectContentHandlerImpl::startElement(
        const char*pwchRawName, int cchRawName,
        const PortableAttributes *pAttributes)
{
    m_stObjPushedFlags.push(false);
    CXmlSerializable* pParentObject = !m_stObjects.empty() ? m_stObjects.top() : NULL;

    if (!pParentObject)
    {   // Root object?
        CMapObjects::iterator it = m_mapRootObjects.find(std::string((char*)pwchRawName, cchRawName));
        if (it == m_mapRootObjects.end())
        {
            //ATLASSERT(FALSE);
            return E_FAIL;
        }
        CXmlSerializable* pObject = it->second;
        if (pObject)
        {
            m_stObjects.push(pObject);

            const CMetaInfo* pMetaInfo = pObject->GetMetaInfo();
            if (pMetaInfo && pAttributes)
            {
                // convert attribute names/values to wide strings and keep them alive during processing
                //std::vector<std::string> convNames; convNames.reserve(pAttributes->getLength());
                //std::vector<std::string> convVals; convVals.reserve(pAttributes->getLength());
                //for (int i = 0; i < pAttributes->getLength(); ++i)
                for (const auto& attr: pAttributes->items)
                {
                    //const auto &attr = pAttributes->at(i);
                    //std::wstring_convert<std::codecvt_utf8_utf16<char>> conv;
                    //convNames.push_back(attr.localName);
                    //convVals.push_back(attr.value);
                //}
                //for (int i = 0; i < pAttributes->getLength(); ++i)
                //{
                    STRING_DATA stringData{};
                    stringData.pwsz = attr.localName.c_str();
                    stringData.nLength = static_cast<int>(attr.localName.size());

                    SET_DATA_FUNC_PAIR* pEnd = pMetaInfo->m_pSetDataFunc + pMetaInfo->m_nSetDataFuncSize;
                    SET_DATA_FUNC_PAIR* pFind = std::find(pMetaInfo->m_pSetDataFunc, pEnd, stringData);
                    if (pFind != pEnd)
                    {
                        if (!pFind->second(pObject, attr.value.c_str(), static_cast<int>(attr.value.size())))
                            return E_FAIL;
                    }
                }
            }
        }
        return S_OK;
    }

    // Child object?
    {
        const CMetaInfo* pMetaInfo = pParentObject->GetMetaInfo();
        if (!pMetaInfo)
            return E_FAIL;

        CXmlSerializable* pObject = NULL;

        STRING_DATA stringData{};
        stringData.pwsz = (const char*)pwchRawName;
        stringData.nLength = cchRawName;

        ADD_OBJ_FUNC_PAIR* pEnd = pMetaInfo->m_pAddObjFunc + pMetaInfo->m_nAddObjFuncSize;
        ADD_OBJ_FUNC_PAIR* pFind = std::find(pMetaInfo->m_pAddObjFunc, pEnd, stringData);
        if (pFind != pEnd)
        {
            pObject = pFind->second(pParentObject);
        }

        if (!pObject)
        {
            // Simple value inside parent?
            stringData.pwsz = ""; stringData.nLength = 0;
            m_stObjects.push(pParentObject);
            return S_OK;
        }

        m_stObjPushedFlags.top() = true;
        m_stObjects.push(pObject);

        const CMetaInfo* pChildMeta = pObject->GetMetaInfo();
        if (pChildMeta && pAttributes)
        {
            //std::vector<std::string> convNames; convNames.reserve(pAttributes->getLength());
            //std::vector<std::string> convVals; convVals.reserve(pAttributes->getLength());
            //std::wstring_convert<std::codecvt_utf8_utf16<char>> conv;
            //for (int i = 0; i < pAttributes->getLength(); ++i)
            for (const auto& attr : pAttributes->items)
            {
                //const auto &attr = pAttributes->at(i);
                //convNames.push_back(attr.localName);
                //convVals.push_back(attr.value);
            //}
            //for (int i = 0; i < pAttributes->getLength(); ++i)
            //{
                STRING_DATA stringData2{};
                stringData2.pwsz = attr.localName.c_str();
                stringData2.nLength = static_cast<int>(attr.localName.size());
                SET_DATA_FUNC_PAIR* pEnd2 = pChildMeta->m_pSetDataFunc + pChildMeta->m_nSetDataFuncSize;
                SET_DATA_FUNC_PAIR* pFind2 = std::find(pChildMeta->m_pSetDataFunc, pEnd2, stringData2);
                if (pFind2 != pEnd2)
                {
                    if (!pFind2->second(pObject, attr.value.c_str(), static_cast<int>(attr.value.size())))
                        return E_FAIL;
                }
            }
        }
    }
    return S_OK;
}
#endif
