#include "parser/expression/var_ref.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "parser/expression/identifier.hpp"

shared_ptr<Expression> VarRefExpressionParser::parse() {
  // asd(     function call
  // asd {    struct definition
  if (parser->next_token.type == TOKEN_LEFT_PARENTHESES ||
      parser->next_token.type == TOKEN_LEFT_BRACE)
    return IdentifierExpressionParser(parser).parse();

  return parse_var_ref();
}
shared_ptr<VarRefExpression> VarRefExpressionParser::parse_var_ref() const {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  return make_shared<VarRefExpression>(position, identifier_token.literal);
}
