cmake_minimum_required(VERSION 3.16)
project(XMLSerializablePortable CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(xmlserial
    XMLSerializable.cpp
    #SAXContentHandlerImpl.cpp
    SAXObjectContentHandlerImpl.cpp
    #StdAfx.cpp
    Libxml2Driver.cpp
)

target_include_directories(xmlserial PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#if (WIN32)
#    target_compile_definitions(xmlserial PRIVATE XS_BASE XS_WINDOWS=1 _WIN32_WINNT=0x0601)
#    target_link_libraries(xmlserial PRIVATE msxml6 comsuppw)
#else()
    target_compile_definitions(xmlserial PRIVATE XS_BASE)
    #find_package(PkgConfig REQUIRED)
    #pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
    #target_compile_definitions(xmlserial PRIVATE XS_WINDOWS=0)

    find_package(LibXml2 REQUIRED)

    target_include_directories(xmlserial PRIVATE ${LIBXML2_INCLUDE_DIRS})
    #target_link_libraries(xmlserial PRIVATE ${LIBXML2_LIBRARIES})

    target_link_libraries(xmlserial PRIVATE LibXml2::LibXml2)

    target_compile_options(xmlserial PRIVATE ${LIBXML2_CFLAGS_OTHER})
#endif()
