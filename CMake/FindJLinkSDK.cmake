cmake_minimum_required(VERSION 3.5)

find_path(JLINKSDK_PREFIX Inc/JLinkARMDLL.h
    PATHS "${CMAKE_SOURCE_DIR}/3rdParty" "$ENV{ProgramFiles}/SEGGER"
    PATH_SUFFIXES JLink_SDK_V694a JLink_SDK_V694c JLink_SDK_V696 JLink_SDK_V698a
    REQUIRED
)

set(JLINKSDK_INCLUDE_DIR "${JLINKSDK_PREFIX}/Inc")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(_jlinksdk_library_path_suffix Etc_GCC)
    set(_jlinksdk_libraries JLink SYS UTIL)
else()
    set(_jlinksdk_library_path_suffix Etc)
    set(_jlinksdk_libraries JLink JLinkARM SYS UTIL)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_jlinksdk_library_suffix _x64)
endif()

foreach(_lib ${_jlinksdk_libraries})
    find_library(_jlinksdk_lib_${_lib} ${_lib}${_jlinksdk_library_suffix}
        PATHS ${JLINKSDK_PREFIX}
        PATH_SUFFIXES ${_jlinksdk_library_path_suffix}
        REQUIRED
    )
    set(JLINKSDK_LIBRARY ${JLINKSDK_LIBRARY} "${_jlinksdk_lib_${_lib}}")
endforeach()

message("-- SEGGER JLink SDK found in " ${JLINKSDK_PREFIX})
