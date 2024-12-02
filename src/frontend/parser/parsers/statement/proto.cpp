#include <memory>

#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "frontend/parser/precedence.hpp"
#include "frontend/token/token_type.hpp"
#include "utils/logger/logger.hpp"

ProtoStatementParser::ProtoStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<ProtoStatement> ProtoStatementParser::parse() {
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    Logger::error("Expected identifier as prototype name");
    return nullptr;
  }

  auto expression_statement_parser = ExpressionStatementParser(parser);

  auto name_expression =
      expression_statement_parser.parse_expression(Precedence::CALL);
  unique_ptr<IdentifierExpression> name(
      static_cast<IdentifierExpression *>(name_expression.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    Logger::error("Expected left parantheses after prototype name");
  }

  parser->eat_token(); // eat (

  vector<unique_ptr<IdentifierExpression>> arguments;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
      Logger::error("Expected identifier as prototype argument");
      return nullptr;
    }

    auto argument_expression =
        expression_statement_parser.parse_expression(Precedence::LOWEST);
    arguments.push_back(unique_ptr<IdentifierExpression>(
        static_cast<IdentifierExpression *>(argument_expression.release())));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token(); // eat ,
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      Logger::error("Expected right parantheses or comma");
      return nullptr;
    }
  }

  parser->eat_token(); // eat )

  return make_unique<ProtoStatement>(std::move(name), std::move(arguments));
}
