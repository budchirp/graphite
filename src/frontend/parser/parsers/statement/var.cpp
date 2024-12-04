#include <memory>

#include "frontend/ast/expressions/identifier.hpp"
#include "frontend/parser/parsers/expression/identifier.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/var.hpp"
#include "frontend/parser/precedence.hpp"
#include "frontend/token/token_type.hpp"
#include "utils/logger/logger.hpp"

VarStatementParser::VarStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<VarStatement> VarStatementParser::parse() {
  parser->eat_token(); // eat var

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    Logger::error("Expected identifier after var");
  }

  auto identifier_expression_parser = IdentifierExpressionParser(parser);
  auto name_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> name(
      static_cast<IdentifierExpression *>(name_expression.release()));

  if (parser->current_token.type != TokenType::TOKEN_COLON) {
    Logger::error("Expected : after identifier");
    return nullptr;
  }

  parser->eat_token(); // eat :

  bool is_pointer = false;
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
      parser->eat_token(); // eat *
      is_pointer = true;
    } else {
      Logger::error("Expected type identifier after :");
      return nullptr;
    }
  }

  auto type_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> type(
      static_cast<IdentifierExpression *>(type_expression.release()));
  if (is_pointer) {
    type = make_unique<IdentifierExpression>("*" + type->get_value());
  }

  if (parser->current_token.type != TokenType::TOKEN_ASSIGN) {
    Logger::error("Expected = after type");
    return nullptr;
  }

  parser->eat_token(); // eat =

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto expression =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  return make_unique<VarStatement>(std::move(name), std::move(type),
                                   std::move(expression));
}
