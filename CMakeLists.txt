cmake_minimum_required (VERSION 2.6)
project (Kiwi)
set (Kiwi_VERSION_MAJOR 0)
set (Kiwi_VERSION_MINOR 1)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "Building with C++11 support enabled")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()

# Include path
# include_directories(
#   ThirdParty/Flip/include/flip
# )

# Unit tests sources
set(TEST_SOURCES
    test/Modules/Core/test_KiwiCore.cpp
    )

# Unit tests executable
add_executable(test_suite ${TEST_SOURCES})

target_link_libraries(test_suite ${TEST_LIBS})