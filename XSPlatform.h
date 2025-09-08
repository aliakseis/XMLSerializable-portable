#pragma once

// Minimal cross-platform helpers


#include <assert.h>
#include <cstdint>

// Portable GUID definition for non-Windows platforms
struct GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
};

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
