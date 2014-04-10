find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

file(GLOB swdb_tests ${CMAKE_CURRENT_SOURCE_DIR}/test*.cpp)

add_executable(test_main ${swdb_tests})

# Link test executable against gtest & gtest_main
target_link_libraries(test_main ${libswdb} ${GTEST_LIBRARIES} pthread)
# ADD_TEST(test_main test_main)
add_test(test_main test_main " --gtest_color=yes --gtest_break_on_failure")


