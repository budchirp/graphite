#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/type.hpp"
#include "parser/parsers/expression/type.hpp"
#include "token/token_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> TypeExpressionParser::parse() {
  return make_unique<TypeExpression>(*parser->get_lexer()->position, parse_type(parser));
}

shared_ptr<Type> TypeExpressionParser::parse_type(const shared_ptr<Parser> &parser) {
  if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
    parser->eat_token(); // eat *
    return make_shared<PointerType>(parse_type(parser));
  }

  auto type_name = parser->current_token.literal;
  if (auto type = parser->get_env()->get_type(type_name)) {
    parser->eat_token();
    return type;
  } else {
    parser->get_logger()->error("Unknown type: " + type_name, LogTypes::Error::UNKNOWN);
    return nullptr;
  }
}
