#include "ast/expression/call.hpp"

#include <memory>
#include <vector>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/call.hpp"
#include "parser/parsers/statement/expression.hpp"

unique_ptr<Expression> CallExpressionParser::parse() {
  unique_ptr<IdentifierExpression> name_expression(
      dynamic_cast<IdentifierExpression *>(left.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after identifier",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat '('

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<unique_ptr<Expression>> arguments;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    arguments.push_back(
        expression_statement_parser.parse_expression(Precedence::LOWEST));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      parser->get_logger()->error("Expected right parentheses or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat ')'

  return make_unique<CallExpression>(position, std::move(name_expression),
                                     std::move(arguments));
}
