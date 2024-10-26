cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(
    rfloat
    VERSION 0.1.0
    LANGUAGES CXX
)

if(CMAKE_CROSSCOMPILING)
  set(TEST_RUNNER ${QEMU_RUN})
else()
  set(TEST_RUNNER)
endif()

find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)

FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
endif()

add_library(doctest INTERFACE)
message(WARNING "doctest_SOURCE_DIR: ${doctest_SOURCE_DIR}")
target_include_directories(doctest INTERFACE ${doctest_SOURCE_DIR}/doctest)


option(RFLOAT_BENCHMARKS "Enable benchmarks" OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/headers/*)
add_custom_target(format COMMAND clang-format-16 -i ${ALL_SOURCE_FILES})

# rfloat
add_library(rfloat INTERFACE)
target_include_directories(rfloat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers/rfloat)

# Tests
if(NOT CMAKE_CROSSCOMPILING)
add_executable(rfloat_tests)
target_link_libraries(rfloat_tests GTest::gtest_main rfloat)
target_compile_options(rfloat_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(rfloat_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rfloat_tests.cpp)

add_executable(rcmath_tests)
target_link_libraries(rcmath_tests GTest::gtest_main rfloat)
target_compile_options(rcmath_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(rcmath_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rcmath_tests.cpp)

add_executable(reproducibility_tests)
target_link_libraries(reproducibility_tests GTest::gtest_main rfloat)
target_compile_options(reproducibility_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(reproducibility_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/reproducibility_tests.cpp)

add_executable(cpp_examples)
set_target_properties(cpp_examples PROPERTIES CXX_STANDARD 23)
target_link_libraries(cpp_examples GTest::gtest_main rfloat)
target_compile_options(cpp_examples PRIVATE ${COMPILE_OPTIONS})
target_sources(cpp_examples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp_examples.cpp)

# Tools

add_executable(compiler_test)
target_link_libraries(compiler_test rfloat)
target_compile_options(compiler_test PRIVATE ${COMPILE_OPTIONS})
target_sources(compiler_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/compiler_test.cpp)

add_executable(gen_reproducibility_tests)
target_link_libraries(gen_reproducibility_tests rfloat)
target_compile_options(gen_reproducibility_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(gen_reproducibility_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/gen_repro_tests.cpp)

if(RFLOAT_BENCHMARKS)
    add_subdirectory(src/benchmarks)
endif()

enable_testing()
gtest_discover_tests(rfloat_tests PROPERTIES LAUNCHER "${TEST_RUNNER}")
gtest_discover_tests(rcmath_tests PROPERTIES LAUNCHER "${TEST_RUNNER}")
gtest_discover_tests(reproducibility_tests PROPERTIES LAUNCHER "${TEST_RUNNER}")

else()

add_executable(qemu_reproducibility_tests)
target_link_libraries(qemu_reproducibility_tests doctest rfloat)
target_compile_options(qemu_reproducibility_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(qemu_reproducibility_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/bare_metal_reproducibility_tests.cpp)

endif()

