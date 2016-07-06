cmake_minimum_required(VERSION 3.0)
link_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

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




FILE (GLOB_RECURSE sources src/*.cpp)

add_library (neuroon-alg-core ${sources})
target_include_directories (neuroon-alg-core PUBLIC src)

FILE (GLOB_RECURSE test_files test/*.cpp)
add_executable (neuroon-alg-core-test ${test_files})

target_link_libraries (neuroon-alg-core-test neuroon-alg-core)
