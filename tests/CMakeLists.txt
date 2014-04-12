find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

file(GLOB swdb_tests ${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp)
# set(swdb_tests
#     test_main.cpp
#     test_table.cpp
# )

add_executable(test_main ${swdb_tests})

target_link_libraries(test_main libswdb ${GTEST_LIBRARIES} pthread)
add_test(test_main test_main " --gtest_color=yes --gtest_break_on_failure")


