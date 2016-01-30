#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
  // Run
  testing::InitGoogleTest(&argc, argv);
  //testing::GTEST_FLAG(print_time) = true;
  int result = RUN_ALL_TESTS();
  return result;
}
