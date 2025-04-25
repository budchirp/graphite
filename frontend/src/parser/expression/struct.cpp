#include "ast/expression/struct.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "lexer/token/token_type.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/struct.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> StructExpressionParser::parse() {
  return parse_struct();
}

shared_ptr<StructExpression> StructExpressionParser::parse_struct() const {
  const auto position = *parser->get_lexer()->position;

  auto name_expression(dynamic_pointer_cast<IdentifierExpression>(left));
  if (!name_expression) {
    parser->get_logger()->error("Expected identifier expression",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected { after identifier",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat {

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  unordered_map<string,
                pair<shared_ptr<IdentifierExpression>, shared_ptr<Expression>>>
      fields;
  while (parser->current_token.type != TOKEN_RIGHT_BRACE) {
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
      parser->get_logger()->error("Expected identifier as field name",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    auto field_name_expression =
        identifier_expression_parser.parse_identifier();

    if (parser->current_token.type != TOKEN_COLON) {
      parser->get_logger()->error("Expected : after field name",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    parser->eat_token();  // eat :

    fields.insert_or_assign(field_name_expression->value,
                            pair(field_name_expression,
                                 expression_statement_parser.parse_expression(
                                     Precedence::Value::LOWEST)));

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ,
    } else if (parser->current_token.type != TOKEN_RIGHT_BRACE) {
      parser->get_logger()->error("Expected right brace or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat }

  return make_shared<StructExpression>(position, name_expression, fields);
}
