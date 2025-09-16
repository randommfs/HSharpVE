add_executable(tests
  Tokenizer/Tokenizer.cpp
  Tokenizer/Utils.cpp
)
target_link_libraries(tests hve-tokenizer gtest::gtest)
