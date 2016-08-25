cmake_minimum_required(VERSION 3.0)
link_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

option(desktop_build "desktop_build" ON)

message("-- desktop_build: " ${desktop_build})

add_subdirectory(3rd_party/dlib)
set (MODEL_RES_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}/res/offline_model\")
set (ONLINE_MODEL_RES_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}/res/online_model\")
message ("-- model directory: " ${MODEL_RES_DIRECTORY})
message ("-- online model directory: " ${ONLINE_MODEL_RES_DIRECTORY})
add_definitions(-DMODEL_RES_DIRECTORY=${MODEL_RES_DIRECTORY})
add_definitions(-DONLINE_MODEL_RES_DIRECTORY=${ONLINE_MODEL_RES_DIRECTORY})


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

FILE (GLOB_RECURSE sources src/*.cpp src/utils/*.cpp src/sleep_staging/*.cpp)

add_library (neuroon-alg-core SHARED ${sources})

target_include_directories (neuroon-alg-core PUBLIC src)
target_include_directories (neuroon-alg-core PUBLIC src/util)
target_include_directories (neuroon-alg-core PUBLIC src/sleep_staging)
target_include_directories (neuroon-alg-core PUBLIC src/sleep_staging/offline)
target_include_directories (neuroon-alg-core PUBLIC src/sleep_staging/online)
target_include_directories (neuroon-alg-core PUBLIC api)
target_include_directories (neuroon-alg-core PUBLIC 3rd_party/dlib)
target_include_directories (neuroon-alg-core PUBLIC 3rd_party/easylogging)

target_link_libraries(neuroon-alg-core
    dlib_shared
    )

add_custom_command(
        TARGET neuroon-alg-core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/sleep_staging/offline/model
                ${CMAKE_CURRENT_BINARY_DIR}/res/offline_model)

add_custom_command(
        TARGET neuroon-alg-core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/sleep_staging/online/model
                ${CMAKE_CURRENT_BINARY_DIR}/res/online_model)



if (desktop_build)
   find_package(GTest REQUIRED)
   include_directories(${GTEST_INCLUDE_DIRS})
  
   add_executable(runTests test/tests.cpp)
   target_link_libraries(runTests 
       ${GTEST_LIBRARIES}
        neuroon-alg-core
        pthread
        dlib
        )
        
    add_subdirectory(tools)
    target_include_directories (runTests PUBLIC 3rd_party/easylogging)
endif()

file(MAKE_DIRECTORY ./build)
file(MAKE_DIRECTORY ./build/functional_test_results)
file(MAKE_DIRECTORY ./build/res)
file(MAKE_DIRECTORY ./build/res/offline_model)
file(MAKE_DIRECTORY ./build/res/online_model)
     
     
SET(CMAKE_COLOR_MAKEFILE ON)
#SET(CMAKE_VERBOSE_MAKEFILE ON)
