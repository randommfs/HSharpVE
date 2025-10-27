#include "Parser.hpp"
#include "Tokenizer.hpp"

#include <optional>
#include <stdexcept>

HVE::Parser::TranslationUnit HVE::Parser::Parser::Parse() {
  
}

std::optional<HVE::Parser::NodeTerm*> HVE::Parser::Parser::ParseTerm() {
  switch (Peek().type) {
  case TokenType::INT_LITERAL: {
    auto int_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermIntLit>(std::move(int_tok));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::FLOAT_LITERAL: {
    auto float_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermFloatLit>(std::move(float_tok));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::IDENTIFIER: {
    if (auto call = ParseFuncCall()) {
      return m_alloc.Emplace<NodeTerm>(call.value());
    }
    if (auto op = ParseSubscriptOp()) {
      return m_alloc.Emplace<NodeTerm>(op.value());
    }
    auto int_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermIdent>(std::move(int_tok));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::UP_ARROW: {
    Consume();
    if (!TryConsume(TokenType::UP_ARROW)) {
      throw std::runtime_error("Invalid up arrow usage");
    }
    auto ident = TryConsume(TokenType::IDENTIFIER);
    auto* lit = m_alloc.Emplace<NodeTermReflectIdent>(std::move(ident.value()));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::OPEN_PARENTHESIS: {
    Consume();
    auto expr = ParseExpr();
    if (!expr.value()) {
      throw std::runtime_error("Expected expression");
    }
    if (!TryConsume(TokenType::CLOSE_PARENTHESIS)) {
      throw std::runtime_error("No closing parenthesis for expression");
    }
    auto pexpr = m_alloc.Emplace<NodeTermParen>(expr.value());
    return m_alloc.Emplace<NodeTerm>(pexpr);
    break;
  }
  default:
    return {};
  }
}

std::optional<HVE::Parser::NodeExpr*> HVE::Parser::Parser::ParseExpr(int min_prec) {
  auto complex_term = ParseComplexTerm();
  if (!complex_term.has_value()) {
    return {};
  }
  std::optional<NodeTerm*> term_lhs;
  if (complex_term.value()->terms.size() == 1) {
    term_lhs = complex_term.value()->terms[0];
  } else {
    term_lhs = m_alloc.Emplace<NodeTerm>(complex_term.value());
  }
  auto expr_lhs = m_alloc.Emplace<NodeExpr>(term_lhs.value());
  while (true) {
    std::optional<Token> tok = Peek();
    std::optional<int> prec;
    if (tok.has_value()) {
      prec = GetPrecedence(tok->type);
      if (!prec.has_value() || prec < min_prec) {
        break;
      }
    } else {
      break;
    }
    auto op = Consume();
    auto expr_rhs = ParseExpr(prec.value() + 1);
    if (!expr_rhs.has_value()) {
      throw std::runtime_error("Failed to parse expression");
    }
    auto expr = m_alloc.Emplace<NodeBinExpr>();
    auto expr_lhs2 = m_alloc.Emplace<NodeExpr>();
    switch (op.type) {
    case TokenType::PLUS:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprAdd>(expr_lhs2, expr_rhs.value());
      break;
    case TokenType::MINUS:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprSub>(expr_lhs2, expr_rhs.value());
      break;
    case TokenType::STAR:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprMulti>(expr_lhs2, expr_rhs.value());
      break;
    case TokenType::SLASH:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprDiv>(expr_lhs2, expr_rhs.value());
      break;
    default:
      std::abort();
    }
    expr_lhs->var = expr;
  }
  return expr_lhs;
}

std::optional<HVE::Parser::NodeStmt*> HVE::Parser::Parser::ParseStatement() {
  std::optional<NodeExpr*> expr;
  auto stmt = m_alloc.Alloc<NodeStmt>();
  if (auto decl = ParseVarDeclaration()) {
    stmt->stmt = decl.value();
  } else if ((expr = ParseExpr()).has_value()) {
    if (TryPeek(TokenType::EQUAL)) {
      auto assign = ParseAssignment(expr.value());
      stmt->stmt = assign.value();
    } else {
      stmt->stmt = expr.value();
    }
  }
  if (!TryConsume(TokenType::SEMICOLON)) {
    throw std::runtime_error("Expected ';' in a statement");
  }
  return stmt;
}

std::optional<HVE::Parser::NodeScope*> HVE::Parser::Parser::ParseScope() {
  if (!TryConsume(TokenType::OPEN_BRACE)) {
    throw std::runtime_error("Expected '{' before scope");
  }
  auto scope = m_alloc.Emplace<NodeScope>();
  while (true) {
    std::optional<NodeStmt*> stmt = ParseStatement();
    if (!stmt.has_value()) {
      break;
    }
    scope->stmts.emplace_back(stmt.value());
  }
  if (!TryConsume(TokenType::CLOSE_BRACE)) {
    throw std::runtime_error("Expected '}' after scope");
  }
  return scope;
}

std::optional<HVE::Parser::NodeFuncCall*> HVE::Parser::Parser::ParseFuncCall() {
  if (TryPeek(TokenType::IDENTIFIER) && TryPeek(TokenType::OPEN_PARENTHESIS, 1)) {
    auto func_name = Consume();
    Consume();
    NodeFuncCall* call = m_alloc.Emplace<NodeFuncCall>();
    call->name = func_name;
    while (true) {
      std::optional<NodeExpr*> arg = ParseExpr();
      if (!arg) {
        break;
      }
      call->args.push_back(arg.value());
      if (!TryConsume(TokenType::COMMA)) {
        break;
      }
    }
    if (!TryConsume(TokenType::CLOSE_PARENTHESIS)) {
      throw std::runtime_error("Expected closing parenthesis in function call");
    }
    return call;
  } else if (TryPeek(TokenType::IDENTIFIER) && TryPeek(TokenType::OPEN_ANGLE_BRACKET, 1)) {
    auto func_name = Consume();
    Consume();
    NodeFuncCall* call = m_alloc.Emplace<NodeFuncCall>();
    call->name = func_name;
    while (true) {
      std::optional<NodeExpr*> arg = ParseExpr();
      if (!arg) {
        break;
      }
      call->template_args.push_back(arg.value());
      if (!TryConsume(TokenType::COMMA)) {
        break;
      }
    }
    if (!TryConsume(TokenType::CLOSE_ANGLE_BRACKET) || !TryConsume(TokenType::OPEN_PARENTHESIS)) {
      throw std::runtime_error("Invalid function call");
    }
    while (true) {
      std::optional<NodeExpr*> arg = ParseExpr();
      if (!arg) {
        break;
      }
      call->args.push_back(arg.value());
      if (!TryConsume(TokenType::COMMA)) {
        break;
      }
    }
    if (!TryConsume(TokenType::CLOSE_PARENTHESIS)) {
      throw std::runtime_error("Expected closing parenthesis in function call");
    }
    return call;

  }
  return {};
}

std::optional<HVE::Parser::NodeFuncDef*> HVE::Parser::Parser::ParseFuncDef() {
  switch (Peek().type) {
  case TokenType::PRIVATE:
  case TokenType::PUBLIC: {
    auto access_modifier = Consume();
    auto name = TryConsume(TokenType::IDENTIFIER);
    if (!name.has_value() || !TryConsume(TokenType::OPEN_PARENTHESIS)) {
      throw std::runtime_error("Invalid function definition");
    }
    while (true) {

    }
    break;
  }
  default:
    return {};
  }
}

std::string HVE::Parser::Parser::ParseType() {
  if (TryPeek(TokenType::OPEN_BRACKET) && TryPeek(TokenType::IDENTIFIER, 1) && TryPeek(TokenType::CLOSE_BRACKET, 2)) {
    Consume();
    auto ident = Consume();
    Consume();
    return ident.lexeme;
  } else if (TryPeek(TokenType::IDENTIFIER)) {
    return Consume().lexeme;
  }
  return {};
}

std::optional<HVE::Parser::NodeExpr*> HVE::Parser::Parser::ParseLValue() {

}

std::optional<HVE::Parser::NodeAssignment*> HVE::Parser::Parser::ParseAssignment(NodeExpr* lhs) {
  std::optional<HVE::Parser::NodeExpr*> expr;
  if (lhs == nullptr) {
    expr = ParseExpr();
    if (!expr) {
      return {};
    }
  } else {
    expr = lhs;
  }
  if (!TryPeek(TokenType::EQUAL)) {
    throw std::runtime_error("Expected '=' in assignment");
  }
  auto assignment = m_alloc.Emplace<NodeAssignment>();
  assignment->lhs = expr.value();

  assignment->op = Consume();
  expr = ParseExpr();
  if (!expr) {
    throw std::runtime_error("Expected expression");
  }
  assignment->rhs = expr.value();
  return assignment;
}

std::optional<HVE::Parser::NodeSubscriptOp*> HVE::Parser::Parser::ParseSubscriptOp() {
  if (!TryPeek(TokenType::IDENTIFIER) || !TryPeek(TokenType::OPEN_BRACKET, 1)) {
    return {};
  }
  auto ident = Consume();
  Consume();
  auto expr = ParseExpr();
  if (!expr) {
    throw std::runtime_error("Expected expression");
  }
  auto subscript_op = m_alloc.Emplace<NodeSubscriptOp>();
  subscript_op->ident = ident;
  subscript_op->subscript_args.push_back(expr.value());
  while (TryConsume(TokenType::COMMA)) {
    auto sexpr = ParseExpr();
    subscript_op->subscript_args.push_back(sexpr.value());
  }
  if (!TryConsume(TokenType::CLOSE_BRACKET)) {
    throw std::runtime_error("Expected closing bracket");
  }
  return subscript_op;
}

std::optional<HVE::Parser::NodeComplexTerm *> HVE::Parser::Parser::ParseComplexTerm() {
  auto term = ParseTerm();
  if (!term.has_value()) {
    return {};
  }
  if (TryConsume(TokenType::DOT)) {
    auto complex_term = m_alloc.Emplace<NodeComplexTerm>();
    complex_term->terms.push_back(term.value());
    auto sub_cmpx_term = ParseComplexTerm();
    if (sub_cmpx_term) {
      if (sub_cmpx_term.value()->terms.size() == 1) {
        complex_term->terms.push_back(sub_cmpx_term.value()->terms[0]);
      } else {
        complex_term->terms.insert(complex_term->terms.end(), sub_cmpx_term.value()->terms.begin(), sub_cmpx_term.value()->terms.end());
      }
    }
    return complex_term;
  }
  auto complex_term = m_alloc.Emplace<NodeComplexTerm>();
  complex_term->terms.push_back(term.value());
  return complex_term;
}


std::optional<HVE::Token> HVE::Parser::Parser::TryConsume(TokenType type) {
  if (Peek().type == type) {
    return Consume();
  }
  return std::nullopt;
}

HVE::Token HVE::Parser::Parser::Peek(std::uint8_t offset) {
  return m_tokens[m_current_token + offset];
}

bool HVE::Parser::Parser::TryPeek(TokenType type, std::uint8_t offset) {
  return !(m_current_token + offset >= m_tokens.size() || m_tokens[m_current_token + offset].type != type);
}


HVE::Token HVE::Parser::Parser::Consume() {
  return m_tokens[m_current_token++];
}
