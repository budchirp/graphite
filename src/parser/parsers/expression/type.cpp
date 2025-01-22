#include "ast/expression/type.hpp"

#include <memory>

#include "analyzer/analyzer.hpp"
#include "ast/expression.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/type.hpp"
#include "token/token_type.hpp"
#include "types/null.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> TypeExpressionParser::parse() {
  return make_unique<TypeExpression>(*parser->get_lexer()->position,
                                     parse_type());
}

shared_ptr<Type> TypeExpressionParser::parse_type() {
  if (parser->current_token.type == TOKEN_ASTERISK) {
    parser->eat_token();  // eat *
    return make_shared<PointerType>(parse_type());
  }

  auto type_name = parser->current_token.literal;
  if (auto type = parser->get_program()->get_env()->get_type(type_name)) {
    parser->eat_token();

    switch (parser->current_token.type) {
      case TOKEN_QUESTION_MARK:
        if (Analyzer::is_null(type).first) {
          parser->get_logger()->error("You cant make null type nullable",
                                      LogTypes::Error::TYPE_MISMATCH);
          return nullptr;
        }

        parser->eat_token();  // eat ?

        if (auto pointer_type = Analyzer::is_pointer(type); pointer_type.first) {
          return make_shared<PointerType>(make_shared<NullType>(pointer_type.second->type));
        }

        return make_shared<NullType>(type);

      default:
        return type;
    }
  } else {
    return nullptr;
  }
}
