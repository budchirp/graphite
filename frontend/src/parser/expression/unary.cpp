#include "parser/expression/unary.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/unary.hpp"
#include "lexer/token/token.hpp"
#include "logger/log_types.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> UnaryExpressionParser::parse() {
  Token op_token;

  shared_ptr<Expression> expression;
  if (left) {
    expression = left;

    op_token = parser->current_token;
    parser->eat_token();  // eat postfix
  } else {
    op_token = parser->current_token;
    parser->eat_token();  // eat prefix

    expression =
        ExpressionStatementParser(parser).parse_expression(Precedence::PREFIX);
    if (!expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }
  }

  return make_shared<UnaryExpression>(*expression->get_position(), op_token,
                                      expression);
}
