#include "parser/expression/index.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/index.hpp"
#include "logger/log_types.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> IndexExpressionParser::parse() {
  parser->eat_token();  // eat [

  auto index_expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);
  if (!index_expression) {
    parser->get_logger()->error("Failed to parse expression",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
    parser->get_logger()->error("Expected closing right bracket",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ]

  return make_shared<IndexExpression>(*left->get_position(), left,
                                      index_expression);
}
