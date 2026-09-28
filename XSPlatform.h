#pragma once

// Minimal cross-platform helpers


#include <assert.h>
#include <cstdint>

#if defined(_WIN32)
  #include <guiddef.h>
#else
struct GUID {
    std::uint32_t Data1{};
    std::uint16_t Data2{};
    std::uint16_t Data3{};
    std::uint8_t Data4[8]{};
};
#endif

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
