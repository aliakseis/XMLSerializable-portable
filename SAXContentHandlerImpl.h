// SAXContentHandlerImpl.h: interface for the SAXContentHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "XSPlatform.h"
#include <string>

// Portable shim with the same method names (HRESULT-like return)
using HRESULT = long;
#ifndef S_OK
#define S_OK 0L
#endif

struct ISAXLocator; // not used

struct PortableAttributes; // defined in PortableSAX.h

class CSAXContentHandlerImpl
{
public:
    CSAXContentHandlerImpl() {}
    virtual ~CSAXContentHandlerImpl() {}

    virtual HRESULT putDocumentLocator(ISAXLocator*) { return S_OK; }
    virtual HRESULT startDocument() { return S_OK; }
    virtual HRESULT endDocument() { return S_OK; }
    virtual HRESULT startPrefixMapping(const char*, int, const char*, int) { return S_OK; }
    virtual HRESULT endPrefixMapping(const char*, int) { return S_OK; }

    // Portable overloads
    virtual HRESULT startElement(const char* /*pwchRawName*/, int /*cchRawName*/,
                                 const PortableAttributes* /*pAttributes*/) { return S_OK; }

    virtual HRESULT endElement(const char*, int) { return S_OK; }
    virtual HRESULT characters(const char*, int) { return S_OK; }
    virtual HRESULT ignorableWhitespace(const char*, int) { return S_OK; }
    virtual HRESULT processingInstruction(const char*, int, const char*, int) { return S_OK; }
    virtual HRESULT skippedEntity(const char*, int) { return S_OK; }
};
