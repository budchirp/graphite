#include <memory>

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

  if (parser->current_token.type != TokenType::TOKEN_ASSIGN) {
    Logger::error("Expected = after identifier");
  }

  parser->eat_token(); // eat =

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto expression =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  return make_unique<VarStatement>(std::move(name), std::move(expression));
}
