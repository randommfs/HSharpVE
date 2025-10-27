add_library(hve-tokenizer STATIC
  Tokenizer/Tokenizer.cpp
)
target_link_libraries(hve-tokenizer PRIVATE eternal::eternal)
target_include_directories(hve-tokenizer PUBLIC Tokenizer)
target_compile_options(hve-tokenizer PUBLIC -O0 -ggdb3)

add_library(hve-parser STATIC
    Parser/AST/ASTBuilder.cpp
        Parser/AST/VariableDecl.cpp
)
target_link_libraries(hve-parser PUBLIC hve-tokenizer)
target_include_directories(hve-parser PUBLIC Parser Arena)
target_compile_options(hve-parser PUBLIC -O0 -ggdb3)

add_library(hve-semantic-analyzer STATIC
    Parser/Analyzer/SemanticAnalyzer.cpp
)
target_link_libraries(hve-semantic-analyzer PUBLIC hve-parser)
target_include_directories(hve-semantic-analyzer PUBLIC Parser)
target_compile_options(hve-semantic-analyzer PUBLIC -O0 -ggdb3)

add_library(hve-mangler STATIC
    Mangle/Mangle.cpp)
target_link_libraries(hve-mangler PUBLIC hve-parser)
target_include_directories(hve-mangler PUBLIC Mangle)
target_compile_options(hve-mangler PUBLIC -O0 -ggdb3)

add_executable(hve-ng
  Main/Main.cpp
)
target_link_libraries(hve-ng hve-tokenizer)
