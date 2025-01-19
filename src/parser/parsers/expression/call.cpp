#include <memory>
#include <vector>

#include "ast/expression.hpp"
#include "ast/expression/call.hpp"
#include "ast/expression/var_ref.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/call.hpp"
#include "parser/parsers/statement/expression.hpp"

unique_ptr<Expression> CallExpressionParser::parse() {
  // if (!dynamic_cast<IdentifierExpression *>(left.get())) {
  //   parser->get_logger()->error("Expected an identifier for call function",
  //                         LogTypes::Error::SYNTAX);
  //   return nullptr;
  // }

  unique_ptr<VarRefExpression> name(
      dynamic_cast<VarRefExpression *>(left.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after identifier",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  const auto position = *parser->get_lexer()->position;
  
  parser->eat_token(); // eat '('

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<unique_ptr<Expression>> arguments;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    arguments.push_back(
        expression_statement_parser.parse_expression(Precedence::LOWEST));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token(); // eat ','
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      parser->get_logger()->error("Expected right parentheses or comma",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token(); // eat ')'

  auto type = parser->get_program()->get_env()->get_symbol(name->get_value());
  if (!type) {
    parser->get_logger()->error("Undefined function `" + name->get_value() + "`", LogTypes::Error::UNDEFINED);
    return nullptr;
  }

  return make_unique<CallExpression>(position, type, std::move(name->identifier),
                                     std::move(arguments));
}
