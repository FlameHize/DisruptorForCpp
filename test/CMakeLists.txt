enable_testing()

#for gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(PROJECT_TEST_NAME disruptor_test)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
file(GLOB TEST_SRC_FILES ${PROJECT_TEST_DIR}/*.cc)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} 
                    disruptor
                    ${GTEST_LIBRARIES} 
                    pthread)

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})