#include "parser/expression/var_ref.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/var_ref.hpp"
#include "lexer/token/token_type.hpp"
#include "parser/expression/identifier.hpp"

shared_ptr<Expression> VarRefExpressionParser::parse() {
  if (parser->next_token.type == TOKEN_LEFT_PARENTHESES ||
      parser->next_token.type == TOKEN_LEFT_BRACKET ||
      parser->next_token.type == TOKEN_LEFT_BRACE ||
      parser->previous_token.type == TOKEN_ASTERISK ||
      parser->previous_token.type == TOKEN_AMPERSAND ||
      parser->next_token.type == TOKEN_PLUSPLUS ||
      parser->next_token.type == TOKEN_MINUSMINUS ||
      parser->next_token.type == TOKEN_ASSIGN ||
      parser->next_token.type == TOKEN_DOT)
    return IdentifierExpressionParser(parser).parse();

  return parse_var_ref();
}
shared_ptr<VarRefExpression> VarRefExpressionParser::parse_var_ref() const {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  return make_shared<VarRefExpression>(position, identifier_token.literal);
}
