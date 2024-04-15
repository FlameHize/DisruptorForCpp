set(PROJECT_BENCHMARK_DIR ${PROJECT_SOURCE_DIR}/benchmark)

#unicast 1P-1C
add_executable(unicast_1P-1C ${PROJECT_BENCHMARK_DIR}/unicast_1P_1C.cc)
target_link_libraries(unicast_1P-1C disruptor pthread)

#three step pipeline
add_executable(three_step_pipeline_1P-3C ${PROJECT_BENCHMARK_DIR}/three_step_pipeline_1P_3C.cc)
target_link_libraries(three_step_pipeline_1P-3C disruptor pthread)