#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expressions/if.hpp"
#include "frontend/parser/parsers/expression/group.hpp"
#include "frontend/parser/parsers/expression/if.hpp"
#include "frontend/parser/parsers/statement/block.hpp"
#include "utils/logger/logger.hpp"

IfExpressionParser::IfExpressionParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<Expression> IfExpressionParser::parse() {
  parser->eat_token(); // eat if

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    Logger::error("Expected left parantheses after if");
    return nullptr;
  }

  auto group_expression_parser = GroupExpressionParser(parser);
  auto condition = group_expression_parser.parse();

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    Logger::error("Expected left brace after if condition");
    return nullptr;
  }

  auto block_statement_parser = BlockStatementParser(parser);
  auto consequence = block_statement_parser.parse();

  unique_ptr<BlockStatement> alternative = nullptr;
  if (parser->current_token.type == TokenType::TOKEN_ELSE) {
    parser->eat_token(); // eat else

    if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
      Logger::error("Expected left brace after else");
    }

    alternative = block_statement_parser.parse();
  }

  return make_unique<IfExpression>(std::move(condition), std::move(consequence),
                                   std::move(alternative));
}
