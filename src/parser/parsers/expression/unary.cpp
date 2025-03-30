#include "parser/parsers/expression/unary.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/unary.hpp"
#include "ast/expression/var_ref.hpp"
#include "lexer/token/token.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/var_ref.hpp"

unique_ptr<Expression> UnaryExpressionParser::parse() {
  Token op_token;

  unique_ptr<VarRefExpression> expression;

  if (left) {
    unique_ptr<VarRefExpression> _expression(
        dynamic_cast<VarRefExpression *>(left.release()));
    if (!_expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    expression = std::move(_expression);

    op_token = parser->current_token;
    parser->eat_token();  // eat postfix
  } else {
    op_token = parser->current_token;
    parser->eat_token();  // eat prefix

    expression = VarRefExpressionParser(parser).parse_var_ref();
    if (!expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  return make_unique<UnaryExpression>(*expression->get_position(), op_token,
                                      std::move(expression));
}
