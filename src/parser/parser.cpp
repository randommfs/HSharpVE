#include "parser/tokens.hpp"
#include <optional>
#include <iostream>
#include <functional>

#include <cassert>

#include <ve/exceptions.hpp>

static constexpr char parser_fail_msg[] = "Parser failed to parse expression.\n";

std::optional<HSharpParser::NodeBinExpr *> HSharpParser::Parser::parse_bin_expr() {
    std::string msg{};
    HSharpVE::EExceptionReason exc_type;
    std::optional<NodeExpression*> lhs;
    NodeBinExpr* bin_expr;
    if (!(lhs = parse_expression())) {
        msg.append(parser_fail_msg);
        exc_type = HSharpVE::EExceptionReason::UNEXPECTED_TOKEN;
        //goto error;
    }

    bin_expr = allocator.alloc<NodeBinExpr>();
    if (!peek().has_value()){
        msg.append(parser_fail_msg);
        exc_type = HSharpVE::EExceptionReason::EARLY_EOF;
        //goto error;
    }
    switch (peek()->type){
        case HSharpParser::EToken::ADDITION_SIGN:{
            auto bin_expr_add = allocator.alloc<NodeBinExprAdd>();
            bin_expr_add->lhs = lhs.value();
            skip();
            bin_expr_add->rhs = parse_expression().value();
            bin_expr->var = bin_expr_add;
            return bin_expr;
        }
        case HSharpParser::EToken::SUBTRACTION_SIGN:{
            auto bin_expr_sub = allocator.alloc<NodeBinExprSub>();
            bin_expr_sub->lhs = lhs.value();
            skip();
            bin_expr_sub->rhs = parse_expression().value();
            bin_expr->var = bin_expr_sub;
            return bin_expr;
        }
        case HSharpParser::EToken::STAR_SIGN:{
            auto bin_expr_mul = allocator.alloc<NodeBinExprMul>();
            bin_expr_mul->lhs = lhs.value();
            skip();
            bin_expr_mul->rhs = parse_expression().value();
            bin_expr->var = bin_expr_mul;
            return bin_expr;
        }
        // case TOK_FSLASH:{
        //     auto bin_expr_div = allocator.alloc<NodeBinExprDiv>();
        //     bin_expr_div->lhs = lhs.value();
        //     skip();
        //     bin_expr_div->rhs = parse_expression().value();
        //     bin_expr->var = bin_expr_div;
        //     return bin_expr;
        // }
        default:
            msg.append(parser_fail_msg);
            exc_type = HSharpVE::EExceptionReason::UNEXPECTED_TOKEN;
            goto error;
    }

    error:
    error(HSharpVE::EExceptionSource::TOKENIZER, exc_type, msg);
}

std::optional<HSharpParser::NodeTerm*> HSharpParser::Parser::parse_term() {
    auto type = peek()->type;
    switch (type){
        case HSharpParser::EToken::INTEGER_LITERAL:{
            std::optional<Token> int_lit = consume();
            auto term_int_lit = allocator.alloc<NodeTermIntLit>();
            auto expr = allocator.alloc<NodeTerm>();
            term_int_lit->int_lit = int_lit.value();
            expr->term = term_int_lit;
            return expr;
        }
        case HSharpParser::EToken::IDENTIFIER:{
            std::optional<Token> ident = consume();
            auto term_ident = allocator.alloc<NodeTermIdent>();
            auto expr = allocator.alloc<NodeTerm>();
            term_ident->ident = ident.value();
            expr->term = term_ident;
            return expr;
        }
        default:
            return {};
    }
}


std::optional<HSharpParser::NodeExpression *> HSharpParser::Parser::parse_expression() {
    if (auto term = parse_term()) {
        if (try_consume(HSharpParser::EToken::ADDITION_SIGN)) {
            auto bin_expr = allocator.alloc<NodeBinExpr>();
            auto bin_expr_add = allocator.alloc<NodeBinExprAdd>();
            auto lhs_expr = allocator.alloc<NodeExpression>();
            std::optional<NodeExpression*> rhs;
            lhs_expr->expr = term.value();
            bin_expr_add->lhs = lhs_expr;

        while (true){
            std::optional<NodeExpression*> expr_rhs;
            NodeExpression* expr_lhs2;
            NodeBinExpr* expr;
            std::optional<Token> cur_tok = peek();
            std::optional<int> prec;

            if (!cur_tok.has_value()) break;
            prec = bin_precedence(cur_tok->ttype);
            if (!prec.has_value() || prec < min_prec) break;

            Token token = consume();
            int next_min_prec = prec.value() + 1;
            expr_rhs = parse_expression(next_min_prec);
            if (!expr_rhs.has_value())
                HSharpVE::error(HSharpVE::EExceptionSource::PARSER,
                                HSharpVE::EExceptionReason::PARSE_ERROR,
                                "Failed to parse expression");

            expr = allocator.emplace<NodeBinExpr>();
            expr_lhs2 = allocator.emplace<NodeExpression>();
            switch(token.ttype){
                case TOK_PLUS:{
                    expr_lhs2->expr = expr_lhs->expr;
                    expr->var = allocator.emplace<NodeBinExprAdd>(expr_lhs2, expr_rhs.value());
                    break;
                }
                case TOK_MINUS:{
                    expr_lhs2->expr = expr_lhs->expr;
                    expr->var = allocator.emplace<NodeBinExprSub>(expr_lhs2, expr_rhs.value());
                    break;
                }
                case TOK_MUL_SIGN:{
                    expr_lhs2->expr = expr_lhs->expr;
                    expr->var = allocator.emplace<NodeBinExprMul>(expr_lhs2, expr_rhs.value());
                    break;
                }
                case TOK_FSLASH:{
                    expr_lhs2->expr = expr_lhs->expr;
                    expr->var = allocator.emplace<NodeBinExprDiv>(expr_lhs2, expr_rhs.value());
                    break;
                }
                default:
                    assert(false);
            }
            expr_lhs->expr = expr;
        }
    } else if (auto str_lit = try_consume(HSharpParser::EToken::STRING_LITERAL)) {
        auto expr_str_lit = allocator.alloc<NodeExpressionStrLit>();
        auto expr = allocator.alloc<NodeExpression>();
        expr_str_lit->str_lit = str_lit.value();
        expr->expr = expr_str_lit;
        expr->line = str_lit->line;
        return expr;
    }

    error:
    std::string msg{};
    msg.append("Parser failed to parse expression.\n");
    msg.append(std::format("Last token: {}", toString(tokens[index].type)));
    error(HSharpVE::EExceptionSource::PARSER, HSharpVE::EExceptionReason::UNEXPECTED_TOKEN, msg);
}

std::optional<HSharpParser::NodeStmt *> HSharpParser::Parser::parse_statement() {
    // if (peek().has_value() && peek().value().ttype == TokenType::TOK_EXIT &&
    //     peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_PAREN_OPEN) {
    //     auto stmt_exit = allocator.alloc<NodeStmtExit>();
    //     auto node_stmt = allocator.alloc<NodeStmt>();
    //     std::optional<NodeExpression*> node_expr;
    //     stmt_exit->line = consume().line;
    //     skip();

    //     if (!(node_expr = parse_expression())) goto error;

    //     stmt_exit->expr = node_expr.value();
    //     stmt_exit->line = node_expr.value()->line;

    //     try_consume(TokenType::TOK_PAREN_CLOSE, 1);
    //     try_consume(TokenType::TOK_SEMICOLON, 1);

    //     node_stmt->statement = stmt_exit;
    //     node_stmt->line = stmt_exit->line;
    //     return node_stmt;
    // } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_PRINT &&
    //            peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_PAREN_OPEN) {
    //     skip(2);
    //     auto stmt_print = allocator.alloc<NodeStmtPrint>();
    //     if (auto expr = parse_expression())
    //         stmt_print->expr = expr.value();

    //     try_consume(TokenType::TOK_PAREN_CLOSE, 1);
    //     try_consume(TokenType::TOK_SEMICOLON, 1);

    //     auto node_stmt = allocator.alloc<NodeStmt>();
    //     node_stmt->statement = stmt_print;
    //     return node_stmt;
    // } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_INPUT &&
    //            peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_PAREN_OPEN) {
    //     skip(2);
    //     auto node_input = allocator.alloc<NodeStmtInput>();
    //     if (auto expr = parse_expression())
    //         node_input->expr = expr.value();

    //     try_consume(TokenType::TOK_PAREN_CLOSE, 1);
    //     try_consume(TokenType::TOK_SEMICOLON, 1);

    //     auto node_stmt = allocator.alloc<NodeStmt>();
    //     node_stmt->statement = node_input;
    //     return node_stmt;
    // } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_VAR &&
    //            peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_IDENT &&
    //            peek(2).has_value() && peek(2).value().ttype == TokenType::TOK_EQUALITY_SIGN) {
    //     skip();
    //     auto node_stmt_var = allocator.alloc<NodeStmtVar>();
    //     auto node_stmt = allocator.alloc<NodeStmt>();
    //     std::optional<NodeExpression*> expr;
    //     node_stmt_var->ident = consume();
    //     skip();
    //     if (!(expr = parse_expression())) goto error;

    //     node_stmt_var->expr = expr.value();

    //     try_consume(TokenType::TOK_SEMICOLON, 1);

    //     node_stmt->statement = node_stmt_var;
    //     return node_stmt;
    // } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_IDENT &&
    //             peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_EQUALITY_SIGN) {
    //     auto node_stmt = allocator.alloc<NodeStmtVarAssign>();
    //     auto stmt = allocator.alloc<NodeStmt>();
    //     std::optional<NodeExpression*> expr;
    //     node_stmt->ident = consume();
    //     skip();
    //     if (!(expr = parse_expression())) goto error;

    //     node_stmt->expr = expr.value();

    //     try_consume(TokenType::TOK_SEMICOLON, 1);

    //     stmt->statement = node_stmt;
    //     return stmt;
    // }

    // error:
    // error(HSharpVE::EExceptionSource::PARSER, HSharpVE::EExceptionReason::SYNTAX_ERROR, parser_fail_msg);
}


std::optional<HSharpParser::NodeProgram> HSharpParser::Parser::parse_program() {
    NodeProgram program;
    while (peek().has_value()) {
        std::optional<NodeStmt*> stmt = parse_statement();
        if (!stmt.has_value())
            error(HSharpVE::EExceptionSource::PARSER, HSharpVE::EExceptionReason::SYNTAX_ERROR, "Parser failed to parse statement");

        program.statements.push_back(stmt.value());
    }
    return program;
}