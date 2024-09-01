#pragma once

// STD
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// Local
#include <common/arena.hpp>
#include <parser/parser/nodes.hpp>
#include <parser/tokenizer/tokens.hpp>
#include <parser/parser/interfaces/i-token-parser.hpp>


namespace hsharp {

    bool is_bin_operator(EToken ttype);
    std::optional<int> bin_precedence(EToken ttype);

    // class Parser {
    // protected:
    //     const std::vector<Token>& tokens;
    //     const std::vector<std::string>& lines;
    //     std::size_t index = 0;
    //     ArenaAllocator allocator;

    //     [[nodiscard]] std::optional<Token> peek(int offset = 0) const;
    //     Token try_consume(EToken type, int mode);
    //     std::optional<Token> try_consume(EToken type);
    //     Token consume();
    //     void skip(int count = 1);

    //     std::optional<NodeStmt*> parse_statement();
    //     std::optional<NodeExpression*> parse_expression(int min_prec = 0);
    //     std::optional<NodeBinExpr*> parse_bin_expr();
    //     std::optional<NodeTerm*> parse_term();

    // public:
    //     Parser(std::vector<Token>&& tokens);

    //     std::optional<NodeProgram> parse_program();
    // };
    
    class Parser {
    public:
        Parser();
        hsharp::arena_ptr<nodes::Program> parse(std::vector<Token> tokens);

    private:

    };
}
