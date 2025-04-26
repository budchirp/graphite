#include "ast/expression/call.hpp"

#include <memory>
#include <vector>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/call.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> CallExpressionParser::parse() {
  auto name_expression(dynamic_pointer_cast<IdentifierExpression>(left));

  if (parser->current_token.type != TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after identifier",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat '('

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<shared_ptr<Expression>> arguments;
  while (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
    auto argument_expression =
        expression_statement_parser.parse_expression(Precedence::Value::LOWEST);
    if (!argument_expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    arguments.push_back(argument_expression);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
      parser->get_logger()->error("Expected right parentheses or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat ')'

  return make_shared<CallExpression>(*name_expression->get_position(),
                                     name_expression, arguments);
}
