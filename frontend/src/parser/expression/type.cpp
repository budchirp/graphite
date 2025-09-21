#include "parser/expression/type.hpp"

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/integer.hpp"
#include "semantic/type_helper.hpp"
#include "types/array.hpp"
#include "types/null.hpp"
#include "types/parser/parser_struct_type.hpp"
#include "types/parser/parser_unresolved_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

shared_ptr<Expression> TypeExpressionParser::parse() { return parse_type(); }

shared_ptr<TypeExpression> TypeExpressionParser::parse_type() {
  return make_shared<TypeExpression>(*parser->get_lexer()->position,
                                     get_type());
}

shared_ptr<Type> TypeExpressionParser::get_type() {
  if (parser->current_token.type == TOKEN_STRUCT) {
    parser->eat_token();  // eat struct

    if (parser->current_token.type != TOKEN_LEFT_BRACE) {
      parser->get_logger()->error("Expected { after struct",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    parser->eat_token();  // eat {

    auto identifier_expression_parser = IdentifierExpressionParser(parser);

    unordered_map<shared_ptr<IdentifierExpression>, shared_ptr<TypeExpression>>
        fields;
    while (parser->current_token.type != TOKEN_RIGHT_BRACE) {
      if (parser->current_token.type != TOKEN_IDENTIFIER) {
        parser->get_logger()->error("Expected identifier as field name",
                                    LogTypes::Error::SYNTAX);
        return nullptr;
      }

      auto field_name_expression =
          identifier_expression_parser.parse_identifier();
      if (!field_name_expression) {
        parser->get_logger()->error("Failed to parse identifier",
                                    LogTypes::Error::INTERNAL);
        return nullptr;
      }

      if (parser->current_token.type != TOKEN_COLON) {
        parser->get_logger()->error("Expected : after field name",
                                    LogTypes::Error::SYNTAX);
        return nullptr;
      }

      parser->eat_token();  // eat :

      auto field_type_expression = parse_type();
      if (!field_type_expression) {
        parser->get_logger()->error("Failed to parse type",
                                    LogTypes::Error::INTERNAL);
        return nullptr;
      }

      fields.insert_or_assign(field_name_expression, field_type_expression);
    }

    parser->eat_token();  // eat }
    
    return make_shared<ParserStructType>(fields);
  }

  bool is_mutable = false;

  if (parser->current_token.type == TOKEN_MUT) {
    parser->eat_token();  // eat mut
    is_mutable = true;
  }

  if (parser->current_token.type == TOKEN_ASTERISK) {
    parser->eat_token();  // eat *
    return make_shared<PointerType>(get_type(), is_mutable);
  }

  auto type_name = parser->current_token.literal;
  parser->eat_token();  // eat type name

  shared_ptr<Type> type = make_shared<ParserUnresolvedType>(type_name);

type_parser:
  switch (parser->current_token.type) {
    case TOKEN_LEFT_BRACKET: {
      parser->eat_token();  // eat [

      int size = -1;
      if (parser->current_token.type == TOKEN_INT) {
        size = stoi(IntegerExpressionParser(parser).parse_integer()->value);
      }

      if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
        parser->get_logger()->error("Expected closing right bracket",
                                    LogTypes::Error::SYNTAX);
        return nullptr;
      }

      parser->eat_token();  // eat ]

      type = make_shared<ArrayType>(type, size);
      goto type_parser;
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
