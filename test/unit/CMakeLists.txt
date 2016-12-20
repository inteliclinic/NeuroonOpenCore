find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

FILE (GLOB_RECURSE sources *.cpp)

add_executable(unit_tests ${sources})
target_link_libraries(unit_tests
    ${GTEST_LIBRARIES}
     neuroon-alg-core
     pthread
     dlib
     )
target_include_directories (unit_tests PUBLIC 3rd_party/easylogging)
target_include_directories (unit_tests PUBLIC test)
