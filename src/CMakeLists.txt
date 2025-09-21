add_library(hve-tokenizer STATIC
  Tokenizer/Tokenizer.cpp
)
target_link_libraries(hve-tokenizer eternal::eternal)
target_include_directories(hve-tokenizer PUBLIC Tokenizer)

add_library(hve-parser STATIC
  Parser/Parser.cpp
)
target_link_libraries(hve-parser hve-tokenizer)
target_include_directories(hve-parser PUBLIC Parser)

add_executable(hve-ng
  Main/Main.cpp
)
target_link_libraries(hve-ng hve-tokenizer)
