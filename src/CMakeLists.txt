add_library(hve-tokenizer STATIC
  Tokenizer/Tokenizer.cpp
)
target_link_libraries(hve-tokenizer eternal::eternal)
target_include_directories(hve-tokenizer PUBLIC Tokenizer)
target_compile_options(hve-tokenizer PUBLIC -O0 -ggdb3)

add_library(hve-parser STATIC
  Parser/Parser.cpp
  Parser/VariableDecl.cpp
)
target_link_libraries(hve-parser hve-tokenizer)
target_include_directories(hve-parser PUBLIC Parser Arena)
target_compile_options(hve-parser PUBLIC -O0 -ggdb3)
add_executable(hve-ng
  Main/Main.cpp
)
target_link_libraries(hve-ng hve-tokenizer)
