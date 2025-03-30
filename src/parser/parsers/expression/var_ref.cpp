#include "parser/parsers/expression/var_ref.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "parser/parsers/expression/identifier.hpp"

unique_ptr<Expression> VarRefExpressionParser::parse() {
  if (parser->next_token.type == TOKEN_LEFT_PARENTHESES)
    return IdentifierExpressionParser(parser).parse();

  return parse_var_ref();
}
unique_ptr<VarRefExpression> VarRefExpressionParser::parse_var_ref() {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  return make_unique<VarRefExpression>(position, identifier_token.literal);
}
