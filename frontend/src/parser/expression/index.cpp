#include "parser/expression/index.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/index.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> IndexExpressionParser::parse() {
  auto identifier_expression(
      dynamic_pointer_cast<IdentifierExpression>(left));
  if (!identifier_expression) {
    parser->get_logger()->error("Expected identifier expression",
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

  return make_shared<IndexExpression>(*identifier_expression->get_position(),
                                      identifier_expression, index_expression);
}
