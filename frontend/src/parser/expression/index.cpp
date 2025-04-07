#include "parser/expression/index.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/index.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> IndexExpressionParser::parse() {
  shared_ptr<VarRefExpression> variable_expression(
      static_pointer_cast<VarRefExpression>(left));
  if (!variable_expression) {
    parser->get_logger()->error("Expected variable reference expression",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat [

  auto index_expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
    parser->get_logger()->error("Expected closing right bracket",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ]

  return make_shared<IndexExpression>(*variable_expression->get_position(),
                                      variable_expression, index_expression);
}
