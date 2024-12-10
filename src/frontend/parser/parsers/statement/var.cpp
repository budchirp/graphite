#include <memory>

#include "frontend/ast/expression/identifier.hpp"
#include "frontend/parser/parsers/expression/identifier.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/var.hpp"
#include "frontend/parser/precedence.hpp"
#include "frontend/token/token_type.hpp"
#include "logger/log_types.hpp"

VarStatementParser::VarStatementParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<VarStatement> VarStatementParser::parse() {
  parser->eat_token(); // eat var

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->logger->error("Expected identifier after var",
                          LogTypes::Error::SYNTAX);
  }

  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  auto name_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> name(
      dynamic_cast<IdentifierExpression *>(name_expression.release()));

  if (parser->current_token.type != TokenType::TOKEN_COLON) {
    parser->logger->error("Expected : after identifier",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token(); // eat :

  bool is_pointer = false;
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
      parser->eat_token(); // eat *
      is_pointer = true;
    } else {
      parser->logger->error("Expected type identifier after :",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  auto type_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> type(
      dynamic_cast<IdentifierExpression *>(type_expression.release()));
  if (is_pointer) {
    type = make_unique<IdentifierExpression>("*" + type->get_value());
  }

  if (parser->current_token.type != TokenType::TOKEN_ASSIGN) {
    parser->logger->error("Expected = after type", LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token(); // eat =

  auto expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);
  return make_unique<VarStatement>(std::move(name), std::move(type),
                                   std::move(expression));
}
