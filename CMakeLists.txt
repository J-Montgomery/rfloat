cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(
    rfloat
    VERSION 1.0.0
    LANGUAGES CXX
)

include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
    SOURCE_DIR doctest
)

FetchContent_MakeAvailable(doctest)

option(RFLOAT_BENCHMARKS "Enable benchmarks" OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/headers/*)
add_custom_target(format COMMAND clang-format-16 -i ${ALL_SOURCE_FILES})

# rfloat
add_library(rfloat INTERFACE)
target_include_directories(rfloat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers/rfloat)

# Tests
add_executable(rfloat_tests)
target_link_libraries(rfloat_tests doctest rfloat)
target_compile_options(rfloat_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(rfloat_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rfloat_tests.cpp)

add_executable(rcmath_tests)
target_link_libraries(rcmath_tests doctest rfloat)
target_compile_options(rcmath_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(rcmath_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rcmath_tests.cpp)

add_executable(reproducibility_tests)
target_link_libraries(reproducibility_tests doctest rfloat)
target_compile_options(reproducibility_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(reproducibility_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/reproducibility_tests.cpp)

add_executable(compiler_bug_tests)
target_link_libraries(compiler_bug_tests doctest rfloat)
target_compile_options(compiler_bug_tests PRIVATE ${COMPILE_OPTIONS})
target_sources(compiler_bug_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/compiler_bug_tests.cpp)

add_executable(cpp_examples)
set_target_properties(cpp_examples PROPERTIES CXX_STANDARD 23)
target_link_libraries(cpp_examples doctest rfloat)
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
add_test(rfloat_tests rfloat_tests)
add_test(rcmath_tests rcmath_tests)
add_test(reproducibility_tests reproducibility_tests)
add_test(compiler_bug_tests compiler_bug_tests)
add_test(cpp_examples cpp_examples)

