#include "parser/parsers/expression/var_ref.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "parser/parsers/expression/identifier.hpp"
#include "token/token_type.hpp"

unique_ptr<Expression> VarRefExpressionParser::parse() {
  if (parser->next_token.type == TOKEN_LEFT_PARENTHESES)
    return IdentifierExpressionParser(parser).parse();

  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  auto variable =
      parser->get_program()->get_env()->get_variable(identifier_token.literal);

  if (!variable || !variable->type) {
    parser->get_logger()->error(
        "Undefined variable `" + identifier_token.literal + "`",
        LogTypes::Error::UNDEFINED);
    return nullptr;
  }

  return make_unique<VarRefExpression>(position, variable->type,
                                       identifier_token.literal);
}
