// XMLSerializable.cpp : Defines the entry point for the console application.
//

#include "XSPlatform.h"

#include "XSBase.h"

#include "SAXObjectContentHandlerImpl.h"

#include <iostream>
#include <sstream> 

#if !0
extern int ParseXmlWithLibxml2(const char* utf8text, CSAXObjectContentHandlerImpl* handler);
#endif




class CEmployee : public CXmlSerializable
{
public:
	BEGIN_META_MAP(CEmployee)
		META_PROP(CSTLString,	Region)
	END_META_MAP
};


class COrder : public CXmlSerializable
{
	BEGIN_META_MAP(COrder)
		META_PROP(CSTLString,	ShipCity)
		META_PROP(CSTLString,	EmployeeName)
		META_OBJECTS(CEmployee,	Employees)
	END_META_MAP
};

class CCustomer : public CXmlSerializable
{
	BEGIN_META_MAP(CCustomer)
		META_PROP(CSTLString,	CustomerID)
		META_PROP(CSTLString,	ContactName)
		META_OBJECTS(COrder,	Orders)
	END_META_MAP
};

class CCustomersList : public CXmlSerializable
{
	BEGIN_META_MAP(CCustomersList)
		META_OBJECTS(CCustomer,	Customers)
	END_META_MAP
};


// XML and ASP.NET
// By Kirk Evans, Ashwin Kamanna, Joel Mueller.
// Listing 9.4
// http://www.informit.com/articles/article.asp?p=99813
const char g_pszXml[] = R"(<ROOT>
<Customers CustomerID='ALFKI' ContactName='Maria Anders'>
   <Orders ShipCity='Berlin' EmployeeName='Michael Suyama'>
     <Employees />
   </Orders>
   <Orders ShipCity='Berlin' EmployeeName='Margaret Peacock'>
     <Employees Region='WA' />
     <Employees Region='WA' />
   </Orders>
   <Orders ShipCity='Berlin' EmployeeName='Nancy Davolio'>
     <Employees Region='WA' />
     <Employees Region='WA' />
   </Orders>
   <Orders ShipCity='Berlin' EmployeeName='Janet Leverling'>
     <Employees Region='WA' />
   </Orders>
</Customers>
<Customers CustomerID='TRAIH' ContactName='Helvetius Nagy'>
   <Orders ShipCity='Kirkland' EmployeeName='Margaret Peacock'>
     <Employees Region='WA' />
   </Orders>
   <Orders ShipCity='Kirkland' EmployeeName='Anne Dodsworth'>
     <Employees />
   </Orders>
   <Orders ShipCity='Kirkland' EmployeeName='Michael Suyama'>
     <Employees />
   </Orders>
</Customers>
</ROOT>)";

/*
class CComUsageScope
{
public:
	CComUsageScope()	{ ::CoInitialize(NULL); }
	~CComUsageScope()	{ ::CoUninitialize(); }
};
*/

int main(int /*argc*/, char* /*argv[]*/)
{
	#if 0
    CComUsageScope comUsageScope;
#endif

	#if 0
    CComPtr<ISAXXMLReader> pSAXXMLReader;
#endif
	CSAXObjectContentHandlerImpl saxObjectContentHandler;

	#if 0
    HRESULT hr = pSAXXMLReader.CoCreateInstance(__uuidof(SAXXMLReader));
    if (FAILED(hr)) return 1;
    hr = pSAXXMLReader->putContentHandler(&saxObjectContentHandler);
    if (FAILED(hr)) return 1;
#else
    //int hr = 0;
#endif
		//return 1;

	CCustomersList customersList;
	// add root objects
	saxObjectContentHandler.AddRootObject("ROOT", &customersList);

	#if 0
    hr = pSAXXMLReader->parse(CComVariant(g_pszXml));
    if (FAILED(hr)) return hr;
#else
    auto hr = ParseXmlWithLibxml2(g_pszXml, &saxObjectContentHandler);
    if (hr != 0) return hr;
#endif

	std::ostringstream ostr;
	customersList.ToXml(ostr);
	std::cout << ostr.str();

	return 0;
}
