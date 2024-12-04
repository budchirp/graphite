#include <memory>

#include "frontend/parser/parsers/expression/identifier.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "frontend/token/token_type.hpp"
#include "utils/logger/logger.hpp"

ProtoStatementParser::ProtoStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<ProtoStatement> ProtoStatementParser::parse() {
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    Logger::error("Expected identifier as prototype name");
    return nullptr;
  }

  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  auto name_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> name(
      static_cast<IdentifierExpression *>(name_expression.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    Logger::error("Expected left parantheses after prototype name");
  }

  parser->eat_token(); // eat (

  vector<
      pair<unique_ptr<IdentifierExpression>, unique_ptr<IdentifierExpression>>>
      parameters;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
      Logger::error("Expected identifier as prototype parameter");
      return nullptr;
    }

    auto parameter_expression = identifier_expression_parser.parse();

    if (parser->current_token.type != TokenType::TOKEN_COLON) {
      Logger::error("Expected : after parameter name");
      return nullptr;
    }

    parser->eat_token(); // eat :

    bool is_pointer = false;
    if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
      if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
        parser->eat_token(); // eat *
        is_pointer = true;
      } else {
        Logger::error("Expected type identifier after :");
        return nullptr;
      }
    }

    auto type_expression = identifier_expression_parser.parse();
    unique_ptr<IdentifierExpression> type(
        static_cast<IdentifierExpression *>(type_expression.release()));
    if (is_pointer) {
      type = make_unique<IdentifierExpression>("*" + type->get_value());
    }

    parameters.push_back(pair(
        unique_ptr<IdentifierExpression>(static_cast<IdentifierExpression *>(
            parameter_expression.release())),
        std::move(type)));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token(); // eat ,
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      Logger::error("Expected right parantheses or comma");
      return nullptr;
    }
  }

  parser->eat_token(); // eat )

  if (parser->current_token.type != TokenType::TOKEN_ARROW) {
    Logger::error("Expected -> after )");
    return nullptr;
  }

  parser->eat_token(); // eat ->

  bool is_pointer = false;
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
      parser->eat_token(); // eat *
      is_pointer = true;
    } else {
      Logger::error("Expected type identifier after :");
      return nullptr;
    }
  }

  auto type_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> type(
      static_cast<IdentifierExpression *>(type_expression.release()));
  if (is_pointer) {
    type = make_unique<IdentifierExpression>("*" + type->get_value());
  }

  return make_unique<ProtoStatement>(std::move(name), std::move(parameters),
                                     std::move(type));
}
