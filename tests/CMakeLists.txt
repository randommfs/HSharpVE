add_executable(tests
  Tokenizer/Tokenizer.cpp
  Tokenizer/Utils.cpp
  Parser/ParseTerm.cpp
)
target_link_libraries(tests hve-tokenizer hve-parser gtest::gtest)

add_test(NAME test COMMAND tests)
