#include "ast/expression/type.hpp"

#include <llvm/IR/DerivedTypes.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/integer.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/integer.hpp"
#include "parser/parsers/expression/type.hpp"
#include "semantic/type_helper.hpp"
#include "types/array.hpp"
#include "types/null.hpp"
#include "types/parser/unknown_parser_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> TypeExpressionParser::parse() { return parse_type(); }

unique_ptr<TypeExpression> TypeExpressionParser::parse_type() {
  return make_unique<TypeExpression>(*parser->get_lexer()->position,
                                     get_type());
}

shared_ptr<Type> TypeExpressionParser::get_type() {
  if (parser->current_token.type == TOKEN_ASTERISK) {
    parser->eat_token();  // eat *
    return make_shared<PointerType>(get_type());
  }

  auto type_name = parser->current_token.literal;

  parser->eat_token();

  shared_ptr<Type> type = make_shared<UnknownParserType>(type_name);

type_parser:
  switch (parser->current_token.type) {
    case TOKEN_LEFT_BRACKET: {
      parser->eat_token();  // eat [

      uint size = 1;
      if (parser->current_token.type == TOKEN_INT) {
        size =
            stoi(IntegerExpressionParser(parser).parse_integer()->get_value());
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
      if (TypeHelper::is_null(type)) {
        parser->get_logger()->error("You cant make null type nullable",
                                    LogTypes::Error::TYPE_MISMATCH);
        return nullptr;
      }

      parser->eat_token();  // eat ?

      type = make_shared<NullType>(type);
      break;
    }

    default:
      break;
  }

  return type;
}
