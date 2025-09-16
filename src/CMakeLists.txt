add_library(hve-tokenizer STATIC
  Tokenizer/Tokenizer.cpp
)
target_link_libraries(hve-tokenizer eternal::eternal)
target_include_directories(hve-tokenizer PUBLIC Tokenizer)

add_executable(hve-ng
  Main/Main.cpp
)
target_link_libraries(hve-ng hve-tokenizer)
