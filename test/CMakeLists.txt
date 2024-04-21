enable_testing()

#for gtest
find_package(GTest REQUIRED)

set(PROJECT_TEST_NAME disruptor_test)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
file(GLOB TEST_SRC_FILES ${PROJECT_TEST_DIR}/*.cc)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_TEST_NAME} 
                    ${GTEST_LIBRARIES} 
                    disruptor
                    pthread)

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})