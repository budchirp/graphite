#include "ast/expression/if.hpp"

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/group.hpp"
#include "parser/parsers/expression/if.hpp"
#include "parser/parsers/statement/block.hpp"

unique_ptr<Expression> IfExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat if

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after if",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto condition = GroupExpressionParser(parser).parse();
  if (!condition) {
    parser->get_logger()->error(
        "Failed to parse the condition of the if expression",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after condition",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto block_statement_parser = BlockStatementParser(parser);
  auto consequence = block_statement_parser.parse();
  if (!consequence) {
    parser->get_logger()->error("Failed to parse if body",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  unique_ptr<BlockStatement> alternative = nullptr;
  if (parser->current_token.type == TokenType::TOKEN_ELSE) {
    parser->eat_token();  // eat else

    if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
      parser->get_logger()->error("Expected left brace after else",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    alternative = block_statement_parser.parse();
    if (!alternative) {
      parser->get_logger()->error("Failed to parse else body",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  return make_unique<IfExpression>(position, consequence->get_type(),
                                   std::move(condition), std::move(consequence),
                                   std::move(alternative));
}
