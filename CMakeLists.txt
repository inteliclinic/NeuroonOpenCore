cmake_minimum_required(VERSION 3.0)
link_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(3rd_party/dlib)
add_subdirectory(tools)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_BUILD_TYPE Debug)

FILE (GLOB_RECURSE sources src/*.cpp src/utils/*.cpp)

add_library (neuroon-alg-core ${sources})

target_include_directories (neuroon-alg-core PUBLIC src)
target_include_directories (neuroon-alg-core PUBLIC src/util)
target_include_directories (neuroon-alg-core PUBLIC api)
target_include_directories (neuroon-alg-core PUBLIC 3rd_party/dlib)

target_link_libraries(neuroon-alg-core
    dlib
    )



add_executable(runTests test/tests.cpp)
target_link_libraries(runTests 
    ${GTEST_LIBRARIES}
     neuroon-alg-core
     pthread
     dlib
     )

     
     
SET(CMAKE_COLOR_MAKEFILE ON)
#SET(CMAKE_VERBOSE_MAKEFILE ON)
