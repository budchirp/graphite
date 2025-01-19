#include "parser/parsers/statement/for.hpp"

#include <memory>

#include "parser/parsers/statement/block.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/parsers/statement/var.hpp"
#include "parser/precedence.hpp"

unique_ptr<ForStatement> ForStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat for

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after for keyword",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat (

  auto init = VarStatementParser(parser).parse();

  if (parser->current_token.type != TokenType::TOKEN_SEMICOLON) {
    parser->get_logger()->error("Expected semicolon after initializer",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ;

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto condition =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TokenType::TOKEN_SEMICOLON) {
    parser->get_logger()->error("Expected semicolon after condition",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ;

  auto increment =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    parser->get_logger()->error("Expected right parentheses after expression",
                                LogTypes::Error::SYNTAX);
  }

  parser->eat_token();  // eat )

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after expressions",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto body = BlockStatementParser(parser).parse();
  if (!body) {
    parser->get_logger()->error("Failed to parse the body of the for statement",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<ForStatement>(position, std::move(init),
                                   std::move(condition), std::move(increment),
                                   std::move(body));
}
