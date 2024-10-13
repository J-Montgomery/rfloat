project(benchmarks
    VERSION 0.1.0
    LANGUAGES CXX)

# Whetstone
add_executable(whetstone_bench_double whetstone.cpp)
target_link_libraries(whetstone_bench_double rfloat)
target_compile_options(whetstone_bench_double PRIVATE ${COMPILE_OPTIONS} -DFP_TYPE_STD_DOUBLE -DPRINTOUT)

add_executable(whetstone_bench_rdouble whetstone.cpp)
target_link_libraries(whetstone_bench_rdouble rfloat)
target_compile_options(whetstone_bench_rdouble PRIVATE ${COMPILE_OPTIONS} -DFP_TYPE_RDOUBLE -DPRINTOUT)

# Linpack
add_executable(linpack_bench_double linpack.cpp)
target_link_libraries(linpack_bench_double rfloat)
target_compile_options(linpack_bench_double PRIVATE ${COMPILE_OPTIONS} -DFP_TYPE_STD -DDP)

add_executable(linpack_bench_rdouble linpack.cpp)
target_link_libraries(linpack_bench_rdouble rfloat)
target_compile_options(linpack_bench_rdouble PRIVATE ${COMPILE_OPTIONS} -DFP_TYPE_R -DDP)