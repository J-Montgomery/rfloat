project(benchmarks)

add_executable(whetstone_bench_double whetstone.cpp)
target_link_libraries(whetstone_bench_double rfloat)
target_compile_options(whetstone_bench_double PRIVATE ${COMPILE_OPTIONS} -DFP_TYPE_STD_DOUBLE -DPRINTOUT)

add_executable(whetstone_bench_rdouble whetstone.cpp)
target_link_libraries(whetstone_bench_rdouble rfloat)
target_compile_options(whetstone_bench_rdouble PRIVATE ${COMPILE_OPTIONS} -DFP_TYPE_RDOUBLE -DPRINTOUT)