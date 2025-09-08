// SAXContentHandlerImpl.h: interface for the SAXContentHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "XSPlatform.h"
#include <string>

#if 0
// Keep the original inheritance from ISAXContentHandler on Windows
class CSAXContentHandlerImpl : public ISAXContentHandler
{
public:
    CSAXContentHandlerImpl();
    virtual ~CSAXContentHandlerImpl();

    virtual HRESULT STDMETHODCALLTYPE putDocumentLocator(ISAXLocator *pLocator) override;
    virtual HRESULT STDMETHODCALLTYPE startDocument() override;
    virtual HRESULT STDMETHODCALLTYPE endDocument() override;
    virtual HRESULT STDMETHODCALLTYPE startPrefixMapping(const char*pwchPrefix, int cchPrefix, const char*pwchUri, int cchUri) override;
    virtual HRESULT STDMETHODCALLTYPE endPrefixMapping(const char*pwchPrefix, int cchPrefix) override;
    virtual HRESULT STDMETHODCALLTYPE startElement(const char*pwchNamespaceUri, int cchNamespaceUri, const char*pwchLocalName, int cchLocalName, const char*pwchRawName, int cchRawName, ISAXAttributes *pAttributes) override;
    virtual HRESULT STDMETHODCALLTYPE endElement(const char*pwchNamespaceUri, int cchNamespaceUri, const char*pwchLocalName, int cchLocalName, const char*pwchRawName, int cchRawName) override;
    virtual HRESULT STDMETHODCALLTYPE characters(const char*pwchChars, int cchChars) override;
    virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace(const char*pwchChars, int cchChars) override;
    virtual HRESULT STDMETHODCALLTYPE processingInstruction(const char*pwchTarget, int cchTarget, const char*pwchData, int cchData) override;
    virtual HRESULT STDMETHODCALLTYPE skippedEntity(const char*pwchName, int cchName) override;
};
#else
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
    virtual HRESULT startElement(const char* /*pwchNamespaceUri*/, int /*cchNamespaceUri*/,
                                 const char* /*pwchLocalName*/, int /*cchLocalName*/,
                                 const char* /*pwchRawName*/, int /*cchRawName*/,
                                 const PortableAttributes* /*pAttributes*/) { return S_OK; }

    virtual HRESULT endElement(const char*, int, const char*, int, const char*, int) { return S_OK; }
    virtual HRESULT characters(const char*, int) { return S_OK; }
    virtual HRESULT ignorableWhitespace(const char*, int) { return S_OK; }
    virtual HRESULT processingInstruction(const char*, int, const char*, int) { return S_OK; }
    virtual HRESULT skippedEntity(const char*, int) { return S_OK; }
};
#endif
