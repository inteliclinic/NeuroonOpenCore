find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

FILE (GLOB_RECURSE sources *.cpp *.c)


add_executable(functional_tests ${sources})
target_link_libraries(functional_tests
    ${GTEST_LIBRARIES}
     neuroon-alg-core
     pthread
     dlib
     )
target_include_directories (functional_tests PUBLIC 3rd_party/easylogging)
