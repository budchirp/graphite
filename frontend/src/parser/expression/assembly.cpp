#include "parser/expression/assembly.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/assembly.hpp"
#include "ast/expression/string.hpp"
#include "lexer/token/token_type.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/string.hpp"

shared_ptr<Expression> AssemblyExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat asm

  if (parser->current_token.type != TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after asm keyword",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat (

  auto assembly = StringExpressionParser(parser).parse_string();
  if (!assembly) {
    parser->get_logger()->error("Failed to parse expression",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_COLON) {
    parser->get_logger()->error("Expected colon after assembly",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat :

  auto string_expression_parser = StringExpressionParser(parser);
  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  vector<pair<shared_ptr<StringExpression>, shared_ptr<IdentifierExpression>>>
      output;
  while (parser->current_token.type != TOKEN_COLON) {
    auto constraint_expression = string_expression_parser.parse_string();
    if (!constraint_expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    auto variable_expression = identifier_expression_parser.parse_identifier();
    if (!variable_expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    output.emplace_back(constraint_expression, variable_expression);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TOKEN_COLON) {
      parser->get_logger()->error("Expected colon or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat :

  vector<pair<shared_ptr<StringExpression>, shared_ptr<IdentifierExpression>>>
      input;
  while (parser->current_token.type != TOKEN_COLON) {
    auto constraint_expression = string_expression_parser.parse_string();
    if (!constraint_expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    auto variable_expression = identifier_expression_parser.parse_identifier();
    if (!variable_expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    input.emplace_back(constraint_expression, variable_expression);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TOKEN_COLON) {
      parser->get_logger()->error("Expected colon or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat :

  vector<shared_ptr<StringExpression>> clobbered;
  while (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
    auto register_expression = string_expression_parser.parse_string();
    if (!register_expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    clobbered.push_back(register_expression);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
      parser->get_logger()->error("Expected right parentheses or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat )

  return make_shared<AssemblyExpression>(position, assembly, output, input,
                                         clobbered);
}
