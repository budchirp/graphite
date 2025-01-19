#include "parser/parsers/expression/var_ref.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"

unique_ptr<Expression> VarRefExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  auto type =
      parser->get_program()->get_env()->get_symbol(identifier_token.literal);

  if (!type) {
    parser->get_logger()->error(
        "Undefined variable `" + identifier_token.literal + "`",
        LogTypes::Error::UNDEFINED);
    return nullptr;
  }

  return make_unique<VarRefExpression>(
      position, type,
      make_unique<IdentifierExpression>(position, identifier_token.literal));
}
