set(PROJECT_BENCHMARK_DIR ${PROJECT_SOURCE_DIR}/benchmark)

#unicast 1P-1C
add_executable(unicast_1P-1C ${PROJECT_BENCHMARK_DIR}/unicast_1P_1C.cc)
target_link_libraries(unicast_1P-1C disruptor pthread)

#three step pipeline
add_executable(three_step_pipeline_1P-3C ${PROJECT_BENCHMARK_DIR}/three_step_pipeline_1P_3C.cc)
target_link_libraries(three_step_pipeline_1P-3C disruptor pthread)

#sequencer 
add_executable(sequencer_3P-1C ${PROJECT_BENCHMARK_DIR}/sequencer_3P_1C.cc)
target_link_libraries(sequencer_3P-1C disruptor pthread)

#multicast
add_executable(multicast_1P-3C ${PROJECT_BENCHMARK_DIR}/multicast_1P_3C.cc)
target_link_libraries(multicast_1P-3C disruptor pthread)

#diamond
add_executable(diamond_1P-3C ${PROJECT_BENCHMARK_DIR}/diamond_1P_3C.cc)
target_link_libraries(diamond_1P-3C disruptor pthread)