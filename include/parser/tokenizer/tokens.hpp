#pragma once

#include <cstdint>
#include <string>

namespace hsharp {

    enum class EToken : std::int_fast8_t {
        BUILTIN_FUNCTION,               // built-in print, exit and so on
        IDENTIFIER,                     // variable, function or entity defined in runtime
        INTEGER_LITERAL,                // int literal
        STRING_LITERAL,                 // str literal
        STATEMENT_TERMINATOR,           // statement termination sign (semicolon)
        ADDITION_SIGN,                  // plus
        SUBTRACTION_SIGN,               // minus
        STAR_SIGN,                      // *
        ASSIGNMENT_SIGN,                // =
        PARENTHESIS_OPEN_SIGN,          // (
        PARENTHESIS_CLOSE_SIGN,         // )
        CONTEXT_OPEN_SIGN,              // {               
        CONTEXT_CLOSE_SIGN,             // }
        OWNING_SIGN,                    // .
        BRANCHING_START,                // if
        BRANCHING_ALT,                  // else
        BRANCHING_LIMITED_ALT           // elif
        // 229 more
    };

    std::string toString(EToken token);

    struct Token {
        EToken type;
        std::string payload;
    };

    Token makeToken(EToken type, std::string payload);
}
