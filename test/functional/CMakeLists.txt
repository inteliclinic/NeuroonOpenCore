find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(functional_tests functional_tests.cpp)
target_link_libraries(functional_tests
    ${GTEST_LIBRARIES}
     neuroon-alg-core
     pthread
     dlib
     )
target_include_directories (functional_tests PUBLIC 3rd_party/easylogging)
target_include_directories (functional_tests PUBLIC test)
