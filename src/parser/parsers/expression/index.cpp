#include "parser/parsers/expression/index.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/index.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"
#include "types/array.hpp"

unique_ptr<Expression> IndexExpressionParser::parse() {
  unique_ptr<VarRefExpression> variable_expression(
      dynamic_cast<VarRefExpression *>(left.release()));
  if (!variable_expression) {
    parser->get_logger()->error("Expected variable reference expression",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token(); // eat [

  auto index_expression = ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
    parser->get_logger()->error("Expected closing right bracket",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ]

  auto array_type = dynamic_pointer_cast<ArrayType>(variable_expression->get_type());

  return make_unique<IndexExpression>(*variable_expression->get_position(),
                                      array_type->child_type,
                                      std::move(variable_expression), std::move(index_expression));
}
