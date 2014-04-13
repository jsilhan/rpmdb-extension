#include <gtest/gtest.h>


class MinimalistPrinter : public ::testing::EmptyTestEventListener {
    // Called before a test starts.
    virtual void OnTestStart(const ::testing::TestInfo& test_info) {
      // printf("*** Test %s.%s starting.\n",
      //        test_info.test_case_name(), test_info.name());
    }

    // Called after a failed assertion or a SUCCEED() invocation.
    virtual void OnTestPartResult(
        const ::testing::TestPartResult& test_part_result) {
      printf("\n%s in %s:%d\n%s\n\n",
             test_part_result.failed() ? "*** Failure" : "Success",
             test_part_result.file_name(),
             test_part_result.line_number(),
             test_part_result.summary());
    }

    // Called after a test ends.
    virtual void OnTestEnd(const ::testing::TestInfo& test_info) {
      // printf("*** Test %s.%s ending.\n",
      //        test_info.test_case_name(), test_info.name());
    }
  };

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new MinimalistPrinter);
    return RUN_ALL_TESTS();
}
