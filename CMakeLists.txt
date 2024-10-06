cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(
    rfloat
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(GTest REQUIRED)

option(RFLOAT_BENCHMARKS "Enable benchmarks" OFF)
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
target_link_libraries(rcmath_tests GTest::gtest_main rfloat)
target_compile_options(rcmath_tests PRIVATE ${COMPILE_OPTIONS} -DRMATH_NONDETERMINISTIC -D__STDCPP_WANT_MATH_SPEC_FUNCS__)
target_sources(rcmath_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rcmath_tests.cpp)

add_executable(reproducibility_tests)
target_link_libraries(reproducibility_tests GTest::gtest_main rfloat)
target_compile_options(reproducibility_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(reproducibility_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/reproducibility_tests.cpp)

# Tools

add_executable(compiler_test)
target_link_libraries(compiler_test rfloat)
target_compile_options(compiler_test PRIVATE ${COMPILE_OPTIONS})
target_sources(compiler_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/compiler_test.cpp)

add_executable(gen_reproducibility_tests)
target_link_libraries(gen_reproducibility_tests rfloat)
target_compile_options(gen_reproducibility_tests PRIVATE ${COMPILE_OPTIONS} -DRMATH_NONDETERMINISTIC -Wunused-function)
target_sources(gen_reproducibility_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/gen_repro_tests.cpp)

if(RFLOAT_BENCHMARKS)
    add_subdirectory(src/benchmarks)
endif()

enable_testing()
gtest_discover_tests(rfloat_tests)
gtest_discover_tests(rcmath_tests)
gtest_discover_tests(reproducibility_tests)

