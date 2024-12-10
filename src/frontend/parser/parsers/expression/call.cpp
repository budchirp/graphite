#include <memory>
#include <vector>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/call.hpp"
#include "frontend/parser/parsers/expression/call.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

CallExpressionParser::CallExpressionParser(const shared_ptr<Parser> &parser,
                                           unique_ptr<Expression> left) {
  this->parser = parser;
  this->left = std::move(left);
}

unique_ptr<Expression> CallExpressionParser::parse() {
  if (!dynamic_cast<IdentifierExpression *>(left.get())) {
    parser->logger->error("Expected an identifier for call function",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  unique_ptr<IdentifierExpression> name(
      dynamic_cast<IdentifierExpression *>(left.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->logger->error("Expected left parentheses after identifier",
                          LogTypes::Error::SYNTAX);
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
      parser->logger->error("Expected right parentheses or comma",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token(); // eat ')'

  return make_unique<CallExpression>(std::move(name), std::move(arguments));
}
