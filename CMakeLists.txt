cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
project(
    rfloat
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(GTest REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hh)
add_custom_target(format COMMAND clang-format-16 -i ${ALL_SOURCE_FILES})

# rfloat
add_library(rfloat INTERFACE)
target_include_directories(rfloat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers)

# Tests

add_executable(rfloat_tests)
target_link_libraries(rfloat_tests GTest::gtest_main rfloat)
target_compile_options(rfloat_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(rfloat_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rfloat_tests.cpp)

add_executable(rcmath_tests)
target_link_libraries(rcmath_tests GTest::gtest_main rfloat m)
target_compile_options(rcmath_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(rcmath_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rcmath_tests.cpp)

add_executable(gen_test_data)
target_link_libraries(gen_test_data rfloat)
target_sources(gen_test_data PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/gen_test_data.cpp)

enable_testing()
gtest_discover_tests(rfloat_tests)
gtest_discover_tests(rcmath_tests)

