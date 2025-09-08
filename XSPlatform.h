#pragma once

// Minimal cross-platform helpers


#if 0
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
  #include <ole2.h>
  #include <combaseapi.h>
  #include <comdef.h>
  #include <atlbase.h>
  #include <msxml6.h>
#endif

#include <assert.h>
#include <cstdint>
//#include <locale>
//#include <codecvt>

//#ifndef 0
// Portable GUID definition for non-Windows platforms
struct GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
};
//#endif



// Export/import
#if defined(_MSC_VER)
  #ifdef XS_BASE
    #define XS_API __declspec(dllexport)
  #else
    #define XS_API __declspec(dllimport)
  #endif
#else
  #define XS_API
#endif
