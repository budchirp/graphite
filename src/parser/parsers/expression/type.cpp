#include "ast/expression/type.hpp"

#include <llvm/IR/DerivedTypes.h>

#include <memory>

#include "analyzer/analyzer.hpp"
#include "ast/expression.hpp"
#include "ast/expression/integer.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/integer.hpp"
#include "parser/parsers/expression/type.hpp"
#include "token/token_type.hpp"
#include "types/array.hpp"
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
  if (auto actual_type =
          parser->get_program()->get_env()->get_type(type_name)) {
    parser->eat_token();

    shared_ptr<Type> type = actual_type;

  type_parser:
    switch (parser->current_token.type) {
      case TOKEN_LEFT_BRACKET: {
        parser->eat_token();  // eat [

        uint size = 1;
        if (parser->current_token.type == TOKEN_INT) {
          size = IntegerExpressionParser(parser).parse_integer()->get_value();
        }

        if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
          parser->get_logger()->error("Expected closing right bracket",
                                      LogTypes::Error::SYNTAX);
          return nullptr;
        }

        parser->eat_token();  // eat ]

        type = make_shared<ArrayType>(type, size);
        goto type_parser;
        break;
      }

      case TOKEN_QUESTION_MARK: {
        if (Analyzer::is_null(type).first) {
          parser->get_logger()->error("You cant make null type nullable",
                                      LogTypes::Error::TYPE_MISMATCH);
          return nullptr;
        }

        parser->eat_token();  // eat ?

        if (auto pointer_type = Analyzer::is_pointer(type);
            pointer_type.first) {
          return make_shared<PointerType>(
              make_shared<NullType>(pointer_type.second->pointee_type));
        }

        type = make_shared<NullType>(type);
        break;
      }

      default:
        break;
    }

    return type;
  } else {
    return nullptr;
  }
}
