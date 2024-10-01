cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 23)
project(
    rfloat
    VERSION 0.0.1
    LANGUAGES CXX
)

find_package(GTest REQUIRED)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g3 -ggdb")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -Wall")

# rlibm

set(RLIBM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlibm)

set(RLIBM_SRCS
    ${RLIBM_DIR}/source/float/cosh.cpp
    ${RLIBM_DIR}/source/float/sinh.cpp
    ${RLIBM_DIR}/source/float/cospi.cpp
    ${RLIBM_DIR}/source/float/sinpi.cpp
    ${RLIBM_DIR}/source/float/log.cpp
    ${RLIBM_DIR}/source/float/log2.cpp
    ${RLIBM_DIR}/source/float/log10.cpp
    ${RLIBM_DIR}/source/float/exp.cpp
    ${RLIBM_DIR}/source/float/exp10.cpp)

add_library(rlibm)
target_include_directories(rlibm PUBLIC ${RLIBM_DIR}/include)
target_sources(rlibm PRIVATE ${RLIBM_SRCS})

# rfloat

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hh)
add_custom_target(format COMMAND clang-format-16 -i ${ALL_SOURCE_FILES})

add_library(rfloat INTERFACE)
target_include_directories(rfloat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers)

add_executable(rfloat_tests)
target_link_libraries(rfloat_tests GTest::gtest_main rfloat)
target_sources(rfloat_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rfloat_tests.cpp)


add_executable(rcmath_tests)
target_link_libraries(rcmath_tests GTest::gtest_main rfloat rlibm m)
target_sources(rcmath_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/rcmath_tests.cpp)

add_executable(gen_test_data)
target_link_libraries(gen_test_data rfloat)
target_sources(gen_test_data PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/gen_test_data.cpp)

enable_testing()
gtest_discover_tests(rfloat_tests)
gtest_discover_tests(rcmath_tests)

#####################################

