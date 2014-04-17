find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(GMOCK_ROOT "/usr" CACHE PATH "Path to the gmock root directory.")
find_package(GMock REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})

file(GLOB swdb_tests ${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp)
add_executable(test_main ${swdb_tests})

target_link_libraries(test_main libswdb ${GMOCK_BOTH_LIBRARIES} pthread)
add_test(test_main test_main " --gtest_color=yes --gtest_break_on_failure")


