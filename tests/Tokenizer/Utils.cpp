#include "gtest/gtest.h"

#define private public
#include "Tokenizer.hpp"

TEST(Tokenizer, SkipWhitespace) {
  std::string test{"     \n \t \t\n"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_TRUE(tokenizer.SkipWhitespace());
  ASSERT_EQ(tokenizer.cur_line, 3);
}

TEST(Tokenizer, SkipWhitespace_EmptyStr) {
  std::string test;
  HVE::Tokenizer tokenizer(test);

  ASSERT_FALSE(tokenizer.SkipWhitespace());
}

TEST(Tokenizer, SkipComments_Multiline) {
  std::string test{"/* \n\n*/test"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_TRUE(tokenizer.SkipComments());
  ASSERT_EQ(tokenizer.cur_line, 3);
  ASSERT_NE(tokenizer.cur_raw_offset, test.length());
}

TEST(Tokenizer, SkipComments_Singleline) {
  std::string test{"//test\ntest"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_FALSE(tokenizer.SkipComments());
  ASSERT_EQ(tokenizer.cur_line, 2);
  ASSERT_NE(tokenizer.cur_raw_offset, test.length());
}

TEST(Tokenizer, SkipComments_Empty) {
  std::string test;
  HVE::Tokenizer tokenizer(test);

  ASSERT_FALSE(tokenizer.SkipComments());
}
