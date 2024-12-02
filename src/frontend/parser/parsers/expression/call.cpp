#include <memory>
#include <vector>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expressions/call.hpp"
#include "frontend/parser/parsers/expression/call.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "utils/logger/logger.hpp"

CallExpressionParser::CallExpressionParser(shared_ptr<Parser> parser,
                                           unique_ptr<Expression> left) {
  this->parser = parser;
  this->left = std::move(left);
}

unique_ptr<Expression> CallExpressionParser::parse() {
  auto *identifier_expression =
      dynamic_cast<IdentifierExpression *>(left.get());
  if (!identifier_expression) {
    Logger::error("Expected an identifier for call function");
  }

  unique_ptr<IdentifierExpression> name(
      static_cast<IdentifierExpression *>(left.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    Logger::error("Expected left parentheses after identifier");
    return nullptr;
  }

  parser->eat_token(); // eat '('

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<unique_ptr<Expression>> arguments;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    arguments.push_back(
        expression_statement_parser.parse_expression(Precedence::LOWEST));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token(); // eat ','
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      Logger::error("Expected right parantheses or comma");
      return nullptr;
    }
  }

  parser->eat_token(); // eat ')'

  return make_unique<CallExpression>(std::move(name), std::move(arguments));
}
