cmake_minimum_required(VERSION 3.28)

project(HSharpVE LANGUAGES CXX C VERSION 0.0)

set(MAIN_SRC
    src/Main/Main.cpp
    src/Main/ArgsHandler.cpp
PARENT_SCOPE)

set(TESTS_SRC
    test/tests_tokenizer.cpp
    test/tests_parser.cpp
    test/tests_ve.cpp
PARENT_SCOPE)

set(PARSER_SRC Parser/Parser.cpp)

set(COMPILER_SRC
    Compiler/Compiler_NoOptimizations.cpp
    Compiler/NoOpts/Expressions.cpp
    Compiler/NoOpts/Tokens.cpp
    Compiler/Compiler_WithOptimizations.cpp)

add_library(parser STATIC ${PARSER_SRC})
add_library(compiler STATIC ${COMPILER_SRC})

target_include_directories(parser PUBLIC ${INCLUDE_DIR})
target_include_directories(compiler PUBLIC ${INCLUDE_DIR})

target_link_libraries(parser PUBLIC pog)
