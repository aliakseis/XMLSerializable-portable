// XMLSerializable.cpp : Defines the entry point for the console application.
//

#include "XSPlatform.h"

#include "XSBase.h"

#include "SAXObjectContentHandlerImpl.h"

#include <iostream>
#include <sstream> 



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


int main(int /*argc*/, char* /*argv[]*/)
{
	CSAXObjectContentHandlerImpl saxObjectContentHandler;

	CCustomersList customersList;
	// add root objects
	saxObjectContentHandler.AddRootObject("ROOT", &customersList);

    auto hr = ParseXmlWithLibxml2(g_pszXml, &saxObjectContentHandler);
    if (hr != 0) return hr;

	std::ostringstream ostr;
	customersList.ToXml(ostr);
	std::cout << ostr.str();

	return 0;
}
